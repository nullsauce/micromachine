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

/* STRB - Immediate
   Encoding: 011 1 0 Imm:5 Rn:3 Rt:3 */
MICROMACHINE_TEST_F(strbImmediate, UseAMixOfRegistersWordAlignedWithSmallestOffset, CpuTestFixture) {

	emitInstruction16("01110iiiiinnnttt", 0, registers::R7, registers::R0);
	setReg(registers::R7, TestMachine::INITIAL_PC + 4);
	memWrite32(TestMachine::INITIAL_PC + 4, 0xBAADFEED);

	StepAndExpectThatInstruction16IsExecutedAndThat(memoryAt(TestMachine::INITIAL_PC + 4).equals32(0xBAADFE00));
}

MICROMACHINE_TEST_F(strbImmediate, UseAnotherMixOfRegistersSecondByteInWord, CpuTestFixture) {

	emitInstruction16("01110iiiiinnnttt", 1, registers::R0, registers::R7);
	setReg(registers::R0, TestMachine::INITIAL_PC + 4);
	memWrite32(TestMachine::INITIAL_PC + 4, 0xBAADFEED);

	StepAndExpectThatInstruction16IsExecutedAndThat(memoryAt(TestMachine::INITIAL_PC + 4).equals32(0xBAAD77ED));
}

MICROMACHINE_TEST_F(strbImmediate, YetAnotherMixOfRegistersThirdByteInWord, CpuTestFixture) {

	emitInstruction16("01110iiiiinnnttt", 2, registers::R3, registers::R4);
	setReg(registers::R3, TestMachine::INITIAL_PC + 4);
	memWrite32(TestMachine::INITIAL_PC + 4, 0xBAADFEED);

	StepAndExpectThatInstruction16IsExecutedAndThat(memoryAt(TestMachine::INITIAL_PC + 4).equals32(0xBA44FEED));
}

MICROMACHINE_TEST_F(strbImmediate, YetAnotherMixOfRegistersFourthByteInWord, CpuTestFixture) {
	emitInstruction16("01110iiiiinnnttt", 3, registers::R1, registers::R5);
	setReg(registers::R1, TestMachine::INITIAL_PC + 4);
	memWrite32(TestMachine::INITIAL_PC + 4, 0xBAADFEED);

	StepAndExpectThatInstruction16IsExecutedAndThat(memoryAt(TestMachine::INITIAL_PC + 4).equals32(0x55ADFEED));
}

MICROMACHINE_TEST_F(strbImmediate, LargestOffset, CpuTestFixture) {

	emitInstruction16("01110iiiiinnnttt", 31, registers::R2, registers::R4);
	setReg(registers::R2, TestMachine::INITIAL_PC);
	memWrite32(TestMachine::INITIAL_PC + 28, 0xBAADFEED);

	StepAndExpectThatInstruction16IsExecutedAndThat(memoryAt(TestMachine::INITIAL_PC + 28).equals32(0x44ADFEED));
}

MICROMACHINE_TEST_F(strbImmediate, AttemptStoreToInvalidAddress, CpuTestFixture) {
	emitInstruction16("01110iiiiinnnttt", 0, registers::R3, registers::R0);
	setReg(registers::R3, 0xFFFFFFFC);
	StepAndExpectThat(hardfaultHandlerReached());
}
