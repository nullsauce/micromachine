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


/* CMN - Register (Compare Negative)
   Encoding: 010000 1011 Rm:3 Rn:3 */
MICROMACHINE_TEST_F(cmnRegister, UseLowestRegisterForAllArgs, CpuTestFixture) {
	code_gen().emit_ins16("0100001011mmmnnn", registers::R0, registers::R0);
	Step();
	ExpectThat().APSRFlagsMatches("nZcv");
}

MICROMACHINE_TEST_F(cmnRegister, UseHigestRegisterForAllArgs, CpuTestFixture) {
	getCpu().regs().set(registers::R7, 0x77777777);
	code_gen().emit_ins16("0100001011mmmnnn", registers::R7, registers::R7);
	Step();
	ExpectThat().APSRFlagsMatches("NzcV");
}

MICROMACHINE_TEST_F(cmnRegister, UseDifferentRegistersForEachArg, CpuTestFixture) {
	getCpu().regs().set(registers::R1, 0x11111111);
	getCpu().regs().set(registers::R2, 0x22222222);
	code_gen().emit_ins16("0100001011mmmnnn", registers::R1, registers::R2);
	Step();
	ExpectThat().APSRFlagsMatches("nzcv");
}

// Force APSR flags to be set which haven't already been covered above.
MICROMACHINE_TEST_F(cmnRegister, ForceCarryWithNoOverflow, CpuTestFixture) {
	code_gen().emit_ins16("0100001011mmmnnn", registers::R1, registers::R2);
	getCpu().regs().set(registers::R1, -1);
	getCpu().regs().set(registers::R2, 1);
	Step();
	ExpectThat().APSRFlagsMatches("nZCv");
}

MICROMACHINE_TEST_F(cmnRegister, ForceCarryAndOverflow, CpuTestFixture) {
	code_gen().emit_ins16("0100001011mmmnnn", registers::R1, registers::R2);
	getCpu().regs().set(registers::R1, -1);
	getCpu().regs().set(registers::R2, 0x80000000U);
	Step();
	ExpectThat().APSRFlagsMatches("nzCV");
}
