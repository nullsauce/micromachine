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


/* MOV - Immediate
   Encoding: 001 00 Rd:3 Imm:8 */
/* NOTE: APSR_C state is maintained by this instruction. */
MICROMACHINE_TEST_F(movImmediate, MovToR0, CpuTestFixture) {
	code_gen().emit_ins16("00100dddiiiiiiii", registers::R0, 127);
	// Use a couple of tests to explicitly set/clear carry to verify both states are maintained.
	getCpu().regs().app_status_register().write_carry_flag(false);
	Step();
	ExpectThat().Register(registers::R0).Equals(127);
	ExpectThat().XPSRFlagsEquals("nzc");
}

MICROMACHINE_TEST_F(movImmediate, MovToR7, CpuTestFixture) {
	code_gen().emit_ins16("00100dddiiiiiiii", registers::R7, 127);
	getCpu().regs().app_status_register().write_carry_flag(true);
	Step();
	ExpectThat().XPSRFlagsEquals("nzC");
	ExpectThat().Register(registers::R7).Equals(127);
}

MICROMACHINE_TEST_F(movImmediate, MovSmallestImmediateValueToR3, CpuTestFixture) {
	code_gen().emit_ins16("00100dddiiiiiiii", registers::R3, 0);
	Step();
	ExpectThat().XPSRFlagsEquals("nZ");
	ExpectThat().Register(registers::R3).Equals(0);
}

MICROMACHINE_TEST_F(movImmediate, MovLargestImmediateValueToR3, CpuTestFixture) {
	code_gen().emit_ins16("00100dddiiiiiiii", registers::R3, 255);
	Step();
	ExpectThat().XPSRFlagsEquals("nz");
	ExpectThat().Register(registers::R3).Equals(255);
}
