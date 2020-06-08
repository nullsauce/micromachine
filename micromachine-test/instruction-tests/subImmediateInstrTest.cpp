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

/* SUB - Immediate - Encoding T1
   Encoding: 000 11 1 1 Imm:3 Rn:3 Rd:3 */
MICROMACHINE_TEST_F(subImmediate, T1UseLowestRegisterOnly_SmallestImmediate, CpuTestFixture) {
	emitInstruction16("0001111iiinnnddd", 0, registers::R0, registers::R0);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZCv"),
	reg(registers::R0).equals(0U));
}

MICROMACHINE_TEST_F(subImmediate, T1UseHigestRegisterOnly_LargestImmediate, CpuTestFixture) {
	emitInstruction16("0001111iiinnnddd", 7, registers::R7, registers::R7);

	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzCv"),
	reg(registers::R7).equals(INITIAL_R7 - 7U));
}

MICROMACHINE_TEST_F(subImmediate, T1UseDifferentRegistersForEachArg, CpuTestFixture) {
	emitInstruction16("0001111iiinnnddd", 3, registers::R0, registers::R2);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("Nzcv"),
	reg(registers::R2).equals(0U - 3U));
}

MICROMACHINE_TEST_F(subImmediate, T1ForceOverflowPastLargestNegativeInteger, CpuTestFixture) {
	emitInstruction16("0001111iiinnnddd", 1, registers::R1, registers::R6);
	setReg(registers::R1, 0x80000000);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzCV"),
	reg(registers::R6).equals(0x80000000U - 1U));
}

/* SUB - Immediate - Encoding T2
   Encoding: 001 11 Rdn:3 Imm:8 */
MICROMACHINE_TEST_F(subImmediate, T2LowestRegister_SmallestImmediate, CpuTestFixture) {
	emitInstruction16("00111dddiiiiiiii", registers::R0, 0);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZCv"),
	reg(registers::R0).equals(0U));
}

MICROMACHINE_TEST_F(subImmediate, T2HigestRegister_LargestImmediate, CpuTestFixture) {
	emitInstruction16("00111dddiiiiiiii", registers::R7, 255);

	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzCv"),
	reg(registers::R7).equals(INITIAL_R7 - 255U));
}

MICROMACHINE_TEST_F(subImmediate, T2Subtract127FromR0CausesNoCarryToIndicateBorrowAndNegativeResult, CpuTestFixture) {
	emitInstruction16("00111dddiiiiiiii", registers::R0, 127);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("Nzcv"),
	reg(registers::R0).equals(0U - 127U));
}

MICROMACHINE_TEST_F(subImmediate, T2ForceOverflowPastLargestNegativeInteger, CpuTestFixture) {
	emitInstruction16("00111dddiiiiiiii", registers::R3, 1);
	setReg(registers::R3, 0x80000000);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzCV"),
	reg(registers::R3).equals(0x80000000U - 1U));
}
