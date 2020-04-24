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
   Encoding: 1011 0 10 M:1 RegisterList:8 */
MICROMACHINE_TEST_F(push, JustPushLR, CpuTestFixture) {

	emitInstruction16("1011010Mrrrrrrrr", 1, 0);

	memWrite32(TestMachine::INITIAL_SP - 4, 0x0);

	StepAndExpectThatInstruction16IsExecutedAndThat(
		reg(registers::SP).equals(TestMachine::INITIAL_SP - 4),
		memoryAt(TestMachine::INITIAL_SP - 4).equals32(CpuTestFixture::INITIAL_LR));
}

MICROMACHINE_TEST_F(push, JustPushR0, CpuTestFixture) {

	emitInstruction16("1011010Mrrrrrrrr", 0, 1);
	memWrite32(TestMachine::INITIAL_SP - 4, 0xFFFFFFFF);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::SP).equals(TestMachine::INITIAL_SP - 4),
													memoryAt(TestMachine::INITIAL_SP - 4).equals32(0));
}

MICROMACHINE_TEST_F(push, JustPushR7, CpuTestFixture) {

	emitInstruction16("1011010Mrrrrrrrr", 0, 1 << 7);
	memWrite32(TestMachine::INITIAL_SP - 4, 0xFFFFFFFF);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::SP).equals(TestMachine::INITIAL_SP - 4),
													memoryAt(TestMachine::INITIAL_SP - 4).equals32(0x77777777));
}

MICROMACHINE_TEST_F(push, PushAll, CpuTestFixture) {

	emitInstruction16("1011010Mrrrrrrrr", 1, 0xFF);

	StepAndExpectThatInstruction16IsExecutedAndThat(
		reg(registers::SP).wasDecrementedBy(4 * 9),
		memoryAt(TestMachine::INITIAL_SP - 4 * (9 - 0)).equals32(0x00000000U),
		memoryAt(TestMachine::INITIAL_SP - 4 * (9 - 1)).equals32(0x11111111U),
		memoryAt(TestMachine::INITIAL_SP - 4 * (9 - 2)).equals32(0x22222222U),
		memoryAt(TestMachine::INITIAL_SP - 4 * (9 - 3)).equals32(0x33333333U),
		memoryAt(TestMachine::INITIAL_SP - 4 * (9 - 4)).equals32(0x44444444U),
		memoryAt(TestMachine::INITIAL_SP - 4 * (9 - 5)).equals32(0x55555555U),
		memoryAt(TestMachine::INITIAL_SP - 4 * (9 - 6)).equals32(0x66666666U),
		memoryAt(TestMachine::INITIAL_SP - 4 * (9 - 7)).equals32(0x77777777U),
		memoryAt(TestMachine::INITIAL_SP - 4 * (9 - 8)).equals32(CpuTestFixture::INITIAL_LR)
	);
}

MICROMACHINE_TEST_F(push, HardFaultFromInvalidMemoryWrite, CpuTestFixture) {
	/*
	 * This test is particularly interesting as it sets the stack pointer to
	 * zero before exception entry which leads the context stack frame to be saved
	 * at an unmapped address range at the very end of memory.
	 * This prevents the test framework for fully working here therefore the exception
	 * entry state is checked only partially.
	 */
	emitInstruction16("1011010Mrrrrrrrr", 0, 1);
	setReg(registers::SP, 0xFFFFFFFC);

	const uint32_t hardfaultHandlerAddress = memRead32(exception::HARDFAULT * sizeof(uint32_t)) & ~1;
	StepAndExpectThat(reg(registers::PC).equals(hardfaultHandlerAddress),
					  reg(registers::SP).wasDecrementedBy(4 * 9),
					  exceptionStateOf(exception::HARDFAULT).isActive(),
					  executionModeIsHandler(),
					  reg(registers::LR).equals(0xFFFFFFF9),
					  IPSRStatePredicate(exception::HARDFAULT),
					  thumbFlagIsSet());
}

/*
TEST_SIM_ONLY(push, UnpredictableToPushNoRegisters)
{
	code_gen().emit_ins16("1011010Mrrrrrrrr", 0, 0));
	setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
	step(&m_context);
}
*/
