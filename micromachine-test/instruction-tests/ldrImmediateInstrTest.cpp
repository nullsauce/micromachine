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

/* LDR - Immediate Encoding T1
   Encoding: 011 0 1 Imm:5 Rn:3 Rt:3 */
MICROMACHINE_TEST_F(ldrImmediate, T1UseAMixOfRegistersWithSmallestOffset, CpuTestFixture) {

	emitInstruction16("01101iiiiinnnttt", 0, registers::R7, registers::R0);
	setReg(registers::R7, TestMachine::INITIAL_PC + 4);
	memWrite32(TestMachine::INITIAL_PC + 4, 0xBAADFEED);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R0).equals(0xBAADFEED));
}

MICROMACHINE_TEST_F(ldrImmediate, T1UseAnotherMixOfRegistersWithLargestOffset, CpuTestFixture) {

	emitInstruction16("01101iiiiinnnttt", 31, registers::R0, registers::R7);
	setReg(registers::R0, TestMachine::INITIAL_PC);
	memWrite32(TestMachine::INITIAL_PC + 31 * 4, 0xBAADFEED);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R7).equals(0xBAADFEED));
}

MICROMACHINE_TEST_F(ldrImmediate, T1AttemptUnalignedLoad, CpuTestFixture) {

	emitInstruction16("01101iiiiinnnttt", 0, registers::R3, registers::R2);
	setReg(registers::R3, TestMachine::INITIAL_PC + 2);
	StepAndExpectThat(hardfaultHandlerReached());
}

MICROMACHINE_TEST_F(ldrImmediate, T1AttemptLoadFromInvalidAddress, CpuTestFixture) {

	emitInstruction16("01101iiiiinnnttt", 16, registers::R3, registers::R2);
	setReg(registers::R3, 0xFFFFFFFC - 16 * 4);
	StepAndExpectThat(hardfaultHandlerReached());
}

/* LDR - Immediate Encoding T2 (SP is base register)
   Encoding: 1001 1 Rt:3 Imm:8 */
MICROMACHINE_TEST_F(ldrImmediate, T2UseHighestRegisterWithSmallestOffset, CpuTestFixture) {

	emitInstruction16("10011tttiiiiiiii", registers::R7, 0);
	setReg(registers::SP, TestMachine::INITIAL_PC + 1024);
	memWrite32(TestMachine::INITIAL_PC + 1024, 0xBAADFEED);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R7).equals(0xBAADFEED));
}

MICROMACHINE_TEST_F(ldrImmediate, T2UseLowestRegisterWithLargestOffset, CpuTestFixture) {

	emitInstruction16("10011tttiiiiiiii", registers::R0, 255);
	setReg(registers::SP, TestMachine::INITIAL_PC + 1024);
	memWrite32(TestMachine::INITIAL_PC + 1024 + 255 * 4, 0xBAADFEED);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R0).equals(0xBAADFEED));
}

/*
 * This tst is disabled because the implementation of SP can't store unaligned values
 * TODO: Fix this
 *
MICROMACHINE_TEST_F(ldrImmediate, T2AttemptUnalignedLoad, CpuTestFixture) {

code_gen().emit_ins16("10011tttiiiiiiii", registers::R2, 0);
	setReg(registers::SP, TestMachine::INITIAL_PC + 1026);
	memWrite32(TestMachine::INITIAL_PC + 1024, 0xBAADFEED);
	StepAndExpectThatInstruction16IsExecutedAndThat(hardfaultHandlerReached(),
	.Register(registers::R2).DidNotChange());
}

MICROMACHINE_TEST_F(ldrImmediate, T2AttemptLoadFromInvalidAddress, CpuTestFixture) {

code_gen().emit_ins16("10011tttiiiiiiii", registers::R2, 0);
	setReg(registers::R3, 0xFFFFFFFC);
	StepAndExpectThatInstruction16IsExecutedAndThat(hardfaultHandlerReached(),
	.Register(registers::R2).DidNotChange());
}
*/
