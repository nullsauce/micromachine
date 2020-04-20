/*  Copyright (C) 2014  Adam Green (https://github.com/adamgreen)

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/

#include "CpuTestFixture.hpp"


/* PUSH
   Encoding: 1100 0 Rn:3 RegisterList:8 */
MICROMACHINE_TEST_F(stm, JustPushR0WithR7AsAddress, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("11000nnnrrrrrrrr", registers::R7, (1 << 0));
	getCpu().regs().set(registers::R7, INITIAL_PC + 16);
	getCpu().mem().write32(INITIAL_PC + 16, 0xFFFFFFFF);
	Step();
	ExpectThat().Register(registers::R7).Equals(INITIAL_PC + 16 + 1 * 4);
	EXPECT_EQ(0x0, getCpu().mem().read32(INITIAL_PC + 16));
}

MICROMACHINE_TEST_F(stm, JustPushR7WithR0AsAddress, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	getCpu().regs().set(registers::R7, 0x77777777);
	code_gen().emit_ins16("11000nnnrrrrrrrr", registers::R0, (1 << 7));
	getCpu().regs().set(registers::R0, INITIAL_PC + 16);
	getCpu().mem().write32(INITIAL_PC + 16, 0xFFFFFFFF);
	Step();
	ExpectThat().Register(registers::R0).Equals(INITIAL_PC + 16 + 1 * 4);
	EXPECT_EQ(0x77777777, getCpu().mem().read32(INITIAL_PC + 16));
}

MICROMACHINE_TEST_F(stm, PushAllWithR0AsAddress, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("11000nnnrrrrrrrr", registers::R0, 0xFF);
	getCpu().regs().set(registers::R0, INITIAL_PC + 16);

	for (int i = 1; i < 8; i++) {
		getCpu().regs().set(i, 0x11111111U * i);
	}

	for (int i = 0; i < 8; i++) {
		getCpu().mem().write32(INITIAL_PC + 16 + 4 * i, 0xFFFFFFFF);
	}

	Step();
	ExpectThat().Register(registers::R0).Equals(INITIAL_PC + 16 + 8 * 4);
	EXPECT_EQ(INITIAL_PC + 16, getCpu().mem().read32(INITIAL_PC + 16 + 4 * 0));
	for (int i = 1; i < 8; i++) {
		EXPECT_EQ(0x11111111U * i, getCpu().mem().read32(INITIAL_PC + 16 + 4 * i));
	}
}

MICROMACHINE_TEST_F(stm, PushAllButR7WithR7AsAddress, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("11000nnnrrrrrrrr", registers::R7, 0x7F);
	getCpu().regs().set(registers::R7, INITIAL_PC + 16);

	for (int i = 0; i < 7; i++) {
		getCpu().regs().set(i, 0x11111111U * i);
	}

	for (int i = 0; i < 7; i++) {
		getCpu().mem().write32(INITIAL_PC + 16 + 4 * i, 0xFFFFFFFF);
	}
	Step();
	ExpectThat().Register(registers::R7).Equals(INITIAL_PC + 16 + 7 * 4);
	for (int i = 0; i < 7; i++) {
		EXPECT_EQ(0x11111111U * i, getCpu().mem().read32(INITIAL_PC + 16 + 4 * i));
	}
}

MICROMACHINE_TEST_F(stm, HardFaultFromInvalidMemoryWrite, CpuTestFixture) {
	code_gen().emit_ins16("11000nnnrrrrrrrr", registers::R0, 1 << 0);
	getCpu().regs().set(registers::R0, 0xFFFFFFFC);
	Step();
	ExpectThat().HardfaultHandlerReached();
}
/*
TEST_SIM_ONLY(stm, UnpredictableToPushNoRegisters)
{
    code_gen().emit_ins16("11000nnnrrrrrrrr", registers::R0, 0);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    ExpectThat().Register(registers::PC).Equals(INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(stm, UnpredictableToPushWritebackRegisterWhichIsntFirstSaved)
{
    code_gen().emit_ins16("11000nnnrrrrrrrr", registers::R7, 0xFF);
    getCpu().regs().set(registers::R7, INITIAL_PC + 16);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    ExpectThat().Register(registers::PC).Equals(INITIAL_PC);
    step(&m_context);
}*/
