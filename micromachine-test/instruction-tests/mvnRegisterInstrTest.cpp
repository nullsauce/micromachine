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

/* MVN - Register (MOve Negative)
   Encoding: 010000 1111 Rm:3 Rd:3 */
/* NOTE: APSR_C state is maintained by this instruction. */
MICROMACHINE_TEST_F(mvnRegister, UseLowestRegisterForAllArgs, CpuTestFixture) {
	// Use a couple of tests to explicitly set/clear carry to verify both states are maintained.
	emitInstruction16("0100001111mmmddd", registers::R0, registers::R0);
	setCarryFlag(true);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("NzCv"),
	reg(registers::R0).equals(~0U));
}

MICROMACHINE_TEST_F(mvnRegister, UseHigestRegisterForAllArgs, CpuTestFixture) {

	emitInstruction16("0100001111mmmddd", registers::R7, registers::R7);
	setCarryFlag(false);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("Nzcv"),
	reg(registers::R7).equals(~INITIAL_R7));
}

MICROMACHINE_TEST_F(mvnRegister, UseDifferentRegistersForEachArg, CpuTestFixture) {


	emitInstruction16("0100001111mmmddd", registers::R2, registers::R1);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("Nzcv"),
	reg(registers::R1).equals(~INITIAL_R2));
}

MICROMACHINE_TEST_F(mvnRegister, MoveANegationOfNegativeOne_ClearsNegativeFlagAndSetsZeroFlag, CpuTestFixture) {
	emitInstruction16("0100001111mmmddd", registers::R2, registers::R1);
	setReg(registers::R2, -1);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZcv"),
	reg(registers::R1).equals(0U));
}
