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
   Encoding: 1011 0 10 M:1 RegisterList:8 */
MICROMACHINE_TEST_F(push, JustPushLR, CpuTestFixture) {
	const uint32_t INITIAL_SP = 0x00002000;
	const uint32_t INITIAL_LR = 0xeeeeeeee;
	getCpu().regs().set(registers::SP, INITIAL_SP);
	getCpu().regs().set(registers::LR, INITIAL_LR);
	code_gen().emit_ins16("1011010Mrrrrrrrr", 1, 0);
	getCpu().mem().write32(INITIAL_SP - 4, 0x0);
	Step();
	ExpectThat().Register(registers::SP).Equals(INITIAL_SP - 4);
	EXPECT_EQ(INITIAL_LR, getCpu().mem().read32(INITIAL_SP - 4));
}

MICROMACHINE_TEST_F(push, JustPushR0, CpuTestFixture) {
	const uint32_t INITIAL_SP = 0x00002000;
	getCpu().regs().set(registers::SP, INITIAL_SP);
	code_gen().emit_ins16("1011010Mrrrrrrrr", 0, 1);
	getCpu().mem().write32(INITIAL_SP - 4, 0xFFFFFFFF);
	Step();
	ExpectThat().Register(registers::SP).Equals(INITIAL_SP - 4);
	EXPECT_EQ(0x0, getCpu().mem().read32(INITIAL_SP - 4));
}

MICROMACHINE_TEST_F(push, JustPushR7, CpuTestFixture) {
	const uint32_t INITIAL_SP = 0x00002000;
	getCpu().regs().set(registers::SP, INITIAL_SP);
	getCpu().regs().set(registers::R7, 0x77777777);
	code_gen().emit_ins16("1011010Mrrrrrrrr", 0, 1 << 7);
	getCpu().mem().write32(INITIAL_SP - 4, 0xFFFFFFFF);
	Step();
	EXPECT_EQ(0x77777777, getCpu().mem().read32(INITIAL_SP - 4));
	ExpectThat().Register(registers::SP).Equals(INITIAL_SP - 4);
}

MICROMACHINE_TEST_F(push, PushAll, CpuTestFixture) {
	const uint32_t INITIAL_SP = 0x00002000;
	const uint32_t INITIAL_LR = 0xeeeeeeee;
	getCpu().regs().set(registers::LR, INITIAL_LR);
	getCpu().regs().set(registers::SP, INITIAL_SP);
	for (reg_idx i = 0; i < 8; i++) {
		getCpu().regs().set(i, 0x11111111 * i);
	}

	code_gen().emit_ins16("1011010Mrrrrrrrr", 1, 0xFF);
	for (int i = 1; i <= 9; i++) {
		getCpu().mem().write32(INITIAL_SP - 4 * i, 0xFFFFFFFF);
	}

	Step();
	ExpectThat().Register(registers::SP).Equals(INITIAL_SP - 4 * 9);
	for (int i = 0; i < 8; i++) {
		EXPECT_EQ(0x11111111U * i, getCpu().mem().read32(INITIAL_SP - 4 * (9 - i)));
	}
	EXPECT_EQ(INITIAL_LR, getCpu().mem().read32(INITIAL_SP - 4));
}

MICROMACHINE_TEST_F(push, HardFaultFromInvalidMemoryWrite, CpuTestFixture) {
	code_gen().emit_ins16("1011010Mrrrrrrrr", 0, 1);
	getCpu().regs().set(registers::SP, 0xFFFFFFFC);
	Step();
	ExpectThat().HardfaultHandlerReached();
}

/*
TEST_SIM_ONLY(push, UnpredictableToPushNoRegisters)
{
    code_gen().emit_ins16("1011010Mrrrrrrrr", 0, 0);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    step(&m_context);
}
*/
