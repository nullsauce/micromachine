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


/* TST - Register
   Encoding: 010000 1000 Rm:3 Rn:3 */
/* NOTE: APSR_C state is maintained by this instruction. */
MICROMACHINE_TEST_F(tstRegister, UseLowestRegisterForBothArgsAndResultShouldBeZero, CpuTestFixture) {
	code_gen().emit_ins16("0100001000mmmnnn", registers::R0, registers::R0);
	// Use a couple of tests to explicitly set/clear carry to verify both states are maintained.
	getCpu().regs().app_status_register().write_carry_flag(false);
	Step();
	ExpectThat().XPSRFlagsEquals("nZc");
}

MICROMACHINE_TEST_F(tstRegister, UseHighestRegisterForBothArgsAndRegisterWillBeUnchanged, CpuTestFixture) {
	getCpu().regs().set(registers::R7, 0x77777777U);
	code_gen().emit_ins16("0100001000mmmnnn", registers::R7, registers::R7);
	getCpu().regs().app_status_register().write_carry_flag(true);
	Step();
	ExpectThat().XPSRFlagsEquals("nzC");
}

MICROMACHINE_TEST_F(tstRegister, AndR3andR7, CpuTestFixture) {
	getCpu().regs().set(registers::R3, 0x33333333U);
	getCpu().regs().set(registers::R7, 0x77777777U);
	code_gen().emit_ins16("0100001000mmmnnn", registers::R3, registers::R7);
	Step();
	ExpectThat().XPSRFlagsEquals("nz");
}

MICROMACHINE_TEST_F(tstRegister, UseAndToJustKeepNegativeSignBit, CpuTestFixture) {
	getCpu().regs().set(registers::R7, 0x77777777U);
	code_gen().emit_ins16("0100001000mmmnnn", registers::R7, registers::R0);
	getCpu().regs().set(registers::R0, -1);
	getCpu().regs().set(registers::R7, 0x80000000);
	Step();
	ExpectThat().XPSRFlagsEquals("Nz");
}
