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


/* ORR - Register
   Encoding: 010000 1100 Rm:3 Rdn:3 */
/* NOTE: APSR_C state is maintained by this instruction. */
MICROMACHINE_TEST_F(orrRegister, UseLowestRegisterForBothArgs, CpuTestFixture) {
	code_gen().emit_ins16("0100001100mmmddd", registers::R0, registers::R0);
	// Use a couple of tests to explicitly set/clear carry to verify both states are maintained.
	getCpu().special_regs().app_status_register().write_carry_flag(false);
	Step();
	ExpectThat().Register(registers::R0).Equals(0);
	ExpectThat().APSRFlagsMatches("nZc");
}

MICROMACHINE_TEST_F(orrRegister, UseHighestRegisterForBothArgs, CpuTestFixture) {
	getCpu().regs().set(registers::R7, 0x77777777);
	code_gen().emit_ins16("0100001100mmmddd", registers::R7, registers::R7);
	getCpu().special_regs().app_status_register().write_carry_flag(true);
	Step();
	ExpectThat().APSRFlagsMatches("nzC");
}

MICROMACHINE_TEST_F(orrRegister, OrR3andR7, CpuTestFixture) {
	getCpu().regs().set(registers::R3, 0x33333333);
	getCpu().regs().set(registers::R7, 0x77777777);
	code_gen().emit_ins16("0100001100mmmddd", registers::R3, registers::R7);
	Step();
	ExpectThat().APSRFlagsMatches("nz");
	ExpectThat().Register(registers::R7).Equals(0x33333333 | 0x77777777);
}

MICROMACHINE_TEST_F(orrRegister, UseOrToTurnOnNegativeSignBit, CpuTestFixture) {
	code_gen().emit_ins16("0100001100mmmddd", registers::R7, registers::R0);
	getCpu().regs().set(registers::R0, 0x7FFFFFFF);
	getCpu().regs().set(registers::R7, 0x80000000);
	Step();
	ExpectThat().APSRFlagsMatches("Nz");
	ExpectThat().Register(registers::R0).Equals(0x7FFFFFFF | 0x80000000);
}

MICROMACHINE_TEST_F(orrRegister, HaveAndResultNotBeSameAsEitherSource, CpuTestFixture) {
	code_gen().emit_ins16("0100001100mmmddd", registers::R0, registers::R7);
	getCpu().regs().set(registers::R0, 0x12345678);
	getCpu().regs().set(registers::R7, 0xF0F0F0F0);
	Step();
	ExpectThat().APSRFlagsMatches("Nz");
	ExpectThat().Register(registers::R7).Equals(0xF2F4F6F8);
}
