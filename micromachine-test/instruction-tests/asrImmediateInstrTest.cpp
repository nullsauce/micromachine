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
using namespace micromachine::testing;

// Immediate values used for shift amount in tests.
#define IMM_1 1
#define IMM_32 0

/* ASR - Immediate (Arithmetic Shift Right)
   Encoding: 000 10 imm:5 Rm:3 Rd:3 */
MICROMACHINE_TEST_F(asrImmediate, ShiftNegativeNumberBy1_Shift0OutFromLowestBit, CpuTestFixture) {

	emitInstruction16("00010iiiiimmmddd", IMM_1, registers::R0, registers::R7);
	setReg(registers::R0, 0x80000000U);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("Nzcv"), reg(registers::R7).equals((int32_t)0x80000000U >> 1));
}

MICROMACHINE_TEST_F(asrImmediate, ShiftPositiveNumberBy1_Shift1OutFromLowestBit, CpuTestFixture) {

	emitInstruction16("00010iiiiimmmddd", IMM_1, registers::R7, registers::R0);
	setReg(registers::R7, 0x7FFFFFFFU);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzCv"), reg(registers::R0).equals((int32_t)0x7FFFFFFFU >> 1));
}

MICROMACHINE_TEST_F(asrImmediate, NegativeNumberBy32_Shift1OutFromHighestBit, CpuTestFixture) {
	emitInstruction16("00010iiiiimmmddd", IMM_32, registers::R0, registers::R0);
	setReg(registers::R0, 0x80000000U);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("NzCv"), reg(registers::R0).equals(0xFFFFFFFFU));
}

MICROMACHINE_TEST_F(asrImmediate, PositiveNumberBy32_Shift0FromHighestBit, CpuTestFixture) {

	emitInstruction16("00010iiiiimmmddd", IMM_32, registers::R1, registers::R6);
	setReg(registers::R1, 0x7FFFFFFFU);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZcv"), reg(registers::R6).equals(0x0U));
}

MICROMACHINE_TEST_F(asrImmediate, R1by1ToR7, CpuTestFixture) {

	emitInstruction16("00010iiiiimmmddd", IMM_1, registers::R1, registers::R7);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzCv"), reg(registers::R7).equals((int32_t)INITIAL_R1 >> 1));
}

MICROMACHINE_TEST_F(asrImmediate, R7by1ToR2, CpuTestFixture) {

	emitInstruction16("00010iiiiimmmddd", IMM_1, registers::R7, registers::R2);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzCv"), reg(registers::R2).equals((int32_t)INITIAL_R7 >> 1));
}

MICROMACHINE_TEST_F(asrImmediate, R0by1, CpuTestFixture) {
	emitInstruction16("00010iiiiimmmddd", IMM_1, registers::R0, registers::R0);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZcv"), reg(registers::R0).equals((int32_t)0x00000000U >> 1));
}
