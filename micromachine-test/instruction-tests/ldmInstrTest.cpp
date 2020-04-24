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
using namespace micromachine::testing;

/* LDM
   Encoding: 1100 1 Rn:3 RegisterList:8 */
MICROMACHINE_TEST_F(ldm, JustPopR0WithR7AsAddress_WritebackNewAddressToR7, CpuTestFixture) {

	emitInstruction16("11001nnnrrrrrrrr", registers::R7, (1 << 0));
	setReg(registers::R7, TestMachine::INITIAL_PC + 16);
	memWrite32(TestMachine::INITIAL_PC + 16, 0xFFFFFFFF);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R0).equals(0xFFFFFFFF),
													reg(registers::R7).equals(TestMachine::INITIAL_PC + 16 + 1 * 4));
}

MICROMACHINE_TEST_F(ldm, JustPopR7WithR0AsAddress_WritebackNewAddressToR0, CpuTestFixture) {

	emitInstruction16("11001nnnrrrrrrrr", registers::R0, (1 << 7));
	setReg(registers::R0, TestMachine::INITIAL_PC + 16);
	memWrite32(TestMachine::INITIAL_PC + 16, 0xFFFFFFFF);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R7).equals(0xFFFFFFFF),
													reg(registers::R0).equals(TestMachine::INITIAL_PC + 16 + 1 * 4));
}

MICROMACHINE_TEST_F(ldm, PopAllNoWriteback, CpuTestFixture) {

	emitInstruction16("11001nnnrrrrrrrr", registers::R0, 0xFF);
	setReg(registers::R0, TestMachine::INITIAL_PC + 16);
	for(int i = 0; i < 8; i++) {
		memWrite32(TestMachine::INITIAL_PC + 16 + 4 * i, i);
	}
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R0).equals(0),
													reg(registers::R1).equals(1),
													reg(registers::R2).equals(2),
													reg(registers::R3).equals(3),
													reg(registers::R4).equals(4),
													reg(registers::R5).equals(5),
													reg(registers::R6).equals(6),
													reg(registers::R7).equals(7));
}

MICROMACHINE_TEST_F(ldm, PopAllButAddressRegister_WritebackNewAddress, CpuTestFixture) {

	emitInstruction16("11001nnnrrrrrrrr", registers::R7, 0x7F);
	setReg(registers::R7, TestMachine::INITIAL_PC + 16);
	for(int i = 0; i < 7; i++) {
		memWrite32(TestMachine::INITIAL_PC + 16 + 4 * i, i);
	}
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R0).equals(0),
													reg(registers::R1).equals(1),
													reg(registers::R2).equals(2),
													reg(registers::R3).equals(3),
													reg(registers::R4).equals(4),
													reg(registers::R5).equals(5),
													reg(registers::R6).equals(6),
													reg(registers::R7).equals(TestMachine::INITIAL_PC + 16 + 7 * 4));
}

MICROMACHINE_TEST_F(ldm, HardFaultFromInvalidMemoryRead, CpuTestFixture) {

	emitInstruction16("11001nnnrrrrrrrr", 0, (1 << 0));
	setReg(registers::R0, 0xFFFFFFFC);
	StepAndExpectThat(hardfaultHandlerReached());
}

/*
TEST_SIM_ONLY(ldm, UnpredictableToPopNoRegisters)
{
	code_gen().emit_ins16("11001nnnrrrrrrrr", 0, 0));
	setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
	reg(registers::PC).equals(TestMachine::INITIAL_PC),
	step(&m_context);
}
*/
