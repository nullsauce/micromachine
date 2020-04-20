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


/* REV16
   Encoding: 1011 1010 01 Rm:3 Rd:3 */
MICROMACHINE_TEST_F(rev16, RevR0toR7, CpuTestFixture) {
	code_gen().emit_ins16("1011101001mmmddd", registers::R0, registers::R7);
	getCpu().regs().set(registers::R0, 0x12345678);
	Step();
	ExpectThat().Register(registers::R7).Equals(0x34127856);
}

MICROMACHINE_TEST_F(rev16, RevR7toR0, CpuTestFixture) {
	code_gen().emit_ins16("1011101001mmmddd", registers::R7, registers::R0);
	getCpu().regs().set(registers::R7, 0x12345678);
	Step();
	ExpectThat().Register(registers::R0).Equals(0x34127856);
}
