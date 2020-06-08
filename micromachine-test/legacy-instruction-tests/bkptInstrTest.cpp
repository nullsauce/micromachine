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


/* BKPT
   Encoding: 1011 1110 Imm:8 */

TEST_F(CpuTestHarness, bkpt_SmallestImmediate)
{
	code_gen().emit_ins16("10111110iiiiiiii", 0);
	setExpectedRegisterValue(registers::PC, INITIAL_PC);
	step();
}

TEST_F(CpuTestHarness, bkpt_LargestImmediate)
{
	code_gen().emit_ins16("10111110iiiiiiii", 255);
	setExpectedRegisterValue(registers::PC, INITIAL_PC);
	step();
}
