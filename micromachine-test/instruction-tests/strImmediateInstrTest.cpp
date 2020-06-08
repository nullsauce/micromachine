/*  Copyright (C) 2014  Adam Green (https://github.com/adamgreen): Original implementation
    Copyright (C) 2019  Flavio Roth (https://github.com/flavioroth): Adaptation

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

/* STR - Immediate Encoding T1
   Encoding: 011 0 0 Imm:5 Rn:3 Rt:3 */
MICROMACHINE_TEST_F(strImmediate, T1UseAMixOfRegistersWithSmallestImmediateOffset, CpuTestFixture) {

	emitInstruction16("01100iiiiinnnttt", 0, registers::R7, registers::R0);
	setReg(registers::R7, TestMachine::INITIAL_PC + 4);
	memWrite32(TestMachine::INITIAL_PC + 4, 0xBAADFEED);
	StepAndExpectThatInstruction16IsExecutedAndThat(memoryAt(TestMachine::INITIAL_PC + 4).equals32(0));
}

MICROMACHINE_TEST_F(strImmediate, T1UseAnotherMixOfRegistersWithLargestImmediateOffset, CpuTestFixture) {

	emitInstruction16("01100iiiiinnnttt", 31, registers::R0, registers::R7);
	setReg(registers::R0, TestMachine::INITIAL_PC);
	memWrite32(TestMachine::INITIAL_PC + 31 * 4, 0xBAADFEED);
	StepAndExpectThatInstruction16IsExecutedAndThat(memoryAt(TestMachine::INITIAL_PC + 31 * 4).equals32(INITIAL_R7));
}

MICROMACHINE_TEST_F(strImmediate, T1AttemptUnalignedStore, CpuTestFixture) {

	emitInstruction16("01100iiiiinnnttt", 0, registers::R3, registers::R2);
	setReg(registers::R3, TestMachine::INITIAL_PC + 2);
	StepAndExpectThat(hardfaultHandlerReached());
}

MICROMACHINE_TEST_F(strImmediate, T1AttemptStoreToInvalidAddress, CpuTestFixture) {

	emitInstruction16("01100iiiiinnnttt", 16, registers::R3, registers::R2);
	setReg(registers::R3, 0xFFFFFFFC - 16 * 4);
	StepAndExpectThat(hardfaultHandlerReached());
}

/* STR - Immediate Encoding T2 (registers::SP is base register)
   Encoding: 1001 0 Rt:3 Imm:8 */
MICROMACHINE_TEST_F(strImmediate, T2HighestRegisterWithSmallestImmediateOffset, CpuTestFixture) {

	emitInstruction16("10010tttiiiiiiii", registers::R7, 0);
	setReg(registers::SP, TestMachine::INITIAL_PC + 1024);
	memWrite32(TestMachine::INITIAL_PC + 1024, 0xBAADFEED);
	StepAndExpectThatInstruction16IsExecutedAndThat(memoryAt(TestMachine::INITIAL_PC + 1024).equals32(INITIAL_R7));
}

MICROMACHINE_TEST_F(strImmediate, T2LowestRegisterWithLargestImmediateOffset, CpuTestFixture) {

	emitInstruction16("10010tttiiiiiiii", registers::R0, 255);
	setReg(registers::SP, TestMachine::INITIAL_PC + 1024);
	memWrite32(TestMachine::INITIAL_PC + 1024 + 255 * 4, 0xBAADFEED);

	StepAndExpectThatInstruction16IsExecutedAndThat(memoryAt(TestMachine::INITIAL_PC + 1024 + 255 * 4).equals32(0));
}

/*
 * This test is disabled because the current software architecture prevents sp
 * from holding an unaligned address. However, the doc clearly states that
 * unaligned STR instructions should raise a hard fault.
 *
 * TODO: Remove the pedantic forced alignment of the value returned by sp()
 * and allow the error to happen. This might have side effects on other parts
 * of the code that wrongly expect that sp wont hold an unaligned value.
 *
MICROMACHINE_TEST_F(strImmediate, T2AttemptUnalignedStore, CpuTestFixture) {

code_gen().emit_ins16("10010tttiiiiiiii", registers::R2, 0);
	setReg(registers::SP, TestMachine::INITIAL_PC + 1026);
	memWrite32(TestMachine::INITIAL_PC + 1024, 0xBAADFEED);
	StepAndExpectThat(hardfaultHandlerReached());*/
/*
TEST_SIM_ONLY(strImmediate, T2AttemptStoreToInvalidAddress)
{
	code_gen().emit_ins16("10010tttiiiiiiii", registers::R2, 0));
	setReg(registers::SP, 0xFFFFFFFC);
	setExpectedExceptionHandled(CPU_STEP_HARDFAULT);
	step(&m_context);
}
*/
