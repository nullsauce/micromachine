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


/* UXTH (Unsigned ExTend Halfword)
   Encoding: 1011 0010 10 Rm:3 Rd:3 */
TEST_F(CpuTestHarness, uxth_ExtendLowestRegisterIntoHighestRegister_PositiveValue)
{
	code_gen().emit_ins16("1011001010mmmddd", registers::R7, registers::R0);
	setRegisterValue(registers::R7, 0x7FFF);
	setExpectedRegisterValue(registers::R0, 0x7FFF);
	step();
}

TEST_F(CpuTestHarness, uxth_ExtendHighestRegisterIntoLowestRegister_NegativeValue)
{
	code_gen().emit_ins16("1011001010mmmddd", registers::R0, registers::R7);
	setRegisterValue(registers::R0, 0x8000);
	setExpectedRegisterValue(registers::R7, 0x8000);
	step();
}

TEST_F(CpuTestHarness, uxth_OverwriteUpperBits_PositiveValue)
{
	code_gen().emit_ins16("1011001010mmmddd", registers::R6, registers::R1);
	setRegisterValue(registers::R6, 0xF00D7FFF);
	setExpectedRegisterValue(registers::R1, 0x7FFF);
	step();
}

TEST_F(CpuTestHarness, uxth_OverwriteUpperBits_NegativeValue)
{
	code_gen().emit_ins16("1011001010mmmddd", registers::R2, registers::R5);
	setRegisterValue(registers::R2, 0xF00D8000);
	setExpectedRegisterValue(registers::R5, 0x8000);
	step();
}
