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

/* ROR - Register (ROtate Right)
   Encoding: 010000 0111 Rm:3 Rdn:3 */
MICROMACHINE_TEST_F(rorRegister, Rotate1by1_CarryOutFromLowestBit, CpuTestFixture) {
	emitInstruction16("0100000111mmmddd", registers::R0, registers::R7);
	setReg(registers::R0, 1);
	setReg(registers::R7, 1);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("NzCv"), reg(registers::R7).equals(0x80000000));
}

MICROMACHINE_TEST_F(rorRegister, Rotate1by0_MinimumShift_CarryUnmodified, CpuTestFixture) {
	emitInstruction16("0100000111mmmddd", registers::R7, registers::R0);
	setReg(registers::R0, 1);
	setReg(registers::R7, 0);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzcv"), reg(registers::R0).equals(1));
}

MICROMACHINE_TEST_F(rorRegister, Rotate2by1_NoCarry, CpuTestFixture) {
	emitInstruction16("0100000111mmmddd", registers::R3, registers::R2);
	setReg(registers::R2, 2);
	setReg(registers::R3, 1);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzcv"), reg(registers::R2).equals(2 >> 1));
}

MICROMACHINE_TEST_F(rorRegister, Rotate16Bits, CpuTestFixture) {
	emitInstruction16("0100000111mmmddd", registers::R3, registers::R2);
	setReg(registers::R2, 0x12345678);
	setReg(registers::R3, 16);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzcv"), reg(registers::R2).equals(0x56781234));
}

MICROMACHINE_TEST_F(rorRegister, RotateWithShiftOf31, CpuTestFixture) {
	emitInstruction16("0100000111mmmddd", registers::R3, registers::R2);
	setReg(registers::R2, 0x80000000);
	setReg(registers::R3, 31);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzcv"), reg(registers::R2).equals(0x00000001));
}

MICROMACHINE_TEST_F(rorRegister, RotateBy32_CarryOutHighestBit, CpuTestFixture) {
	emitInstruction16("0100000111mmmddd", registers::R7, registers::R0);
	setReg(registers::R0, 0x80000000);
	setReg(registers::R7, 32);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("NzCv"), reg(registers::R0).equals(0x80000000));
}

MICROMACHINE_TEST_F(rorRegister, RotateBy33, CpuTestFixture) {
	emitInstruction16("0100000111mmmddd", registers::R3, registers::R2);
	setReg(registers::R2, 0x80000001);
	setReg(registers::R3, 33);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("NzCv"), reg(registers::R2).equals(0xC0000000));
}

MICROMACHINE_TEST_F(rorRegister, RotateWithMaximumShiftOf255, CpuTestFixture) {
	emitInstruction16("0100000111mmmddd", registers::R3, registers::R2);
	setReg(registers::R2, 0x80000000);
	setReg(registers::R3, 255);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzcv"), reg(registers::R2).equals(0x00000001));
}

MICROMACHINE_TEST_F(rorRegister, RotateWithShiftOf256_ShouldBeTreatedAs0Shift_CarryUnmodified, CpuTestFixture) {
	emitInstruction16("0100000111mmmddd", registers::R7, registers::R0);
	setReg(registers::R0, 0x80000000);
	setReg(registers::R7, 256);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("Nzcv"), reg(registers::R0).equals(0x80000000));
}

MICROMACHINE_TEST_F(rorRegister, Rotate0by16, CpuTestFixture) {
	emitInstruction16("0100000111mmmddd", registers::R7, registers::R0);
	setReg(registers::R0, 0);
	setReg(registers::R7, 16);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZcv"), reg(registers::R0).equals(0));
}
