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


/* AND - Register
   Encoding: 010000 0000 Rm:3 Rdn:3 */
/* NOTE: APSR_C state is maintained by this instruction. */
MICROMACHINE_TEST_F(andRegister, UseLowestRegisterForBothArgs, CpuTestFixture) {
	code_gen().emit_ins16("0100000000mmmddd", registers::R0, registers::R0);
	// Use a couple of tests to explicitly set/clear carry to verify both states are maintained.

	Step();
	ExpectThat().APSRFlagsMatches("nZc");
	ExpectThat().Register(registers::R0).Equals(0);
}

MICROMACHINE_TEST_F(andRegister, UseHighestRegisterForBothArgs, CpuTestFixture) {
	code_gen().emit_ins16("0100000000mmmddd", registers::R7, registers::R7);
	getCpu().regs().set(registers::R7, 0x77777777);

	// check that carry remains set
	getCpu().special_regs().app_status_register().write_carry_flag(true);
	Step();
	ExpectThat().Register(registers::R7).Equals(0x77777777);
	ExpectThat().APSRFlagsMatches("nzC");
}

MICROMACHINE_TEST_F(andRegister, AndR3andR7, CpuTestFixture) {
	code_gen().emit_ins16("0100000000mmmddd", registers::R3, registers::R7);
	getCpu().regs().set(registers::R7, 0x77777777);
	getCpu().regs().set(registers::R3, 0x33333333);
	Step();
	ExpectThat().APSRFlagsMatches("nz");
	ExpectThat().Register(registers::R7).Equals(0x33333333);
}

MICROMACHINE_TEST_F(andRegister, UseAndToJustKeepNegativeSignBit, CpuTestFixture) {
	code_gen().emit_ins16("0100000000mmmddd", registers::R6, registers::R1);
	getCpu().regs().set(registers::R1, -1);
	getCpu().regs().set(registers::R6, 0x80000000);
	Step();
	ExpectThat().APSRFlagsMatches("Nz");
	ExpectThat().Register(registers::R1).Equals(0x80000000);
}

MICROMACHINE_TEST_F(andRegister, HaveAndResultNotBeSameAsEitherSource, CpuTestFixture) {
	code_gen().emit_ins16("0100000000mmmddd", registers::R5, registers::R2);
	getCpu().regs().set(registers::R2, 0x12345678);
	getCpu().regs().set(registers::R5, 0xF0F0F0F0);
	Step();
	ExpectThat().APSRFlagsMatches("nz");
	ExpectThat().Register(registers::R2).Equals(0x10305070);
}
