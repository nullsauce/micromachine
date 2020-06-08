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

/* RSB - Immediate
   Encoding: 010000 1001 Rn:3 Rd:3 */
MICROMACHINE_TEST_F(rsbImmediate, UseLowestRegisterOnly, CpuTestFixture) {
	emitInstruction16("0100001001nnnddd", registers::R0, registers::R0);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZCv"),
	reg(registers::R0).equals(0U));
}

MICROMACHINE_TEST_F(rsbImmediate, UseHigestRegisterOnly, CpuTestFixture) {

	emitInstruction16("0100001001nnnddd", registers::R7, registers::R7);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("Nzcv"),
	reg(registers::R7).equals(-INITIAL_R7));
}

MICROMACHINE_TEST_F(rsbImmediate, UseDifferentRegistersForEachArg, CpuTestFixture) {

	emitInstruction16("0100001001nnnddd", registers::R2, registers::R0);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("Nzcv"),
	reg(registers::R0).equals(-INITIAL_R2));
}

MICROMACHINE_TEST_F(rsbImmediate, ForceOverflowByNegatingLargestNegativeValue, CpuTestFixture) {

	emitInstruction16("0100001001nnnddd", registers::R0, registers::R7);
	setReg(registers::R0, 0x80000000);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("NzcV"),
	reg(registers::R7).equals(0x80000000U));
}
