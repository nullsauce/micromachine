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

/* ADD SP Plus Immediate - Encoding T1
   Encoding: 1010 1 Rd:3 Imm:8 */
MICROMACHINE_TEST_F(addSP, T1UseHighestRegisterAddSmallestImmediate, CpuTestFixture) {

	setReg(registers::SP, TestMachine::INITIAL_SP);

	emitInstruction16("10101dddiiiiiiii", registers::R7, 0);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R7).equals(TestMachine::INITIAL_SP + 0));
}

MICROMACHINE_TEST_F(addSP, T1UseLowestRegisterAddLargestImmediate, CpuTestFixture) {

	setReg(registers::SP, TestMachine::INITIAL_SP);

	emitInstruction16("10101dddiiiiiiii", registers::R0, 255);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R0).equals(TestMachine::INITIAL_SP + 255 * 4));
}

MICROMACHINE_TEST_F(addSP, T1UseIntermediateValues, CpuTestFixture) {

	setReg(registers::SP, TestMachine::INITIAL_SP);

	emitInstruction16("10101dddiiiiiiii", registers::R3, 128);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R3).equals(TestMachine::INITIAL_SP + 128 * 4));
}

/* ADD SP Plus Immediate - Encoding T2
   Encoding: 1011 0000 0 Imm:7 */
MICROMACHINE_TEST_F(addSP, T2SmallestImmediate, CpuTestFixture) {

	setReg(registers::SP, TestMachine::TestMachine::INITIAL_PC + 1024);

	emitInstruction16("101100000iiiiiii", 0);

	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::SP).equals(TestMachine::TestMachine::INITIAL_PC + 1024 + 0));
}

MICROMACHINE_TEST_F(addSP, T2LargestImmediate, CpuTestFixture) {

	setReg(registers::SP, TestMachine::TestMachine::INITIAL_PC + 1024);

	emitInstruction16("101100000iiiiiii", 127);

	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::SP).equals(TestMachine::TestMachine::INITIAL_PC + 1024 + 127 * 4));
}

MICROMACHINE_TEST_F(addSP, T2IntermediateValues, CpuTestFixture) {

	setReg(registers::SP, TestMachine::TestMachine::INITIAL_PC + 1024);

	emitInstruction16("101100000iiiiiii", 64);

	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::SP).equals(TestMachine::TestMachine::INITIAL_PC + 1024 + 64 * 4));
}
