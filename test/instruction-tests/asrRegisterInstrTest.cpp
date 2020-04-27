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


/* ASR - Register (Arithmetic Shift Right)
   Encoding: 010000 0100 Rm:3 Rdn:3 */
MICROMACHINE_TEST_F(asrRegister, Shift1by1_CarryOutFromLowestBit, CpuTestFixture) {
	code_gen().emit_ins16("0100000100mmmddd", registers::R0, registers::R7);
	getCpu().regs().set(registers::R7, 1);
	getCpu().regs().set(registers::R0, 1);
	Step();
	ExpectThat().APSRFlagsMatches("nZC");
	ExpectThat().Register(registers::R7).Equals((int32_t)1U >> 1U);
}

MICROMACHINE_TEST_F(asrRegister, Shift1by0_MinimumShift_CarryUnmodified, CpuTestFixture) {
	code_gen().emit_ins16("0100000100mmmddd", registers::R0, registers::R7);
	getCpu().regs().set(registers::R7, 1);
	getCpu().regs().set(registers::R0, 0);
	Step();
	ExpectThat().APSRFlagsMatches("nz");
	ExpectThat().Register(registers::R7).Equals((int32_t)1 >> 0);
}

MICROMACHINE_TEST_F(asrRegister, Shift2by1_NoCarryFromLowestBit, CpuTestFixture) {
	code_gen().emit_ins16("0100000100mmmddd", registers::R3, registers::R2);
	getCpu().regs().set(registers::R2, 2);
	getCpu().regs().set(registers::R3, 1);
	Step();
	ExpectThat().APSRFlagsMatches("nzc");
	ExpectThat().Register(registers::R2).Equals((int32_t)2 >> 1);
}

MICROMACHINE_TEST_F(asrRegister, ShiftNegativeNumberby31, CpuTestFixture) {
	code_gen().emit_ins16("0100000100mmmddd", registers::R3, registers::R2);
	getCpu().regs().set(registers::R2, -1);
	getCpu().regs().set(registers::R3, 31);
	Step();
	ExpectThat().APSRFlagsMatches("NzC");
	ExpectThat().Register(registers::R2).Equals((int32_t)-1 >> 31);
}

MICROMACHINE_TEST_F(asrRegister, ShiftMaximumNegativeValueBy32_CarryOutFromHighestBit, CpuTestFixture) {
	code_gen().emit_ins16("0100000100mmmddd", registers::R7, registers::R0);
	getCpu().regs().set(registers::R0, 0x80000000);
	getCpu().regs().set(registers::R7, 32);
	Step();
	ExpectThat().APSRFlagsMatches("NzC");
	ExpectThat().Register(registers::R0).Equals(-1);
}

MICROMACHINE_TEST_F(asrRegister, ShiftNegativeValueby33, CpuTestFixture) {
	code_gen().emit_ins16("0100000100mmmddd", registers::R3, registers::R2);
	getCpu().regs().set(registers::R2, -1);
	getCpu().regs().set(registers::R3, 33);
	Step();
	ExpectThat().APSRFlagsMatches("NzC");
	ExpectThat().Register(registers::R2).Equals(-1);
}

MICROMACHINE_TEST_F(asrRegister, ShiftPositiveValueby33, CpuTestFixture) {
	code_gen().emit_ins16("0100000100mmmddd", registers::R3, registers::R2);
	getCpu().regs().set(registers::R2, 0x7FFFFFFF);
	getCpu().regs().set(registers::R3, 33);
	Step();
	ExpectThat().APSRFlagsMatches("nZc");
	ExpectThat().Register(registers::R2).Equals(0);
}

MICROMACHINE_TEST_F(asrRegister, ShiftNegativeValueByMaximumShiftOf255, CpuTestFixture) {
	code_gen().emit_ins16("0100000100mmmddd", registers::R3, registers::R2);
	getCpu().regs().set(registers::R2, -1);
	getCpu().regs().set(registers::R3, 255);
	Step();
	ExpectThat().APSRFlagsMatches("NzC");
	ExpectThat().Register(registers::R2).Equals(-1);
}

MICROMACHINE_TEST_F(asrRegister, ShiftOf256ShouldBeTreatedAsShiftOf0_CarryUnmodified, CpuTestFixture) {
	code_gen().emit_ins16("0100000100mmmddd", registers::R7, registers::R0);
	getCpu().regs().set(registers::R0, -1);
	getCpu().regs().set(registers::R7, 256);
	Step();
	ExpectThat().APSRFlagsMatches("Nz");
	ExpectThat().Register(registers::R0).Equals((int32_t)-1 >> 0);
}

MICROMACHINE_TEST_F(asrRegister, ShiftLargestPositiveNumberBy31, CpuTestFixture) {
	code_gen().emit_ins16("0100000100mmmddd", registers::R2, registers::R3);
	getCpu().regs().set(registers::R3, 0x7FFFFFFF);
	getCpu().regs().set(registers::R2, 31);
	Step();
	ExpectThat().APSRFlagsMatches("nZC");
	ExpectThat().Register(registers::R3).Equals((int32_t)0x7FFFFFFF >> 31);
}

MICROMACHINE_TEST_F(asrRegister, ShiftLargestNegativeNumberBy1, CpuTestFixture) {
	code_gen().emit_ins16("0100000100mmmddd", registers::R2, registers::R3);
	getCpu().regs().set(registers::R3, 0x80000000);
	getCpu().regs().set(registers::R2, 1);
	Step();
	ExpectThat().APSRFlagsMatches("Nzc");
	ExpectThat().Register(registers::R3).Equals((int32_t)0x80000000 >> 1);
}
