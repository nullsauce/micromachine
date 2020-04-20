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


/* ROR - Register (ROtate Right)
   Encoding: 010000 0111 Rm:3 Rdn:3 */
MICROMACHINE_TEST_F(rorRegister, Rotate1by1_CarryOutFromLowestBit, CpuTestFixture) {
	code_gen().emit_ins16("0100000111mmmddd", registers::R0, registers::R7);
	getCpu().regs().set(registers::R0, 1);
	getCpu().regs().set(registers::R7, 1);
	Step();
	ExpectThat().APSRFlagsMatches("NzC");
	ExpectThat().Register(registers::R7).Equals(0x80000000);
}

MICROMACHINE_TEST_F(rorRegister, Rotate1by0_MinimumShift_CarryUnmodified, CpuTestFixture) {
	code_gen().emit_ins16("0100000111mmmddd", registers::R7, registers::R0);
	getCpu().regs().set(registers::R0, 1);
	getCpu().regs().set(registers::R7, 0);
	Step();
	ExpectThat().APSRFlagsMatches("nz");
	ExpectThat().Register(registers::R0).Equals(1);
}

MICROMACHINE_TEST_F(rorRegister, Rotate2by1_NoCarry, CpuTestFixture) {
	code_gen().emit_ins16("0100000111mmmddd", registers::R3, registers::R2);
	getCpu().regs().set(registers::R2, 2);
	getCpu().regs().set(registers::R3, 1);
	Step();
	ExpectThat().APSRFlagsMatches("nzc");
	ExpectThat().Register(registers::R2).Equals(2 >> 1);
}

MICROMACHINE_TEST_F(rorRegister, Rotate16Bits, CpuTestFixture) {
	code_gen().emit_ins16("0100000111mmmddd", registers::R3, registers::R2);
	getCpu().regs().set(registers::R2, 0x12345678);
	getCpu().regs().set(registers::R3, 16);
	Step();
	ExpectThat().APSRFlagsMatches("nzc");
	ExpectThat().Register(registers::R2).Equals(0x56781234);
}

MICROMACHINE_TEST_F(rorRegister, RotateWithShiftOf31, CpuTestFixture) {
	code_gen().emit_ins16("0100000111mmmddd", registers::R3, registers::R2);
	getCpu().regs().set(registers::R2, 0x80000000);
	getCpu().regs().set(registers::R3, 31);
	Step();
	ExpectThat().APSRFlagsMatches("nzc");
	ExpectThat().Register(registers::R2).Equals(0x00000001);
}

MICROMACHINE_TEST_F(rorRegister, RotateBy32_CarryOutHighestBit, CpuTestFixture) {
	code_gen().emit_ins16("0100000111mmmddd", registers::R7, registers::R0);
	getCpu().regs().set(registers::R0, 0x80000000);
	getCpu().regs().set(registers::R7, 32);
	Step();
	ExpectThat().APSRFlagsMatches("NzC");
	ExpectThat().Register(registers::R0).Equals(0x80000000);
}

MICROMACHINE_TEST_F(rorRegister, RotateBy33, CpuTestFixture) {
	code_gen().emit_ins16("0100000111mmmddd", registers::R3, registers::R2);
	getCpu().regs().set(registers::R2, 0x80000001);
	getCpu().regs().set(registers::R3, 33);
	Step();
	ExpectThat().APSRFlagsMatches("NzC");
	ExpectThat().Register(registers::R2).Equals(0xC0000000);
}

MICROMACHINE_TEST_F(rorRegister, RotateWithMaximumShiftOf255, CpuTestFixture) {
	code_gen().emit_ins16("0100000111mmmddd", registers::R3, registers::R2);
	getCpu().regs().set(registers::R2, 0x80000000);
	getCpu().regs().set(registers::R3, 255);
	Step();
	ExpectThat().APSRFlagsMatches("nzc");
	ExpectThat().Register(registers::R2).Equals(0x00000001);
}

MICROMACHINE_TEST_F(rorRegister, RotateWithShiftOf256_ShouldBeTreatedAs0Shift_CarryUnmodified, CpuTestFixture) {
	code_gen().emit_ins16("0100000111mmmddd", registers::R7, registers::R0);
	getCpu().regs().set(registers::R0, 0x80000000);
	getCpu().regs().set(registers::R7, 256);
	Step();
	ExpectThat().APSRFlagsMatches("Nz");
	ExpectThat().Register(registers::R0).Equals(0x80000000);
}

MICROMACHINE_TEST_F(rorRegister, Rotate0by16, CpuTestFixture) {
	code_gen().emit_ins16("0100000111mmmddd", registers::R7, registers::R0);
	getCpu().regs().set(registers::R0, 0);
	getCpu().regs().set(registers::R7, 16);
	Step();
	ExpectThat().APSRFlagsMatches("nZc");
	ExpectThat().Register(registers::R0).Equals(0);
}
