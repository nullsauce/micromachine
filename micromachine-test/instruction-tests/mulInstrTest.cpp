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

/* MUL
   Encoding: 010000 1101 Rn:3 Rdm:3 */
MICROMACHINE_TEST_F(mul, UseLowestRegisterForAllArgs, CpuTestFixture) {
	emitInstruction16("0100001101nnnddd", registers::R0, registers::R0);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZcv"),
	reg(registers::R0).equals(0U));
}

MICROMACHINE_TEST_F(mul, UseHigestRegisterForAllArgs_OnlyGetLower32bitsOfResult, CpuTestFixture) {

	emitInstruction16("0100001101nnnddd", registers::R7, registers::R7);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("Nzcv"),
	reg(registers::R7).equals(0x77777777U * 0x77777777U));
}

MICROMACHINE_TEST_F(mul, UseDifferentRegistersForEachArg, CpuTestFixture) {
	emitInstruction16("0100001101nnnddd", registers::R1, registers::R2);
	setReg(registers::R1, 0xA5A5);
	setReg(registers::R2, 2);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzcv"),
	reg(registers::R2).equals(0xA5A5U << 1U));
}

MICROMACHINE_TEST_F(mul, MultiplyBy16BitMaximumValues, CpuTestFixture) {
	emitInstruction16("0100001101nnnddd", registers::R1, registers::R2);
	setReg(registers::R1, 0xFFFF);
	setReg(registers::R2, 0xFFFF);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("Nzcv"),
	reg(registers::R2).equals(0xFFFE0001));
}
