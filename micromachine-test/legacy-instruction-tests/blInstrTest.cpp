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


/* BL (Branch with Link)
   Encoding: 11110 S Imm:10
             11 J1:1 1 J2:1 Imm:11
    Note: J1 and J2 are translated to immediate bits via I? = NOT(J? XOR S) */
TEST_F(CpuTestHarness, bl_OffsetOf0)
{
	code_gen().emit32bit("11110Siiiiiiiiii", "11j1kiiiiiiiiiii", 0, 0, 1, 1, 0);
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	setExpectedRegisterValue(registers::LR, (INITIAL_PC + 4) | 1);
	step();
}

TEST_F(CpuTestHarness, bl_MaximumPositiveOffset)
{
	code_gen().emit32bit("11110Siiiiiiiiii", "11j1kiiiiiiiiiii", 0, 0x3FF, 0, 0, 0x7FF);
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4 + 16777214);
	setExpectedRegisterValue(registers::LR, (INITIAL_PC + 4) | 1);
	step();
}

TEST_F(CpuTestHarness, bl_MaximumNegativeOffset)
{
	code_gen().emit32bit("11110Siiiiiiiiii", "11j1kiiiiiiiiiii", 1, 0, 0, 0, 0);
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4 - 16777216);
	setExpectedRegisterValue(registers::LR, (INITIAL_PC + 4) | 1);
	step();
}
