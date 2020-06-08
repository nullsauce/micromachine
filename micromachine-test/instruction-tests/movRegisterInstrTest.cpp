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

/* MOV - Register Encoding 1
   Encoding: 010001 10 D:1 Rm:4 Rd:3
   NOTE: This encoding doesn't update the APSR flags. */
MICROMACHINE_TEST_F(movRegister, UseLowestRegisterForAllArgs, CpuTestFixture) {
	emitInstruction16("01000110dmmmmddd", registers::R0, registers::R0);
	StepAndExpectThatInstruction16IsExecutedAndThat();
}

MICROMACHINE_TEST_F(movRegister, UseHighRegisterForAllArgs, CpuTestFixture) {
	emitInstruction16("01000110dmmmmddd", registers::LR, registers::LR);
	StepAndExpectThatInstruction16IsExecutedAndThat();
}

MICROMACHINE_TEST_F(movRegister, MoveHighRegisterToLowRegister, CpuTestFixture) {
	emitInstruction16("01000110dmmmmddd", registers::R7, registers::R12);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R7).equals(INITIAL_R12));
}

MICROMACHINE_TEST_F(movRegister, MoveLowRegisterToLHighRegister, CpuTestFixture) {
	emitInstruction16("01000110dmmmmddd", registers::R12, registers::R7);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R12).equals(INITIAL_R7));
}

MICROMACHINE_TEST_F(movRegister, MoveOddAddressIntoPCAndMakeSureLSbitIsCleared, CpuTestFixture) {
	emitInstruction16("01000110dmmmmddd", registers::PC, registers::R1);
	setReg(registers::R1, TestMachine::TestMachine::INITIAL_PC + 1025);
	StepAndExpectThat(reg(registers::PC).equals(TestMachine::TestMachine::INITIAL_PC + 1024));
}

MICROMACHINE_TEST_F(movRegister, MoveEvenAddressIntoPC, CpuTestFixture) {
	setReg(registers::R2, TestMachine::TestMachine::INITIAL_PC + 1024);
	emitInstruction16("01000110dmmmmddd", registers::PC, registers::R2);
	StepAndExpectThat(reg(registers::PC).equals(TestMachine::TestMachine::INITIAL_PC + 1024));
}

MICROMACHINE_TEST_F(movRegister, MovePCintoOtherRegister, CpuTestFixture) {
	emitInstruction16("01000110dmmmmddd", registers::R3, registers::PC);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R3).equals(TestMachine::TestMachine::INITIAL_PC + 4));
}
