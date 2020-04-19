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


/* CMP - Immediate
   Encoding: 001 01 Rn:3 Imm:8 */
TEST_F(CpuTestHarness, cmpImmediate_CompareLowestRegisterToEqualValue)
{
	code_gen().emit_ins16("00101nnniiiiiiii", registers::R0, 0);
	setExpectedXPSRflags("nZCv");
	step();
}

TEST_F(CpuTestHarness, cmpImmediate_CompareHighestRegisterToImmediateWhichIsSmaller)
{
	code_gen().emit_ins16("00101nnniiiiiiii", registers::R7, 127);
	setExpectedXPSRflags("nzCv");
	step();
}

TEST_F(CpuTestHarness, cmpImmediate_CompareRegisterToLargestImmediateWhichIsLarger)
{
	code_gen().emit_ins16("00101nnniiiiiiii", registers::R0, 255);
	setExpectedXPSRflags("Nzcv");
	step();
}

TEST_F(CpuTestHarness, cmpImmediate_CompareRegisterToImmediateWhichWillGenerateNegativeOverflow)
{
	code_gen().emit_ins16("00101nnniiiiiiii", registers::R3, 1);
	setRegisterValue(registers::R3, 0x80000000);
	setExpectedXPSRflags("nzCV");
	step();
}
