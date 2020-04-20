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


/* MUL
   Encoding: 010000 1101 Rn:3 Rdm:3 */
MICROMACHINE_TEST_F(mul, UseLowestRegisterForAllArgs, CpuTestFixture) {
	code_gen().emit_ins16("0100001101nnnddd", registers::R0, registers::R0);
	Step();
	ExpectThat().XPSRFlagsEquals("nZ");
	ExpectThat().Register(registers::R0).Equals(0U);
}

MICROMACHINE_TEST_F(mul, UseHigestRegisterForAllArgs_OnlyGetLower32bitsOfResult, CpuTestFixture) {
	getCpu().regs().set(registers::R7, 0x77777777U);
	code_gen().emit_ins16("0100001101nnnddd", registers::R7, registers::R7);
	Step();
	ExpectThat().XPSRFlagsEquals("Nz");
	ExpectThat().Register(registers::R7).Equals(0x77777777U * 0x77777777U);
}

MICROMACHINE_TEST_F(mul, UseDifferentRegistersForEachArg, CpuTestFixture) {
	code_gen().emit_ins16("0100001101nnnddd", registers::R1, registers::R2);
	getCpu().regs().set(registers::R1, 0xA5A5);
	getCpu().regs().set(registers::R2, 2);
	Step();
	ExpectThat().XPSRFlagsEquals("nz");
	ExpectThat().Register(registers::R2).Equals(0xA5A5U << 1U);
}

MICROMACHINE_TEST_F(mul, MultiplyBy16BitMaximumValues, CpuTestFixture) {
	code_gen().emit_ins16("0100001101nnnddd", registers::R1, registers::R2);
	getCpu().regs().set(registers::R1, 0xFFFF);
	getCpu().regs().set(registers::R2, 0xFFFF);
	Step();
	ExpectThat().XPSRFlagsEquals("Nz");
	ExpectThat().Register(registers::R2).Equals(0xFFFE0001);
}
