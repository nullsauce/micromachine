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


/* LDM
   Encoding: 1100 1 Rn:3 RegisterList:8 */
MICROMACHINE_TEST_F(ldm, JustPopR0WithR7AsAddress_WritebackNewAddressToR7, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	code_gen().emit_ins16("11001nnnrrrrrrrr", registers::R7, (1 << 0));
	getCpu().regs().set(registers::R7, INITIAL_PC + 16);
	getCpu().mem().write32(INITIAL_PC + 16, 0xFFFFFFFF);
	Step();
	ExpectThat().Register(registers::R0).Equals(0xFFFFFFFF);
	ExpectThat().Register(registers::R7).Equals(INITIAL_PC + 16 + 1 * 4);
}

MICROMACHINE_TEST_F(ldm, JustPopR7WithR0AsAddress_WritebackNewAddressToR0, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	code_gen().emit_ins16("11001nnnrrrrrrrr", registers::R0, (1 << 7));
	getCpu().regs().set(registers::R0, INITIAL_PC + 16);
	getCpu().mem().write32(INITIAL_PC + 16, 0xFFFFFFFF);
	Step();
	ExpectThat().Register(registers::R7).Equals(0xFFFFFFFF);
	ExpectThat().Register(registers::R0).Equals(INITIAL_PC + 16 + 1 * 4);
}

MICROMACHINE_TEST_F(ldm, PopAllNoWriteback, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	code_gen().emit_ins16("11001nnnrrrrrrrr", registers::R0, 0xFF);
	getCpu().regs().set(registers::R0, INITIAL_PC + 16);
	for (int i = 0; i < 8; i++) {
		getCpu().mem().write32(INITIAL_PC + 16 + 4 * i, i);
	}
	Step();
	ExpectThat().Register(registers::R0).Equals(0);
	ExpectThat().Register(registers::R1).Equals(1);
	ExpectThat().Register(registers::R2).Equals(2);
	ExpectThat().Register(registers::R3).Equals(3);
	ExpectThat().Register(registers::R4).Equals(4);
	ExpectThat().Register(registers::R5).Equals(5);
	ExpectThat().Register(registers::R6).Equals(6);
	ExpectThat().Register(registers::R7).Equals(7);
}

MICROMACHINE_TEST_F(ldm, PopAllButAddressRegister_WritebackNewAddress, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	code_gen().emit_ins16("11001nnnrrrrrrrr", registers::R7, 0x7F);
	getCpu().regs().set(registers::R7, INITIAL_PC + 16);
	for (int i = 0; i < 7; i++) {
		getCpu().mem().write32(INITIAL_PC + 16 + 4 * i, i);
	}
	Step();
	ExpectThat().Register(registers::R0).Equals(0);
	ExpectThat().Register(registers::R1).Equals(1);
	ExpectThat().Register(registers::R2).Equals(2);
	ExpectThat().Register(registers::R3).Equals(3);
	ExpectThat().Register(registers::R4).Equals(4);
	ExpectThat().Register(registers::R5).Equals(5);
	ExpectThat().Register(registers::R6).Equals(6);
	ExpectThat().Register(registers::R7).Equals(INITIAL_PC + 16 + 7 * 4);
}

MICROMACHINE_TEST_F(ldm, HardFaultFromInvalidMemoryRead, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	code_gen().emit_ins16("11001nnnrrrrrrrr", 0, (1 << 0));
	getCpu().regs().set(registers::R0, 0xFFFFFFFC);
	Step();
	ExpectThat().HardfaultHandlerReached();
}
/*
TEST_SIM_ONLY(ldm, UnpredictableToPopNoRegisters)
{
    code_gen().emit_ins16("11001nnnrrrrrrrr", 0, 0);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    ExpectThat().Register(registers::PC).Equals(INITIAL_PC);
    step(&m_context);
}
*/
