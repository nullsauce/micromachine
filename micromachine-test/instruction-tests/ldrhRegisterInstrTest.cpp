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

/* LDRH - Register
   Encoding: 0101 101 Rm:3 Rn:3 Rt:3 */
MICROMACHINE_TEST_F(ldrhRegister, UseAMixOfRegistersWordAligned, CpuTestFixture) {

	emitInstruction16("0101101mmmnnnttt", registers::R7, registers::R3, registers::R0);
	setReg(registers::R3, TestMachine::INITIAL_PC);
	setReg(registers::R7, 4);
	memWrite32(TestMachine::INITIAL_PC + 4, 0xBAADFEED);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R0).equals(0xFEED));
}

MICROMACHINE_TEST_F(ldrhRegister, UseAnotherMixOfRegistersWordAligned, CpuTestFixture) {

	emitInstruction16("0101101mmmnnnttt", registers::R1, registers::R0, registers::R7);
	setReg(registers::R0, TestMachine::INITIAL_PC);
	setReg(registers::R1, 4);
	memWrite32(TestMachine::INITIAL_PC + 4, 0xBAADFEED);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R7).equals(0xFEED));
}

MICROMACHINE_TEST_F(ldrhRegister, YetAnotherMixOfRegistersNotWordAligned, CpuTestFixture) {

	emitInstruction16("0101101mmmnnnttt", registers::R0, registers::R7, registers::R4);
	setReg(registers::R7, TestMachine::INITIAL_PC);
	setReg(registers::R0, 6);
	memWrite32(TestMachine::INITIAL_PC + 4, 0xBAADFEED);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R4).equals(0xBAAD));
}

MICROMACHINE_TEST_F(ldrhRegister, AttemptUnalignedLoad, CpuTestFixture) {

	emitInstruction16("0101101mmmnnnttt", registers::R7, registers::R3, registers::R2);
	setReg(registers::R3, TestMachine::INITIAL_PC);
	setReg(registers::R7, 1);
	StepAndExpectThat(hardfaultHandlerReached());
}

MICROMACHINE_TEST_F(ldrhRegister, AttemptLoadFromInvalidAddress, CpuTestFixture) {

	emitInstruction16("0101101mmmnnnttt", registers::R7, registers::R3, registers::R2);
	setReg(registers::R3, 0xFFFFFFFC);
	setReg(registers::R7, 0);
	StepAndExpectThat(hardfaultHandlerReached());
}
