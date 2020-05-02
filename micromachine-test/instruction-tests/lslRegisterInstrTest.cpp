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

/* LSL - Register (Logical Shift Left)
   Encoding: 010000 0010 Rm:3 Rdn:3 */
MICROMACHINE_TEST_F(lslRegister, ShiftR7by0_MinimumShift_CarryShouldBeUnmodified, CpuTestFixture) {

	emitInstruction16("0100000010mmmddd", registers::R0, registers::R7);
	setCarryFlag(true);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzCv"), reg(registers::R7).equals(INITIAL_R7));
}

MICROMACHINE_TEST_F(lslRegister, ShiftValue1by31_NegativeResult, CpuTestFixture) {
	emitInstruction16("0100000010mmmddd", registers::R4, registers::R3);
	setReg(registers::R3, 1);
	setReg(registers::R4, 31);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("Nzcv"), reg(registers::R3).equals(1 << 31));
}

MICROMACHINE_TEST_F(lslRegister, ShiftValue1by32_CarryOutFromLowestBit, CpuTestFixture) {
	emitInstruction16("0100000010mmmddd", registers::R7, registers::R0);
	setReg(registers::R0, 1);
	setReg(registers::R7, 32);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZCv"), reg(registers::R0).equals(0));
}

MICROMACHINE_TEST_F(lslRegister, ShiftNegativeValueBy1_CarryOutFromHighestBit, CpuTestFixture) {
	emitInstruction16("0100000010mmmddd", registers::R3, registers::R4);
	setReg(registers::R4, -1);
	setReg(registers::R3, 1);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("NzCv"), reg(registers::R4).equals(0xffffffff << 1));
}

MICROMACHINE_TEST_F(lslRegister, ShiftValue1by33_NoCarry, CpuTestFixture) {
	emitInstruction16("0100000010mmmddd", registers::R7, registers::R0);
	setReg(registers::R0, 1);
	setReg(registers::R7, 33);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZcv"), reg(registers::R0).equals(0));
}

MICROMACHINE_TEST_F(lslRegister, ShiftValuee1by255_MaximumShift, CpuTestFixture) {
	emitInstruction16("0100000010mmmddd", registers::R7, registers::R0);
	setReg(registers::R0, 1);
	setReg(registers::R7, 255);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZcv"), reg(registers::R0).equals(0));
}

MICROMACHINE_TEST_F(lslRegister, ShiftValue1by256_ShouldBeTreatedAs0Shift_CarryUnmodified, CpuTestFixture) {
	emitInstruction16("0100000010mmmddd", registers::R7, registers::R0);
	setCarryFlag(false);
	setReg(registers::R0, 1);
	setReg(registers::R7, 256);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzcv"), reg(registers::R0).equals(1));
}
