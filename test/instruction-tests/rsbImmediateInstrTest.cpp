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


/* RSB - Immediate
   Encoding: 010000 1001 Rn:3 Rd:3 */
MICROMACHINE_TEST_F(rsbImmediate, UseLowestRegisterOnly, CpuTestFixture) {
	code_gen().emit_ins16("0100001001nnnddd", registers::R0, registers::R0);
	Step();
	ExpectThat().APSRFlagsMatches("nZCv");
	ExpectThat().Register(registers::R0).Equals(0U);
}

MICROMACHINE_TEST_F(rsbImmediate, UseHigestRegisterOnly, CpuTestFixture) {
	getCpu().regs().set(registers::R7, 0x77777777U);
	code_gen().emit_ins16("0100001001nnnddd", registers::R7, registers::R7);
	Step();
	ExpectThat().APSRFlagsMatches("Nzcv");
	ExpectThat().Register(registers::R7).Equals(-0x77777777U);
}

MICROMACHINE_TEST_F(rsbImmediate, UseDifferentRegistersForEachArg, CpuTestFixture) {
	getCpu().regs().set(registers::R2, 0x22222222U);
	code_gen().emit_ins16("0100001001nnnddd", registers::R2, registers::R0);
	Step();
	ExpectThat().APSRFlagsMatches("Nzcv");
	ExpectThat().Register(registers::R0).Equals(-0x22222222);
}

MICROMACHINE_TEST_F(rsbImmediate, ForceOverflowByNegatingLargestNegativeValue, CpuTestFixture) {
	getCpu().regs().set(registers::R7, 0x77777777U);
	code_gen().emit_ins16("0100001001nnnddd", registers::R0, registers::R7);
	getCpu().regs().set(registers::R0, 0x80000000);
	Step();
	ExpectThat().APSRFlagsMatches("NzcV");
	ExpectThat().Register(registers::R7).Equals(0x80000000U);
}
