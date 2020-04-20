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


/* MVN - Register (MOve Negative)
   Encoding: 010000 1111 Rm:3 Rd:3 */
/* NOTE: APSR_C state is maintained by this instruction. */
MICROMACHINE_TEST_F(mvnRegister, UseLowestRegisterForAllArgs, CpuTestFixture) {
	// Use a couple of tests to explicitly set/clear carry to verify both states are maintained.
	code_gen().emit_ins16("0100001111mmmddd", registers::R0, registers::R0);
	getCpu().regs().app_status_register().write_carry_flag(true);
	Step();
	ExpectThat().XPSRFlagsEquals("NzC");
	ExpectThat().Register(registers::R0).Equals(~0U);
}

MICROMACHINE_TEST_F(mvnRegister, UseHigestRegisterForAllArgs, CpuTestFixture) {
	getCpu().regs().set(registers::R7, 0x77777777U);
	code_gen().emit_ins16("0100001111mmmddd", registers::R7, registers::R7);
	getCpu().regs().app_status_register().write_carry_flag(false);
	Step();
	ExpectThat().XPSRFlagsEquals("Nzc");
	ExpectThat().Register(registers::R7).Equals(~0x77777777U);
}

MICROMACHINE_TEST_F(mvnRegister, UseDifferentRegistersForEachArg, CpuTestFixture) {
	getCpu().regs().set(registers::R1, 0x11111111U);
	getCpu().regs().set(registers::R2, 0x22222222U);
	code_gen().emit_ins16("0100001111mmmddd", registers::R2, registers::R1);
	Step();
	ExpectThat().XPSRFlagsEquals("Nz");
	ExpectThat().Register(registers::R1).Equals(~0x22222222U);
}

MICROMACHINE_TEST_F(mvnRegister, MoveANegationOfNegativeOne_ClearsNegativeFlagAndSetsZeroFlag, CpuTestFixture) {
	code_gen().emit_ins16("0100001111mmmddd", registers::R2, registers::R1);
	getCpu().regs().set(registers::R2, -1);
	Step();
	ExpectThat().XPSRFlagsEquals("nZ");
	ExpectThat().Register(registers::R1).Equals(0U);
}
