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

// Immediate values used for shift amount in tests.
#define IMM_0 0
#define IMM_1 1
#define IMM_3 3
#define IMM_4 4
#define IMM_31 31

/* LSL - Immediate (Logical Shift Left)
   Encoding: 000 00 imm:5 Rm:3 Rd:3 */
MICROMACHINE_TEST_F(lslImmediate, MovR7toR0_CarryUnmodified, CpuTestFixture) {
	emitInstruction16("00000iiiiimmmddd", IMM_0, registers::R7, registers::R0);

	setCarryFlag(false);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzcv"),
	reg(registers::R0).equals(0x77777777U));
}

MICROMACHINE_TEST_F(lslImmediate, MovR0toR7_ZeroResultAndCarryUnmodified, CpuTestFixture) {
	emitInstruction16("00000iiiiimmmddd", IMM_0, registers::R0, registers::R7);
	setCarryFlag(true);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZCv"),
	reg(registers::R7).equals(0x0));
}

MICROMACHINE_TEST_F(lslImmediate, ShiftR1by3_ResultInNegativeValue, CpuTestFixture) {

	emitInstruction16("00000iiiiimmmddd", IMM_3, registers::R1, registers::R0);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("Nzcv"),
	reg(registers::R0).equals(0x11111111U << 3));
}

MICROMACHINE_TEST_F(lslImmediate, ShiftR1by4_HasCarryOut, CpuTestFixture) {

	emitInstruction16("00000iiiiimmmddd", IMM_4, registers::R1, registers::R0);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R0).equals(0x11111111U << 4), apsrFlagsEquals("nzCv"));
}

MICROMACHINE_TEST_F(lslImmediate, ShiftR0by31_PushesLowestbitIntoSignBit, CpuTestFixture) {
	emitInstruction16("00000iiiiimmmddd", IMM_31, registers::R0, registers::R0);
	setReg(registers::R0, 1U);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("Nzcv"),
	reg(registers::R0).equals(1U << 31));
}

MICROMACHINE_TEST_F(lslImmediate, CarryOutFromHighestBit, CpuTestFixture) {
	emitInstruction16("00000iiiiimmmddd", IMM_1, registers::R0, registers::R0);
	setReg(registers::R0, 0xA0000000U);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzCv"),
	reg(registers::R0).equals(0xA0000000U << 1));
}

MICROMACHINE_TEST_F(lslImmediate, CarryOutFromLowestBit, CpuTestFixture) {
	emitInstruction16("00000iiiiimmmddd", IMM_31, registers::R0, registers::R0);
	setReg(registers::R0, 0x2U);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZCv"),
	reg(registers::R0).equals(0x2U << 31));
}
