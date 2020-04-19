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


/* CMP - Immediate
   Encoding: 001 01 Rn:3 Imm:8 */
MICROMACHINE_TEST_F(cmpImmediate, CompareLowestRegisterToEqualValue, CpuTestFixture) {
	code_gen().emit_ins16("00101nnniiiiiiii", registers::R0, 0);
	Step();
	ExpectThat().XPSRFlagsEquals("nZCv");
}

MICROMACHINE_TEST_F(cmpImmediate, CompareHighestRegisterToImmediateWhichIsSmaller, CpuTestFixture) {
	getCpu().regs().set(registers::R7, 0x77777777);
	code_gen().emit_ins16("00101nnniiiiiiii", registers::R7, 127);
	Step();
	ExpectThat().XPSRFlagsEquals("nzCv");
}

MICROMACHINE_TEST_F(cmpImmediate, CompareRegisterToLargestImmediateWhichIsLarger, CpuTestFixture) {
	code_gen().emit_ins16("00101nnniiiiiiii", registers::R0, 255);
	Step();
	ExpectThat().XPSRFlagsEquals("Nzcv");
}

MICROMACHINE_TEST_F(cmpImmediate, CompareRegisterToImmediateWhichWillGenerateNegativeOverflow, CpuTestFixture) {
	code_gen().emit_ins16("00101nnniiiiiiii", registers::R3, 1);
	getCpu().regs().set(registers::R3, 0x80000000);
	Step();
	ExpectThat().XPSRFlagsEquals("nzCV");
}
