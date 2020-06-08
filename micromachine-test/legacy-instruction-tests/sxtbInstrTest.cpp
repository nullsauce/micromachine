/*  Copyright (C) 2014  Adam Green (https://github.com/adamgreen): Original implementation
    Copyright (C) 2019  Flavio Roth (https://github.com/flavioroth): Adaptation

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
	code_gen().emit16bit("1011001001mmmddd", R7, R0);
	setRegisterValue(R7, 0x7F);
	setExpectedRegisterValue(R0, 0x7F);
	step();
}

TEST_F(CpuTestHarness, sxtb_SignExtendHighestRegisterIntoLowestRegister_NegativeValue)
{
	code_gen().emit16bit("1011001001mmmddd", R0, R7);
	setRegisterValue(R0, 0x80);
	setExpectedRegisterValue(R7, 0xFFFFFF80);
	step();
}

TEST_F(CpuTestHarness, sxtb_OverwriteUpperBits_PositiveValue)
{
	code_gen().emit16bit("1011001001mmmddd", R6, R1);
	setRegisterValue(R6, 0xBADBAD7F);
	setExpectedRegisterValue(R1, 0x7F);
	step();
}

TEST_F(CpuTestHarness, sxtb_OverwriteUpperBits_NegativeValue)
{
	code_gen().emit16bit("1011001001mmmddd", R2, R5);
	setRegisterValue(R2, 0xBADBAD80);
	setExpectedRegisterValue(R5, 0xFFFFFF80);
	step();
}
