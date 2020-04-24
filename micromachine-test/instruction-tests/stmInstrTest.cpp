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

/* PUSH
   Encoding: 1100 0 Rn:3 RegisterList:8 */
MICROMACHINE_TEST_F(stm, JustPushR0WithR7AsAddress, CpuTestFixture) {

	emitInstruction16("11000nnnrrrrrrrr", registers::R7, (1 << 0));
	setReg(registers::R7, TestMachine::INITIAL_PC + 16);
	memWrite32(TestMachine::INITIAL_PC + 16, 0xFFFFFFFF);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R7).equals(TestMachine::INITIAL_PC + 16 + 1 * 4),
													memoryAt(TestMachine::INITIAL_PC + 16).equals32(0));
}

MICROMACHINE_TEST_F(stm, JustPushR7WithR0AsAddress, CpuTestFixture) {

	emitInstruction16("11000nnnrrrrrrrr", registers::R0, (1 << 7));
	setReg(registers::R0, TestMachine::INITIAL_PC + 16);
	memWrite32(TestMachine::INITIAL_PC + 16, 0xFFFFFFFF);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R0).equals(TestMachine::INITIAL_PC + 16 + 1 * 4),
													memoryAt(TestMachine::INITIAL_PC + 16).equals32(0x77777777));
}

MICROMACHINE_TEST_F(stm, PushAllWithR0AsAddress, CpuTestFixture) {

	emitInstruction16("11000nnnrrrrrrrr", registers::R0, 0xFF);
	setReg(registers::R0, TestMachine::INITIAL_PC + 16);

	StepAndExpectThatInstruction16IsExecutedAndThat(
		reg(registers::R0).equals(TestMachine::INITIAL_PC + 16 + 8 * 4),
		memoryAt(TestMachine::INITIAL_PC + 16 + 4 * 0).equals32(TestMachine::INITIAL_PC + 16),
		memoryAt(TestMachine::INITIAL_PC + 16 + 4 * 1).equals32(0x11111111U),
		memoryAt(TestMachine::INITIAL_PC + 16 + 4 * 2).equals32(0x22222222U),
		memoryAt(TestMachine::INITIAL_PC + 16 + 4 * 3).equals32(0x33333333U),
		memoryAt(TestMachine::INITIAL_PC + 16 + 4 * 4).equals32(0x44444444U),
		memoryAt(TestMachine::INITIAL_PC + 16 + 4 * 5).equals32(0x55555555U),
		memoryAt(TestMachine::INITIAL_PC + 16 + 4 * 6).equals32(0x66666666U),
		memoryAt(TestMachine::INITIAL_PC + 16 + 4 * 7).equals32(0x77777777U));
}

MICROMACHINE_TEST_F(stm, PushAllButR7WithR7AsAddress, CpuTestFixture) {

	emitInstruction16("11000nnnrrrrrrrr", registers::R7, 0x7F);
	setReg(registers::R7, TestMachine::INITIAL_PC + 16);

	for(int i = 0; i < 7; i++) {
		setReg(i, 0x11111111U * i);
	}

	for(int i = 0; i < 7; i++) {
		memWrite32(TestMachine::INITIAL_PC + 16 + 4 * i, 0xFFFFFFFF);
	}
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R7).equals(TestMachine::INITIAL_PC + 16 + 7 * 4),
		memoryAt(TestMachine::INITIAL_PC + 16 + 4 * 0).equals32(0x00000000U),
		memoryAt(TestMachine::INITIAL_PC + 16 + 4 * 1).equals32(0x11111111U),
		memoryAt(TestMachine::INITIAL_PC + 16 + 4 * 2).equals32(0x22222222U),
		memoryAt(TestMachine::INITIAL_PC + 16 + 4 * 3).equals32(0x33333333U),
		memoryAt(TestMachine::INITIAL_PC + 16 + 4 * 4).equals32(0x44444444U),
		memoryAt(TestMachine::INITIAL_PC + 16 + 4 * 5).equals32(0x55555555U),
		memoryAt(TestMachine::INITIAL_PC + 16 + 4 * 6).equals32(0x66666666U));
}


MICROMACHINE_TEST_F(stm, HardFaultFromInvalidMemoryWrite, CpuTestFixture) {

	emitInstruction16("11000nnnrrrrrrrr", registers::R0, 1 << 0);
	setReg(registers::R0, 0xFFFFFFFC);

	StepAndExpectThat(hardfaultHandlerReached());
}

/*
TEST_SIM_ONLY(stm, UnpredictableToPushNoRegisters)
{
	code_gen().emit_ins16("11000nnnrrrrrrrr", registers::R0, 0));
	setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
	reg(registers::PC).equals(TestMachine::INITIAL_PC),
	pinkySimStep(&m_context);
}

TEST_SIM_ONLY(stm, UnpredictableToPushWritebackRegisterWhichIsntFirstSaved)
{
	code_gen().emit_ins16("11000nnnrrrrrrrr", registers::R7, 0xFF);
	setReg(registers::R7, TestMachine::INITIAL_PC + 16);
	setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
	reg(registers::PC).equals(TestMachine::INITIAL_PC),
	step(&m_context);
}
*/
