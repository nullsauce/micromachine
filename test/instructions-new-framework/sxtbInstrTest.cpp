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


/* SXTB (Sign ExTend Byte)
   Encoding: 1011 0010 01 Rm:3 Rd:3 */
TEST_F(CpuTestHarness, sxtb_SignExtendLowestRegisterIntoHighestRegister_PositiveValue)
{
	code_gen().emit_ins16("1011001001mmmddd", registers::R7, registers::R0);
	setRegisterValue(registers::R7, 0x7F);
	setExpectedRegisterValue(registers::R0, 0x7F);
	step();
}

TEST_F(CpuTestHarness, sxtb_SignExtendHighestRegisterIntoLowestRegister_NegativeValue)
{
	code_gen().emit_ins16("1011001001mmmddd", registers::R0, registers::R7);
	setRegisterValue(registers::R0, 0x80);
	setExpectedRegisterValue(registers::R7, 0xFFFFFF80);
	step();
}

TEST_F(CpuTestHarness, sxtb_OverwriteUpperBits_PositiveValue)
{
	code_gen().emit_ins16("1011001001mmmddd", registers::R6, registers::R1);
	setRegisterValue(registers::R6, 0xBADBAD7F);
	setExpectedRegisterValue(registers::R1, 0x7F);
	step();
}

TEST_F(CpuTestHarness, sxtb_OverwriteUpperBits_NegativeValue)
{
	code_gen().emit_ins16("1011001001mmmddd", registers::R2, registers::R5);
	setRegisterValue(registers::R2, 0xBADBAD80);
	setExpectedRegisterValue(registers::R5, 0xFFFFFF80);
	step();
}
