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

/* CMN - Register (Compare Negative)
   Encoding: 010000 1011 Rm:3 Rn:3 */
MICROMACHINE_TEST_F(cmnRegister, UseLowestRegisterForAllArgs, CpuTestFixture) {
	emitInstruction16("0100001011mmmnnn", registers::R0, registers::R0);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZcv"));
}

MICROMACHINE_TEST_F(cmnRegister, UseHigestRegisterForAllArgs, CpuTestFixture) {

	emitInstruction16("0100001011mmmnnn", registers::R7, registers::R7);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("NzcV"));
}

MICROMACHINE_TEST_F(cmnRegister, UseDifferentRegistersForEachArg, CpuTestFixture) {

	emitInstruction16("0100001011mmmnnn", registers::R1, registers::R2);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzcv"));
}

// Force APSR flags to be set which haven't already been covered above.
MICROMACHINE_TEST_F(cmnRegister, ForceCarryWithNoOverflow, CpuTestFixture) {
	emitInstruction16("0100001011mmmnnn", registers::R1, registers::R2);
	setReg(registers::R1, -1);
	setReg(registers::R2, 1);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZCv"));
}

MICROMACHINE_TEST_F(cmnRegister, ForceCarryAndOverflow, CpuTestFixture) {
	emitInstruction16("0100001011mmmnnn", registers::R1, registers::R2);
	setReg(registers::R1, -1);
	setReg(registers::R2, 0x80000000U);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzCV"));
}
