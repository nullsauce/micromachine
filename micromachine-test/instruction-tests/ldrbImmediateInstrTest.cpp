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

/* LDRB - Immediate
   Encoding: 011 1 1 Imm:5 Rn:3 Rt:3 */
MICROMACHINE_TEST_F(ldrbImmediate, UseAMixOfRegistersWordAligned, CpuTestFixture) {

	emitInstruction16("01111iiiiinnnttt", 0, registers::R7, registers::R0);
	setReg(registers::R7, TestMachine::INITIAL_PC + 4);
	memWrite32(TestMachine::INITIAL_PC + 4, 0xBAADFEED);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R0).equals(0xED));
}

MICROMACHINE_TEST_F(ldrbImmediate, UseAnotherMixOfRegistersSecondByteInWord, CpuTestFixture) {

	emitInstruction16("01111iiiiinnnttt", 1, registers::R0, registers::R7);
	setReg(registers::R0, TestMachine::INITIAL_PC + 4);
	memWrite32(TestMachine::INITIAL_PC + 4, 0xBAADFEED);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R7).equals(0xFE));
}

MICROMACHINE_TEST_F(ldrbImmediate, YetAnotherMixOfRegistersThirdByteInWord, CpuTestFixture) {

	emitInstruction16("01111iiiiinnnttt", 2, registers::R1, registers::R4);
	setReg(registers::R1, TestMachine::INITIAL_PC + 4);
	memWrite32(TestMachine::INITIAL_PC + 4, 0xBAADFEED);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R4).equals(0xAD));
}

MICROMACHINE_TEST_F(ldrbImmediate, YetAnotherMixOfRegistersFourthByteInWord, CpuTestFixture) {

	emitInstruction16("01111iiiiinnnttt", 3, registers::R2, registers::R5);
	setReg(registers::R2, TestMachine::INITIAL_PC + 4);
	memWrite32(TestMachine::INITIAL_PC + 4, 0xBAADFEED);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R5).equals(0xBA));
}

MICROMACHINE_TEST_F(ldrbImmediate, UseLargestOffset, CpuTestFixture) {

	emitInstruction16("01111iiiiinnnttt", 31, registers::R3, registers::R0);
	setReg(registers::R3, TestMachine::INITIAL_PC);
	memWrite32(TestMachine::INITIAL_PC + 28, 0x12345678);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R0).equals(0x12));
}

MICROMACHINE_TEST_F(ldrbImmediate, LoadAPositiveValue, CpuTestFixture) {

	emitInstruction16("01111iiiiinnnttt", 0, registers::R3, registers::R0);
	setReg(registers::R3, TestMachine::INITIAL_PC + 4);
	memWrite32(TestMachine::INITIAL_PC + 4, 0xFFFFFF7F);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R0).equals(0x7F));
}

MICROMACHINE_TEST_F(ldrbImmediate, AttemptLoadInvalidAddress, CpuTestFixture) {

	emitInstruction16("01111iiiiinnnttt", 0, registers::R3, registers::R2);
	setReg(registers::R3, 0xFFFFFFFC);
	StepAndExpectThat(hardfaultHandlerReached());
}
