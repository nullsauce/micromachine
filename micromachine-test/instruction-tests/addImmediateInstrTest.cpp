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

/* ADD - Immediate - Encoding T1
   Encoding: 000 11 1 0 Imm:3 Rn:3 Rd:3 */
MICROMACHINE_TEST_F(addImmediate, T1UseLowestRegisterOnlyAddLargestImmediate, CpuTestFixture) {
	emitInstruction16("0001110iiinnnddd", 7, registers::R0, registers::R0);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzcv"), reg(registers::R0).equals(0U + 7U));
}

MICROMACHINE_TEST_F(addImmediate, T1UseHigestRegisterOnlyAddSmallestImmediate, CpuTestFixture) {
	emitInstruction16("0001110iiinnnddd", 0, registers::R7, registers::R7);

	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzcv"), reg(registers::R7).equals(0x77777777U + 0U));
}

MICROMACHINE_TEST_F(addImmediate, T1UseDifferentRegistersForEachArg, CpuTestFixture) {
	emitInstruction16("0001110iiinnnddd", 3, registers::R7, registers::R0);

	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzcv"), reg(registers::R0).equals(0x77777777U + 3U));
}

MICROMACHINE_TEST_F(addImmediate, T1ForceCarryByAdding1ToLargestInteger, CpuTestFixture) {
	emitInstruction16("0001110iiinnnddd", 1, registers::R6, registers::R1);

	setReg(registers::R6, 0xFFFFFFFFU);

	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZCv"), reg(registers::R1).equals(0));
}

MICROMACHINE_TEST_F(addImmediate, T1ForceOverflowPastLargestPositiveInteger, CpuTestFixture) {
	emitInstruction16("0001110iiinnnddd", 1, registers::R2, registers::R5);

	setReg(registers::R2, 0x7FFFFFFFU);

	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("NzcV"), reg(registers::R5).equals(0x7FFFFFFFU + 1U));
}

/* ADD - Immediate - Encoding T2
   Encoding: 001 10 Rdn:3 Imm:8 */
MICROMACHINE_TEST_F(addImmediate, T2UseLowestRegisterAndAddLargestImmediate, CpuTestFixture) {
	emitInstruction16("00110dddiiiiiiii", registers::R0, 255);

	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzcv"), reg(registers::R0).equals(0U + 255U));
}

MICROMACHINE_TEST_F(addImmediate, T2UseHigestRegisterAndAddSmallestImmediate, CpuTestFixture) {
	emitInstruction16("00110dddiiiiiiii", registers::R7, 0);

	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzcv"), reg(registers::R7).equals(0x77777777U + 0U));
}

MICROMACHINE_TEST_F(addImmediate, T2ForceCarryByAdding1ToLargestInteger, CpuTestFixture) {
	emitInstruction16("00110dddiiiiiiii", registers::R3, 1);

	setReg(registers::R3, 0xFFFFFFFFU);

	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZCv"), reg(registers::R3).equals(0));
}

MICROMACHINE_TEST_F(addImmediate, T2ForceOverflowPastLargestPositiveInteger, CpuTestFixture) {
	emitInstruction16("00110dddiiiiiiii", registers::R3, 1);

	setReg(registers::R3, 0x7FFFFFFFU);

	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("NzcV"), reg(registers::R3).equals(0x7FFFFFFFU + 1));
}
