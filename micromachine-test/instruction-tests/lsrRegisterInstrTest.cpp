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

/* LSR - Register (Logical Shift Right)
   Encoding: 010000 0011 Rm:3 Rdn:3 */
MICROMACHINE_TEST_F(lsrRegister, ShiftValue1by1_CarryOutFromLowestBit, CpuTestFixture) {
	emitInstruction16("0100000011mmmddd", registers::R0, registers::R7);
	setReg(registers::R0, 1);
	setReg(registers::R7, 1);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZCv"),
	reg(registers::R7).equals(0));
}

MICROMACHINE_TEST_F(lsrRegister, ShiftValue1by0_MinimumShift_CarryUnmodified, CpuTestFixture) {
	emitInstruction16("0100000011mmmddd", registers::R0, registers::R7);
	setCarryFlag(false);
	setReg(registers::R7, 1);
	setReg(registers::R0, 0);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzcv"),
	reg(registers::R7).equals(1));
}

MICROMACHINE_TEST_F(lsrRegister, ShiftValue2by1_NoCarry, CpuTestFixture) {
	emitInstruction16("0100000011mmmddd", registers::R3, registers::R2);
	setReg(registers::R2, 2);
	setReg(registers::R3, 1);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzcv"),
	reg(registers::R2).equals(2 >> 1));
}

MICROMACHINE_TEST_F(lsrRegister, ShiftNegativeValueBy31, CpuTestFixture) {
	emitInstruction16("0100000011mmmddd", registers::R3, registers::R2);
	setReg(registers::R2, -1);
	setReg(registers::R3, 31);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzCv"),
	reg(registers::R2).equals(-1U >> 31));
}

MICROMACHINE_TEST_F(lsrRegister, ShiftNegativeValueBy32_CarryOutFromHighestBit, CpuTestFixture) {
	emitInstruction16("0100000011mmmddd", registers::R7, registers::R0);
	setReg(registers::R0, 0x80000000);
	setReg(registers::R7, 32);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZCv"),
	reg(registers::R0).equals(0));
}

MICROMACHINE_TEST_F(lsrRegister, ShiftNegativeValueBy33_ResultIsZero_CarryClear, CpuTestFixture) {
	emitInstruction16("0100000011mmmddd", registers::R3, registers::R2);
	setReg(registers::R2, -1);
	setReg(registers::R3, 33);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZcv"),
	reg(registers::R2).equals(0));
}

MICROMACHINE_TEST_F(lsrRegister, MaximumShiftOf255_ResultIsZero_CarryClear, CpuTestFixture) {
	emitInstruction16("0100000011mmmddd", registers::R3, registers::R2);
	setReg(registers::R2, -1);
	setReg(registers::R3, 255);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZcv"),
	reg(registers::R2).equals(0));
}

MICROMACHINE_TEST_F(lsrRegister, ShiftOf256_ShouldBeTreatedAs0Shift_CarryUnmodified, CpuTestFixture) {
	emitInstruction16("0100000011mmmddd", registers::R7, registers::R0);
	setCarryFlag(true);
	setReg(registers::R0, -1);
	setReg(registers::R7, 256);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("NzCv"),
	reg(registers::R0).equals(-1));
}
