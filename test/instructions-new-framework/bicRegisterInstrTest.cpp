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


/* BIC - Register
   Encoding: 010000 1110 Rm:3 Rdn:3 */
/* NOTE: APSR_C state is maintained by this instruction. */
MICROMACHINE_TEST_F(bicRegister, UseLowestRegisterForBothArgs, CpuTestFixture) {
	code_gen().emit_ins16("0100001110mmmddd", registers::R0, registers::R0);
	// Use a couple of tests to explicitly set/clear carry to verify both states are maintained.

	// clear carry flag
	getCpu().regs().app_status_register().write_carry_flag(false);
	Step();
	ExpectThat().XPSRFlagsEquals("nZc");
	ExpectThat().Register(registers::R0).Equals(0);
}

MICROMACHINE_TEST_F(bicRegister, UseHighestRegisterForBothArgs, CpuTestFixture) {
	code_gen().emit_ins16("0100001110mmmddd", registers::R7, registers::R7);
	// set carry flag
	getCpu().regs().app_status_register().write_carry_flag(true);
	Step();
	ExpectThat().XPSRFlagsEquals("nZC");
	ExpectThat().Register(registers::R7).Equals(0);
}

MICROMACHINE_TEST_F(bicRegister, UseR3andR7, CpuTestFixture) {
	getCpu().regs().set(registers::R3, 0x33333333);
	getCpu().regs().set(registers::R7, 0x77777777);
	code_gen().emit_ins16("0100001110mmmddd", registers::R3, registers::R7);
	Step();
	ExpectThat().XPSRFlagsEquals("nz");
	ExpectThat().Register(registers::R7).Equals(0x77777777 & ~0x33333333);
}

MICROMACHINE_TEST_F(bicRegister, UseBicToClearLSbit, CpuTestFixture) {
	code_gen().emit_ins16("0100001110mmmddd", registers::R6, registers::R1);
	getCpu().regs().set(registers::R1, -1);
	getCpu().regs().set(registers::R6, 1);
	Step();
	ExpectThat().XPSRFlagsEquals("Nz");
	ExpectThat().Register(registers::R1).Equals(-1U & ~1);
}
