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

/* CMP - Immediate
   Encoding: 001 01 Rn:3 Imm:8 */
MICROMACHINE_TEST_F(cmpImmediate, CompareLowestRegisterToEqualValue, CpuTestFixture) {
	emitInstruction16("00101nnniiiiiiii", registers::R0, 0);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZCv"));
}

MICROMACHINE_TEST_F(cmpImmediate, CompareHighestRegisterToImmediateWhichIsSmaller, CpuTestFixture) {

	emitInstruction16("00101nnniiiiiiii", registers::R7, 127);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzCv"));
}

MICROMACHINE_TEST_F(cmpImmediate, CompareRegisterToLargestImmediateWhichIsLarger, CpuTestFixture) {
	emitInstruction16("00101nnniiiiiiii", registers::R0, 255);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("Nzcv"));
}

MICROMACHINE_TEST_F(cmpImmediate, CompareRegisterToImmediateWhichWillGenerateNegativeOverflow, CpuTestFixture) {
	emitInstruction16("00101nnniiiiiiii", registers::R3, 1);
	setReg(registers::R3, 0x80000000);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzCV"));
}
