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


/* SUB - Register
   Encoding: 000 11 0 1 Rm:3 Rn:3 Rd:3 */
MICROMACHINE_TEST_F(subRegister, UseLowestRegisterForAllArgs, CpuTestFixture) {
	code_gen().emit_ins16("0001101mmmnnnddd", registers::R0, registers::R0, registers::R0);
	Step();
	ExpectThat().APSRFlagsMatches("nZCv");
	ExpectThat().Register(registers::R0).Equals(0);
}

MICROMACHINE_TEST_F(subRegister, UseHigestRegisterForAllArgs, CpuTestFixture) {
	getCpu().regs().set(registers::R7, 0x77777777U);
	code_gen().emit_ins16("0001101mmmnnnddd", registers::R7, registers::R7, registers::R7);
	Step();
	ExpectThat().APSRFlagsMatches("nZCv");
	ExpectThat().Register(registers::R7).Equals(0);
}

MICROMACHINE_TEST_F(subRegister, UseDifferentRegistersForEachArg, CpuTestFixture) {
	getCpu().regs().set(registers::R1, 0x11111111U);
	getCpu().regs().set(registers::R2, 0x22222222U);
	code_gen().emit_ins16("0001101mmmnnnddd", registers::R1, registers::R2, registers::R0);
	Step();
	ExpectThat().APSRFlagsMatches("nzCv");
	ExpectThat().Register(registers::R0).Equals(0x22222222U - 0x11111111U);
}

// Force APSR flags to be set which haven't already been covered above.
MICROMACHINE_TEST_F(subRegister, ForceCarryClearToIndicateBorrowAndResultWillBeNegative, CpuTestFixture) {
	code_gen().emit_ins16("0001101mmmnnnddd", registers::R1, registers::R0, registers::R2);
	getCpu().regs().set(registers::R1, 1);
	Step();
	ExpectThat().APSRFlagsMatches("Nzcv");
	ExpectThat().Register(registers::R2).Equals(0U - 1U);
}

MICROMACHINE_TEST_F(subRegister, ForceNegativeOverflow, CpuTestFixture) {
	code_gen().emit_ins16("0001101mmmnnnddd", registers::R1, registers::R2, registers::R0);
	getCpu().regs().set(registers::R2, 0x80000000U);
	getCpu().regs().set(registers::R1, 1U);
	Step();
	ExpectThat().APSRFlagsMatches("nzCV");
	ExpectThat().Register(registers::R0).Equals(0x7FFFFFFF);
}

MICROMACHINE_TEST_F(subRegister, ForcePositiveOverflow, CpuTestFixture) {
	code_gen().emit_ins16("0001101mmmnnnddd", registers::R1, registers::R2, registers::R0);
	getCpu().regs().set(registers::R2, 0x7FFFFFFFU);
	getCpu().regs().set(registers::R1, -1U);
	Step();
	ExpectThat().APSRFlagsMatches("NzcV");
	ExpectThat().Register(registers::R0).Equals(0x7FFFFFFFU + 1U);
}
