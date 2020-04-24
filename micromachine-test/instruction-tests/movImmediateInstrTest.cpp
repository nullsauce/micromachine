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

/* MOV - Immediate
   Encoding: 001 00 Rd:3 Imm:8 */
/* NOTE: APSR_C state is maintained by this instruction. */
MICROMACHINE_TEST_F(movImmediate, MovToR0, CpuTestFixture) {
	emitInstruction16("00100dddiiiiiiii", registers::R0, 127);
	// Use a couple of tests to explicitly set/clear carry to verify both states are maintained.
	setCarryFlag(false);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R0).equals(127), apsrFlagsEquals("nzcv"));
}

MICROMACHINE_TEST_F(movImmediate, MovToR7, CpuTestFixture) {
	emitInstruction16("00100dddiiiiiiii", registers::R7, 127);
	setCarryFlag(true);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzCv"), reg(registers::R7).equals(127));
}

MICROMACHINE_TEST_F(movImmediate, MovSmallestImmediateValueToR3, CpuTestFixture) {
	emitInstruction16("00100dddiiiiiiii", registers::R3, 0);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZcv"), reg(registers::R3).equals(0));
}

MICROMACHINE_TEST_F(movImmediate, MovLargestImmediateValueToR3, CpuTestFixture) {
	emitInstruction16("00100dddiiiiiiii", registers::R3, 255);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzcv"), reg(registers::R3).equals(255));
}
