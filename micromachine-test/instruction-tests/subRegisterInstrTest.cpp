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

/* SUB - Register
   Encoding: 000 11 0 1 Rm:3 Rn:3 Rd:3 */
MICROMACHINE_TEST_F(subRegister, UseLowestRegisterForAllArgs, CpuTestFixture) {
	emitInstruction16("0001101mmmnnnddd", registers::R0, registers::R0, registers::R0);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZCv"),
	reg(registers::R0).equals(0));
}

MICROMACHINE_TEST_F(subRegister, UseHigestRegisterForAllArgs, CpuTestFixture) {

	emitInstruction16("0001101mmmnnnddd", registers::R7, registers::R7, registers::R7);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZCv"),
	reg(registers::R7).equals(0));
}

MICROMACHINE_TEST_F(subRegister, UseDifferentRegistersForEachArg, CpuTestFixture) {


	emitInstruction16("0001101mmmnnnddd", registers::R1, registers::R2, registers::R0);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzCv"),
	reg(registers::R0).equals(0x22222222U - 0x11111111U));
}

// Force APSR flags to be set which haven't already been covered above.
MICROMACHINE_TEST_F(subRegister, ForceCarryClearToIndicateBorrowAndResultWillBeNegative, CpuTestFixture) {
	emitInstruction16("0001101mmmnnnddd", registers::R1, registers::R0, registers::R2);
	setReg(registers::R1, 1);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("Nzcv"),
	reg(registers::R2).equals(0U - 1U));
}

MICROMACHINE_TEST_F(subRegister, ForceNegativeOverflow, CpuTestFixture) {
	emitInstruction16("0001101mmmnnnddd", registers::R1, registers::R2, registers::R0);
	setReg(registers::R2, 0x80000000U);
	setReg(registers::R1, 1U);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzCV"),
	reg(registers::R0).equals(0x7FFFFFFF));
}

MICROMACHINE_TEST_F(subRegister, ForcePositiveOverflow, CpuTestFixture) {
	emitInstruction16("0001101mmmnnnddd", registers::R1, registers::R2, registers::R0);
	setReg(registers::R2, 0x7FFFFFFFU);
	setReg(registers::R1, -1U);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("NzcV"),
	reg(registers::R0).equals(0x7FFFFFFFU + 1U));
}
