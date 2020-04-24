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

/* LDRH - Immediate
   Encoding: 1000 1 Imm:5 Rn:3 Rt:3 */
MICROMACHINE_TEST_F(ldrhImmediate, UseAMixOfRegistersWordAligned, CpuTestFixture) {

	emitInstruction16("10001iiiiinnnttt", 0, registers::R7, registers::R0);
	setReg(registers::R7, TestMachine::INITIAL_PC + 4);
	memWrite32(TestMachine::INITIAL_PC + 4, 0xBAADFEED);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R0).equals(0xFEED));
}

MICROMACHINE_TEST_F(ldrhImmediate, UseAnotherMixOfRegistersNotWordAligned, CpuTestFixture) {

	emitInstruction16("10001iiiiinnnttt", 1, registers::R0, registers::R7);
	setReg(registers::R0, TestMachine::INITIAL_PC + 4);
	memWrite32(TestMachine::INITIAL_PC + 4, 0xBAADFEED);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R7).equals(0xBAAD));
}

MICROMACHINE_TEST_F(ldrhImmediate, LargestOffset, CpuTestFixture) {

	emitInstruction16("10001iiiiinnnttt", 31, registers::R1, registers::R6);
	setReg(registers::R1, TestMachine::INITIAL_PC);
	memWrite32(TestMachine::INITIAL_PC + 60, 0xBAADFEED);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R6).equals(0xBAAD));
}

MICROMACHINE_TEST_F(ldrhImmediate, AttemptLoadFromInvalidAddress, CpuTestFixture) {

	emitInstruction16("10001iiiiinnnttt", 0, registers::R3, registers::R2);
	setReg(registers::R3, 0xFFFFFFFC);
	StepAndExpectThat(hardfaultHandlerReached());
}
