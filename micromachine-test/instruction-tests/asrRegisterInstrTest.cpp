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

/* ASR - Register (Arithmetic Shift Right)
   Encoding: 010000 0100 Rm:3 Rdn:3 */
MICROMACHINE_TEST_F(asrRegister, Shift1by1_CarryOutFromLowestBit, CpuTestFixture) {
	emitInstruction16("0100000100mmmddd", registers::R0, registers::R7);
	setReg(registers::R7, 1);
	setReg(registers::R0, 1);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZCv"), reg(registers::R7).equals((int32_t)1U >> 1U));
}

MICROMACHINE_TEST_F(asrRegister, Shift1by0_MinimumShift_CarryUnmodified, CpuTestFixture) {
	emitInstruction16("0100000100mmmddd", registers::R0, registers::R7);
	setReg(registers::R7, 1);
	setReg(registers::R0, 0);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzcv"), reg(registers::R7).equals((int32_t)1 >> 0));
}

MICROMACHINE_TEST_F(asrRegister, Shift2by1_NoCarryFromLowestBit, CpuTestFixture) {
	emitInstruction16("0100000100mmmddd", registers::R3, registers::R2);
	setReg(registers::R2, 2);
	setReg(registers::R3, 1);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzcv"), reg(registers::R2).equals((int32_t)2 >> 1));
}

MICROMACHINE_TEST_F(asrRegister, ShiftNegativeNumberby31, CpuTestFixture) {
	emitInstruction16("0100000100mmmddd", registers::R3, registers::R2);
	setReg(registers::R2, -1);
	setReg(registers::R3, 31);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("NzCv"), reg(registers::R2).equals((int32_t)-1 >> 31));
}

MICROMACHINE_TEST_F(asrRegister, ShiftMaximumNegativeValueBy32_CarryOutFromHighestBit, CpuTestFixture) {
	emitInstruction16("0100000100mmmddd", registers::R7, registers::R0);
	setReg(registers::R0, 0x80000000);
	setReg(registers::R7, 32);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("NzCv"), reg(registers::R0).equals(-1));
}

MICROMACHINE_TEST_F(asrRegister, ShiftNegativeValueby33, CpuTestFixture) {
	emitInstruction16("0100000100mmmddd", registers::R3, registers::R2);
	setReg(registers::R2, -1);
	setReg(registers::R3, 33);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("NzCv"), reg(registers::R2).equals(-1));
}

MICROMACHINE_TEST_F(asrRegister, ShiftPositiveValueby33, CpuTestFixture) {
	emitInstruction16("0100000100mmmddd", registers::R3, registers::R2);
	setReg(registers::R2, 0x7FFFFFFF);
	setReg(registers::R3, 33);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZcv"), reg(registers::R2).equals(0));
}

MICROMACHINE_TEST_F(asrRegister, ShiftNegativeValueByMaximumShiftOf255, CpuTestFixture) {
	emitInstruction16("0100000100mmmddd", registers::R3, registers::R2);
	setReg(registers::R2, -1);
	setReg(registers::R3, 255);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("NzCv"), reg(registers::R2).equals(-1));
}

MICROMACHINE_TEST_F(asrRegister, ShiftOf256ShouldBeTreatedAsShiftOf0_CarryUnmodified, CpuTestFixture) {
	emitInstruction16("0100000100mmmddd", registers::R7, registers::R0);
	setReg(registers::R0, -1);
	setReg(registers::R7, 256);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("Nzcv"), reg(registers::R0).equals((int32_t)-1 >> 0));
}

MICROMACHINE_TEST_F(asrRegister, ShiftLargestPositiveNumberBy31, CpuTestFixture) {
	emitInstruction16("0100000100mmmddd", registers::R2, registers::R3);
	setReg(registers::R3, 0x7FFFFFFF);
	setReg(registers::R2, 31);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZCv"), reg(registers::R3).equals((int32_t)0x7FFFFFFF >> 31));
}

MICROMACHINE_TEST_F(asrRegister, ShiftLargestNegativeNumberBy1, CpuTestFixture) {
	emitInstruction16("0100000100mmmddd", registers::R2, registers::R3);
	setReg(registers::R3, 0x80000000);
	setReg(registers::R2, 1);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("Nzcv"), reg(registers::R3).equals((int32_t)0x80000000 >> 1));
}
