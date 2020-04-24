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

/* STRH - Immediate
   Encoding: 1000 0 Imm:5 Rn:3 Rt:3 */
MICROMACHINE_TEST_F(strhImmediate, UseAMixOfRegistersWordAlignedWithSmallestOffset, CpuTestFixture) {

	emitInstruction16("10000iiiiinnnttt", 0, registers::R7, registers::R0);
	setReg(registers::R7, TestMachine::INITIAL_PC + 4);
	memWrite32(TestMachine::INITIAL_PC + 4, 0xBAADFEED);

	StepAndExpectThatInstruction16IsExecutedAndThat(memoryAt(TestMachine::INITIAL_PC + 4).equals32(0xBAAD0000));
}

MICROMACHINE_TEST_F(strhImmediate, AnotherMixOfRegistersNotWordAligned, CpuTestFixture) {

	emitInstruction16("10000iiiiinnnttt", 1, registers::R0, registers::R7);
	setReg(registers::R0, TestMachine::INITIAL_PC + 4);
	memWrite32(TestMachine::INITIAL_PC + 4, 0xBAADFEED);

	StepAndExpectThatInstruction16IsExecutedAndThat(memoryAt(TestMachine::INITIAL_PC + 4).equals32(0x7777FEED));
}

MICROMACHINE_TEST_F(strhImmediate, LargestOffset, CpuTestFixture) {

	emitInstruction16("10000iiiiinnnttt", 31, registers::R1, registers::R6);
	setReg(registers::R1, TestMachine::INITIAL_PC);
	memWrite32(TestMachine::INITIAL_PC + 60, 0xBAADFEED);

	StepAndExpectThatInstruction16IsExecutedAndThat(memoryAt(TestMachine::INITIAL_PC + 60).equals32(0x6666FEED));
}

MICROMACHINE_TEST_F(strhImmediate, AttemptStoreToInvalidAddress, CpuTestFixture) {

	emitInstruction16("10000iiiiinnnttt", 0, registers::R3, registers::R1);
	setReg(registers::R3, 0xFFFFFFFC);
	StepAndExpectThat(hardfaultHandlerReached());
}
