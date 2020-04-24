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

/* POP
   Encoding: 1011 1 10 P:1 RegisterList:8 */
MICROMACHINE_TEST_F(pop, JustPopPC, CpuTestFixture) {

	emitInstruction16("1011110Prrrrrrrr", 1, 0);
	setReg(registers::SP, TestMachine::INITIAL_SP - 4);
	memWrite32(TestMachine::INITIAL_SP - 4, (TestMachine::INITIAL_PC + 16) | 1);
	StepAndExpectThat(reg(registers::SP).equals(TestMachine::INITIAL_SP),
					  reg(registers::PC).equals(TestMachine::INITIAL_PC + 16));
}

MICROMACHINE_TEST_F(pop, JustPopR0, CpuTestFixture) {

	emitInstruction16("1011110Prrrrrrrr", 0, 1);
	setReg(registers::SP, TestMachine::INITIAL_SP - 4);
	memWrite32(TestMachine::INITIAL_SP - 4, 0xFFFFFFFF);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::SP).equals(TestMachine::INITIAL_SP), reg(registers::R0).equals(0xFFFFFFFF));
}

MICROMACHINE_TEST_F(pop, JustPopR7, CpuTestFixture) {

	emitInstruction16("1011110Prrrrrrrr", 0, (1 << 7));
	setReg(registers::SP, TestMachine::INITIAL_SP - 4);
	memWrite32(TestMachine::INITIAL_SP - 4, 0xFFFFFFFF);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::SP).equals(TestMachine::INITIAL_SP), reg(registers::R7).equals(0xFFFFFFFF));
}

MICROMACHINE_TEST_F(pop, PopAll, CpuTestFixture) {

	emitInstruction16("1011110Prrrrrrrr", 1, 0xFF);
	setReg(registers::SP, TestMachine::INITIAL_SP - 4 * 9);
	for(int i = 1; i <= 9; i++) {
		memWrite32(TestMachine::INITIAL_SP - 4 * i, i);
	}
	StepAndExpectThat(reg(registers::SP).equals(TestMachine::INITIAL_SP),
					  reg(registers::R0).equals(9),
					  reg(registers::R1).equals(8),
					  reg(registers::R2).equals(7),
					  reg(registers::R3).equals(6),
					  reg(registers::R4).equals(5),
					  reg(registers::R5).equals(4),
					  reg(registers::R6).equals(3),
					  reg(registers::R7).equals(2),
					  reg(registers::PC).equals(1 & ~1));
}

MICROMACHINE_TEST_F(pop, PopToSetPCToEvenAddressWhichGeneratesHardFaultOnNextStep, CpuTestFixture) {

	emitInstruction16("1011110Prrrrrrrr", 1, 0);
	setReg(registers::SP, TestMachine::INITIAL_SP - 4);

	memWrite32(TestMachine::INITIAL_SP - 4, TestMachine::INITIAL_PC + 16);

	StepAndExpectThat(reg(registers::SP).equals(TestMachine::INITIAL_SP),
					  reg(registers::PC).equals(TestMachine::INITIAL_PC + 16),
					  thumbFlagIsNotSet());

	setNextEmitInstructionAddress(TestMachine::INITIAL_PC + 16);
	emitInstruction16("1011111100000000"); // nop

	// Prepare to execute a nop, but expect a hardfault instead
	StepAndExpectThat(hardfaultHandlerReached());
}

MICROMACHINE_TEST_F(pop, HardFaultFromInvalidMemoryRead, CpuTestFixture) {
	/*
	 * This test is particularly interesting as it sets the stack pointer to
	 * zero before exception entry which leads the context stack frame to be saved
	 * at an unmapped address range at the very end of memory.
	 * This prevents the test framework for fully working here therefore the exception
	 * entry state is checked only partially.
	 */
	emitInstruction16("1011110Prrrrrrrr", 0, 1);
	setReg(registers::SP, 0xFFFFFFFC);

	const uint32_t hardfaultHandlerAddress = memRead32(exception::HARDFAULT * sizeof(uint32_t)) & ~1;

	StepAndExpectThat(
		  reg(registers::PC).equals(hardfaultHandlerAddress),
		  reg(registers::SP).wasDecrementedBy(28), //TODO: Check that this SP change is correct
					  exceptionStateOf(exception::HARDFAULT).isActive(),
		  executionModeIsHandler(),
		  reg(registers::LR).equals(0xFFFFFFF9),
		  IPSRStatePredicate(exception::HARDFAULT),
		  thumbFlagIsSet());
}

/*
TEST_SIM_ONLY(pop, UnpredictableToPopNoRegisters)
{
	code_gen().emit_ins16("1011110Prrrrrrrr", 0, 0));
	setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
	step(&m_context);
}
*/
