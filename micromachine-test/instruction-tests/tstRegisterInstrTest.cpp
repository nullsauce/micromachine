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

/* TST - Register
   Encoding: 010000 1000 Rm:3 Rn:3 */
/* NOTE: APSR_C state is maintained by this instruction. */
MICROMACHINE_TEST_F(tstRegister, UseLowestRegisterForBothArgsAndResultShouldBeZero, CpuTestFixture) {
	emitInstruction16("0100001000mmmnnn", registers::R0, registers::R0);
	// Use a couple of tests to explicitly set/clear carry to verify both states are maintained.
	setCarryFlag(false);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZcv"));
}

MICROMACHINE_TEST_F(tstRegister, UseHighestRegisterForBothArgsAndRegisterWillBeUnchanged, CpuTestFixture) {

	emitInstruction16("0100001000mmmnnn", registers::R7, registers::R7);
	setCarryFlag(true);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzCv"));
}

MICROMACHINE_TEST_F(tstRegister, AndR3andR7, CpuTestFixture) {

	emitInstruction16("0100001000mmmnnn", registers::R3, registers::R7);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzcv"));
}

MICROMACHINE_TEST_F(tstRegister, UseAndToJustKeepNegativeSignBit, CpuTestFixture) {

	emitInstruction16("0100001000mmmnnn", registers::R7, registers::R0);
	setReg(registers::R0, -1);
	setReg(registers::R7, 0x80000000);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("Nzcv"));
}
