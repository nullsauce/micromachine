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


/* LSL - Register (Logical Shift Left)
   Encoding: 010000 0010 Rm:3 Rdn:3 */
MICROMACHINE_TEST_F(lslRegister, ShiftR7by0_MinimumShift_CarryShouldBeUnmodified, CpuTestFixture) {
	getCpu().regs().set(registers::R7, 0x77777777U);
	code_gen().emit_ins16("0100000010mmmddd", registers::R0, registers::R7);
	getCpu().special_regs().app_status_register().write_carry_flag(true);
	Step();
	ExpectThat().APSRFlagsMatches("nzC");
	ExpectThat().Register(registers::R7).Equals(0x77777777U);
}

MICROMACHINE_TEST_F(lslRegister, ShiftValue1by31_NegativeResult, CpuTestFixture) {
	code_gen().emit_ins16("0100000010mmmddd", registers::R4, registers::R3);
	getCpu().regs().set(registers::R3, 1);
	getCpu().regs().set(registers::R4, 31);
	Step();
	ExpectThat().APSRFlagsMatches("Nzc");
	ExpectThat().Register(registers::R3).Equals(1 << 31);
}

MICROMACHINE_TEST_F(lslRegister, ShiftValue1by32_CarryOutFromLowestBit, CpuTestFixture) {
	code_gen().emit_ins16("0100000010mmmddd", registers::R7, registers::R0);
	getCpu().regs().set(registers::R0, 1);
	getCpu().regs().set(registers::R7, 32);
	Step();
	ExpectThat().APSRFlagsMatches("nZC");
	ExpectThat().Register(registers::R0).Equals(0);
}

MICROMACHINE_TEST_F(lslRegister, ShiftNegativeValueBy1_CarryOutFromHighestBit, CpuTestFixture) {
	code_gen().emit_ins16("0100000010mmmddd", registers::R3, registers::R4);
	getCpu().regs().set(registers::R4, -1);
	getCpu().regs().set(registers::R3, 1);
	Step();
	ExpectThat().APSRFlagsMatches("NzC");
	ExpectThat().Register(registers::R4).Equals(0xffffffff << 1);
}

MICROMACHINE_TEST_F(lslRegister, ShiftValue1by33_NoCarry, CpuTestFixture) {
	code_gen().emit_ins16("0100000010mmmddd", registers::R7, registers::R0);
	getCpu().regs().set(registers::R0, 1);
	getCpu().regs().set(registers::R7, 33);
	Step();
	ExpectThat().APSRFlagsMatches("nZc");
	ExpectThat().Register(registers::R0).Equals(0);
}

MICROMACHINE_TEST_F(lslRegister, ShiftValuee1by255_MaximumShift, CpuTestFixture) {
	code_gen().emit_ins16("0100000010mmmddd", registers::R7, registers::R0);
	getCpu().regs().set(registers::R0, 1);
	getCpu().regs().set(registers::R7, 255);
	Step();
	ExpectThat().APSRFlagsMatches("nZc");
	ExpectThat().Register(registers::R0).Equals(0);
}

MICROMACHINE_TEST_F(lslRegister, ShiftValue1by256_ShouldBeTreatedAs0Shift_CarryUnmodified, CpuTestFixture) {
	code_gen().emit_ins16("0100000010mmmddd", registers::R7, registers::R0);
	getCpu().special_regs().app_status_register().write_carry_flag(false);
	getCpu().regs().set(registers::R0, 1);
	getCpu().regs().set(registers::R7, 256);
	Step();
	ExpectThat().APSRFlagsMatches("nzc");
	ExpectThat().Register(registers::R0).Equals(1);
}
