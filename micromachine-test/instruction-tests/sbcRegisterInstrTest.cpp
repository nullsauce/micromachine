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

/* SBC - Register (SUBtract with Carry)
   Encoding: 010000 0110 Rm:3 Rdn:3 */
MICROMACHINE_TEST_F(sbcRegister, UseLowestRegisterForAllArgsAndShouldBeZeroWithCarrySetForNoBorrow, CpuTestFixture) {
	emitInstruction16("0100000110mmmddd", registers::R0, registers::R0);
	setCarryFlag(true);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZCv"),
	reg(registers::R0).equals(0));
}

MICROMACHINE_TEST_F(sbcRegister, UseHigestRegisterForAllArgsAndShouldBeZeroWithCarrySetForNoBorrow, CpuTestFixture) {

	emitInstruction16("0100000110mmmddd", registers::R7, registers::R7);
	setCarryFlag(true);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZCv"),
	reg(registers::R7).equals(0));
}

MICROMACHINE_TEST_F(sbcRegister,
					UseDifferentRegistersForEachArgAndOnlyCarryShouldBeSetToIndicateNoBorrow,
					CpuTestFixture) {


	emitInstruction16("0100000110mmmddd", registers::R1, registers::R2);
	setCarryFlag(true);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzCv"),
	reg(registers::R2).equals(INITIAL_R2 - INITIAL_R1));
}

MICROMACHINE_TEST_F(sbcRegister, ForceCarryClearToIndicateBorrowAndResultWillBeNegative, CpuTestFixture) {

	emitInstruction16("0100000110mmmddd", registers::R1, registers::R0);
	setReg(registers::R1, 1);
	setCarryFlag(true);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("Nzcv"),
	reg(registers::R0).equals(0U - 1U));
}

MICROMACHINE_TEST_F(sbcRegister, ForceNegativeOverflow, CpuTestFixture) {


	emitInstruction16("0100000110mmmddd", registers::R1, registers::R2);
	setReg(registers::R2, 0x80000000U);
	setReg(registers::R1, 1U);
	setCarryFlag(true);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzCV"),
	reg(registers::R2).equals((int32_t)0x80000000U - 1U));
}

MICROMACHINE_TEST_F(sbcRegister, ForcePositiveOverflow, CpuTestFixture) {
	emitInstruction16("0100000110mmmddd", registers::R1, registers::R2);
	setReg(registers::R2, 0x7FFFFFFFU);
	setReg(registers::R1, -1U);
	setCarryFlag(true);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("NzcV"),
	reg(registers::R2).equals(0x7FFFFFFF + 1U));
}

MICROMACHINE_TEST_F(sbcRegister, ClearCarryToCauseABorrowToOccur, CpuTestFixture) {


	emitInstruction16("0100000110mmmddd", registers::R1, registers::R2);
	setCarryFlag(false); // Causes borrow.
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzCv"),
	reg(registers::R2).equals(INITIAL_R2 - 1U - INITIAL_R1));
}
