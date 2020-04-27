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


/* POP
   Encoding: 1011 1 10 P:1 RegisterList:8 */
MICROMACHINE_TEST_F(pop, JustPopPC, CpuTestFixture) {
	const uint32_t INITIAL_SP = 0x00002000;
	const uint32_t INITIAL_PC = code_gen().write_address();
	code_gen().emit_ins16("1011110Prrrrrrrr", 1, 0);
	getCpu().regs().set(registers::SP, INITIAL_SP - 4);
	getCpu().mem().write32(INITIAL_SP - 4, (INITIAL_PC + 16) | 1);
	Step();
	ExpectThat().Register(registers::SP).Equals(INITIAL_SP);
	ExpectThat().Register(registers::PC).Equals(INITIAL_PC + 16);
}

MICROMACHINE_TEST_F(pop, JustPopR0, CpuTestFixture) {
	const uint32_t INITIAL_SP = 0x00002000;
	const uint32_t INITIAL_PC = code_gen().write_address();
	code_gen().emit_ins16("1011110Prrrrrrrr", 0, 1);
	getCpu().regs().set(registers::SP, INITIAL_SP - 4);
	getCpu().mem().write32(INITIAL_SP - 4, 0xFFFFFFFF);
	Step();
	ExpectThat().Register(registers::SP).Equals(INITIAL_SP);
	ExpectThat().Register(registers::R0).Equals(0xFFFFFFFF);
}

MICROMACHINE_TEST_F(pop, JustPopR7, CpuTestFixture) {
	const uint32_t INITIAL_SP = 0x00002000;
	const uint32_t INITIAL_PC = code_gen().write_address();
	code_gen().emit_ins16("1011110Prrrrrrrr", 0, (1 << 7));
	getCpu().regs().set(registers::SP, INITIAL_SP - 4);
	getCpu().mem().write32(INITIAL_SP - 4, 0xFFFFFFFF);
	Step();
	ExpectThat().Register(registers::SP).Equals(INITIAL_SP);
	ExpectThat().Register(registers::R7).Equals(0xFFFFFFFF);
}

MICROMACHINE_TEST_F(pop, PopAll, CpuTestFixture) {
	const uint32_t INITIAL_SP = 0x00002000;
	const uint32_t INITIAL_PC = code_gen().write_address();
	code_gen().emit_ins16("1011110Prrrrrrrr", 1, 0xFF);
	getCpu().regs().set(registers::SP, INITIAL_SP - 4 * 9);
	for (int i = 1; i <= 9; i++) {
		getCpu().mem().write32(INITIAL_SP - 4 * i, i);
	}
	Step();
	ExpectThat().Register(registers::SP).Equals(INITIAL_SP);
	ExpectThat().Register(registers::R0).Equals(9);
	ExpectThat().Register(registers::R1).Equals(8);
	ExpectThat().Register(registers::R2).Equals(7);
	ExpectThat().Register(registers::R3).Equals(6);
	ExpectThat().Register(registers::R4).Equals(5);
	ExpectThat().Register(registers::R5).Equals(4);
	ExpectThat().Register(registers::R6).Equals(3);
	ExpectThat().Register(registers::R7).Equals(2);
	ExpectThat().Register(registers::PC).Equals(1 & ~1);
}

MICROMACHINE_TEST_F(pop, PopToSetPCToEvenAddressWhichGeneratesHardFaultOnNextStep, CpuTestFixture) {
	const uint32_t INITIAL_SP = 0x00002000;
	const uint32_t INITIAL_PC = code_gen().write_address();
	code_gen().emit_ins16("1011110Prrrrrrrr", 1, 0);
	getCpu().regs().set(registers::SP, INITIAL_SP - 4);
	getCpu().mem().write32(INITIAL_SP - 4, INITIAL_PC + 16);

	Step();
	ExpectThat().Register(registers::SP).Equals(INITIAL_SP);
	ExpectThat().Register(registers::PC).Equals(INITIAL_PC + 16);
	ExpectThat().ThumbBitIsNotSet();


	code_gen().set_write_address(INITIAL_PC + 16);
	code_gen().emit_nop();

	// Prepare to execute a nop, but expect a hardfault instead
	Step();
	ExpectThat().HardfaultHandlerReached();
}

MICROMACHINE_TEST_F(pop, HardFaultFromInvalidMemoryRead, CpuTestFixture) {
	code_gen().emit_ins16("1011110Prrrrrrrr", 0, 1);
	getCpu().regs().set(registers::SP, 0xFFFFFFFC);

	Step();
	ExpectThat().HardfaultHandlerReached();
}

/*
TEST_SIM_ONLY(pop, UnpredictableToPopNoRegisters)
{
    code_gen().emit_ins16("1011110Prrrrrrrr", 0, 0);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    step(&m_context);
}
*/
