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

#include "CpuTestHarness.hpp"


/* REVSH
   Encoding: 1011 1010 11 Rm:3 Rd:3 */
TEST_F(CpuTestHarness, revsh_RevR0toR7)
{
	code_gen().emit_ins16("1011101011mmmddd", registers::R0, registers::R7);
	setRegisterValue(registers::R0, 0x12345678);
	setExpectedRegisterValue(registers::R7, 0x7856);
	step();
}

TEST_F(CpuTestHarness, revsh_RevR7toR0)
{
	code_gen().emit_ins16("1011101011mmmddd", registers::R7, registers::R0);
	setRegisterValue(registers::R7, 0x12345678);
	setExpectedRegisterValue(registers::R0, 0x7856);
	step();
}

TEST_F(CpuTestHarness, revsh_PositiveValue)
{
	code_gen().emit_ins16("1011101011mmmddd", registers::R7, registers::R0);
	setRegisterValue(registers::R7, 0xFF7F);
	setExpectedRegisterValue(registers::R0, 0x7FFF);
	step();
}

TEST_F(CpuTestHarness, revsh_NegativeValue)
{
	code_gen().emit_ins16("1011101011mmmddd", registers::R7, registers::R0);
	setRegisterValue(registers::R7, 0x0080);
	setExpectedRegisterValue(registers::R0, 0xFFFF8000);
	step();
}
