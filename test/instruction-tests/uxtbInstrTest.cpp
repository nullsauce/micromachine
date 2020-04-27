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



/* UXTB (Unsigned ExTend Byte)
   Encoding: 1011 0010 11 Rm:3 Rd:3 */
MICROMACHINE_TEST_F(uxtb, ExtendLowestRegisterIntoHighestRegister_PositiveValue, CpuTestFixture) {
	code_gen().emit_ins16("1011001011mmmddd", registers::R7, registers::R0);
	getCpu().regs().set(registers::R7, 0x7F);
	Step();
	ExpectThat().Register(registers::R0).Equals(0x7F);
}

MICROMACHINE_TEST_F(uxtb, ExtendHighestRegisterIntoLowestRegister_NegativeValue, CpuTestFixture) {
	code_gen().emit_ins16("1011001011mmmddd", registers::R0, registers::R7);
	getCpu().regs().set(registers::R0, 0x80);
	Step();
	ExpectThat().Register(registers::R7).Equals(0x80);
}

MICROMACHINE_TEST_F(uxtb, OverwriteUpperBits_PositiveValue, CpuTestFixture) {
	code_gen().emit_ins16("1011001011mmmddd", registers::R6, registers::R1);
	getCpu().regs().set(registers::R6, 0xBADBAD7F);
	Step();
	ExpectThat().Register(registers::R1).Equals(0x7F);
}

MICROMACHINE_TEST_F(uxtb, OverwriteUpperBits_NegativeValue, CpuTestFixture) {
	code_gen().emit_ins16("1011001011mmmddd", registers::R2, registers::R5);
	getCpu().regs().set(registers::R2, 0xBADBAD80);
	Step();
	ExpectThat().Register(registers::R5).Equals(0x80);
}
