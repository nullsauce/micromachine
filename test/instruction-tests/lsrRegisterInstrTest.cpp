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


/* LSR - Register (Logical Shift Right)
   Encoding: 010000 0011 Rm:3 Rdn:3 */
MICROMACHINE_TEST_F(lsrRegister, ShiftValue1by1_CarryOutFromLowestBit, CpuTestFixture) {
	code_gen().emit_ins16("0100000011mmmddd", registers::R0, registers::R7);
	getCpu().regs().set(registers::R0, 1);
	getCpu().regs().set(registers::R7, 1);
	Step();
	ExpectThat().APSRFlagsMatches("nZC");
	ExpectThat().Register(registers::R7).Equals(0);
}

MICROMACHINE_TEST_F(lsrRegister, ShiftValue1by0_MinimumShift_CarryUnmodified, CpuTestFixture) {
	code_gen().emit_ins16("0100000011mmmddd", registers::R0, registers::R7);
	getCpu().special_regs().app_status_register().write_carry_flag(false);
	getCpu().regs().set(registers::R7, 1);
	getCpu().regs().set(registers::R0, 0);
	Step();
	ExpectThat().APSRFlagsMatches("nzc");
	ExpectThat().Register(registers::R7).Equals(1);
}

MICROMACHINE_TEST_F(lsrRegister, ShiftValue2by1_NoCarry, CpuTestFixture) {
	code_gen().emit_ins16("0100000011mmmddd", registers::R3, registers::R2);
	getCpu().regs().set(registers::R2, 2);
	getCpu().regs().set(registers::R3, 1);
	Step();
	ExpectThat().APSRFlagsMatches("nzc");
	ExpectThat().Register(registers::R2).Equals(2 >> 1);
}

MICROMACHINE_TEST_F(lsrRegister, ShiftNegativeValueBy31, CpuTestFixture) {
	code_gen().emit_ins16("0100000011mmmddd", registers::R3, registers::R2);
	getCpu().regs().set(registers::R2, -1);
	getCpu().regs().set(registers::R3, 31);
	Step();
	ExpectThat().APSRFlagsMatches("nzC");
	ExpectThat().Register(registers::R2).Equals(-1U >> 31);
}

MICROMACHINE_TEST_F(lsrRegister, ShiftNegativeValueBy32_CarryOutFromHighestBit, CpuTestFixture) {
	code_gen().emit_ins16("0100000011mmmddd", registers::R7, registers::R0);
	getCpu().regs().set(registers::R0, 0x80000000);
	getCpu().regs().set(registers::R7, 32);
	Step();
	ExpectThat().APSRFlagsMatches("nZC");
	ExpectThat().Register(registers::R0).Equals(0);
}

MICROMACHINE_TEST_F(lsrRegister, ShiftNegativeValueBy33_ResultIsZero_CarryClear, CpuTestFixture) {
	code_gen().emit_ins16("0100000011mmmddd", registers::R3, registers::R2);
	getCpu().regs().set(registers::R2, -1);
	getCpu().regs().set(registers::R3, 33);
	Step();
	ExpectThat().APSRFlagsMatches("nZc");
	ExpectThat().Register(registers::R2).Equals(0);
}

MICROMACHINE_TEST_F(lsrRegister, MaximumShiftOf255_ResultIsZero_CarryClear, CpuTestFixture) {
	code_gen().emit_ins16("0100000011mmmddd", registers::R3, registers::R2);
	getCpu().regs().set(registers::R2, -1);
	getCpu().regs().set(registers::R3, 255);
	Step();
	ExpectThat().APSRFlagsMatches("nZc");
	ExpectThat().Register(registers::R2).Equals(0);
}

MICROMACHINE_TEST_F(lsrRegister, ShiftOf256_ShouldBeTreatedAs0Shift_CarryUnmodified, CpuTestFixture) {
	code_gen().emit_ins16("0100000011mmmddd", registers::R7, registers::R0);
	getCpu().special_regs().app_status_register().write_carry_flag(true);
	getCpu().regs().set(registers::R0, -1);
	getCpu().regs().set(registers::R7, 256);
	Step();
	ExpectThat().APSRFlagsMatches("NzC");
	ExpectThat().Register(registers::R0).Equals(-1);
}
