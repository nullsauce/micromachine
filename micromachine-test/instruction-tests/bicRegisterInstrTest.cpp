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

/* BIC - Register
   Encoding: 010000 1110 Rm:3 Rdn:3 */
/* NOTE: APSR_C state is maintained by this instruction. */
MICROMACHINE_TEST_F(bicRegister, UseLowestRegisterForBothArgs, CpuTestFixture) {
	emitInstruction16("0100001110mmmddd", registers::R0, registers::R0);
	// Use a couple of tests to explicitly set/clear carry to verify both states are maintained.

	// clear carry flag
	setCarryFlag(false);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZcv"), reg(registers::R0).equals(0));
}

MICROMACHINE_TEST_F(bicRegister, UseHighestRegisterForBothArgs, CpuTestFixture) {
	emitInstruction16("0100001110mmmddd", registers::R7, registers::R7);
	// set carry flag
	setCarryFlag(true);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZCv"), reg(registers::R7).equals(0));
}

MICROMACHINE_TEST_F(bicRegister, UseR3andR7, CpuTestFixture) {

	emitInstruction16("0100001110mmmddd", registers::R3, registers::R7);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzcv"), reg(registers::R7).equals(INITIAL_R7 & ~INITIAL_R3));
}

MICROMACHINE_TEST_F(bicRegister, UseBicToClearLSbit, CpuTestFixture) {
	emitInstruction16("0100001110mmmddd", registers::R6, registers::R1);
	setReg(registers::R1, -1);
	setReg(registers::R6, 1);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("Nzcv"), reg(registers::R1).equals(-1U & ~1));
}
