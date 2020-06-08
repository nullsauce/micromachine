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

// Immediate values used for shift amount in tests.
#define IMM_1 1
#define IMM_32 0

/* LSR - Immediate (Logical Shift Right)
   Encoding: 000 01 imm:5 Rm:3 Rd:3 */
MICROMACHINE_TEST_F(lsr, ImmediateR2by1toR0, CpuTestFixture) {

	emitInstruction16("00001iiiiimmmddd", IMM_1, registers::R2, registers::R0);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzcv"), reg(registers::R0).equals(INITIAL_R2 >> 1));
}

MICROMACHINE_TEST_F(lsr, ImmediateR7by32toR0_ZeroResult, CpuTestFixture) {
	emitInstruction16("00001iiiiimmmddd", IMM_32, registers::R7, registers::R0);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZcv"), reg(registers::R0).equals(0x0));
}

MICROMACHINE_TEST_F(lsr, ImmediateR1by1toR7_CarryOut, CpuTestFixture) {

	emitInstruction16("00001iiiiimmmddd", IMM_1, registers::R1, registers::R7);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzCv"), reg(registers::R7).equals(INITIAL_R1 >> 1));
}

MICROMACHINE_TEST_F(lsr, ImmediateR0by32_CarryOutAndIsZero, CpuTestFixture) {
	emitInstruction16("00001iiiiimmmddd", IMM_32, registers::R0, registers::R0);
	setReg(registers::R0, 0x80000000U);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZCv"), reg(registers::R0).equals(0U));
}

// Can't generate a negative result as smallest shift is 1, meaning at least one 0 is shifted in from left.
