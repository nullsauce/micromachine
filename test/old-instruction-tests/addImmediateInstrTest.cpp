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



/* ADD - Immediate - Encoding T1
   Encoding: 000 11 1 0 Imm:3 Rn:3 Rd:3 */
TEST_F(CpuTestHarness, addImmediate_T1UseLowestRegisterOnlyAddLargestImmediate)
{
	code_gen().emit_ins16("0001110iiinnnddd", 7, R0, R0);
	setExpectedXPSRflags("nzcv");
	setExpectedRegisterValue(R0, 0U + 7U);
	step();
}

TEST_F(CpuTestHarness, addImmediate_T1UseHigestRegisterOnlyAddSmallestImmediate)
{
	code_gen().emit_ins16("0001110iiinnnddd", 0, R7, R7);
	setExpectedXPSRflags("nzcv");
	setExpectedRegisterValue(R7, 0x77777777U + 0U);
	step();
}

TEST_F(CpuTestHarness, addImmediate_T1UseDifferentRegistersForEachArg)
{
	code_gen().emit_ins16("0001110iiinnnddd", 3, R7, R0);
	setExpectedXPSRflags("nzcv");
	setExpectedRegisterValue(R0, 0x77777777U + 3U);
	step();
}

TEST_F(CpuTestHarness, addImmediate_T1ForceCarryByAdding1ToLargestInteger)
{
	code_gen().emit_ins16("0001110iiinnnddd", 1, R6, R1);
	setExpectedXPSRflags("nZCv");
	setRegisterValue(R6, 0xFFFFFFFFU);
	setExpectedRegisterValue(R1, 0);
	step();
}

TEST_F(CpuTestHarness, addImmediate_T1ForceOverflowPastLargestPositiveInteger)
{
	code_gen().emit_ins16("0001110iiinnnddd", 1, R2, R5);
	setExpectedXPSRflags("NzcV");
	setRegisterValue(R2, 0x7FFFFFFFU);
	setExpectedRegisterValue(R5, 0x7FFFFFFFU + 1);
	step();
}



/* ADD - Immediate - Encoding T2
   Encoding: 001 10 Rdn:3 Imm:8 */
TEST_F(CpuTestHarness, addImmediate_T2UseLowestRegisterAndAddLargestImmediate)
{
	code_gen().emit_ins16("00110dddiiiiiiii", R0, 255);
	setExpectedXPSRflags("nzcv");
	setExpectedRegisterValue(R0, 0U + 255U);
	step();
}

TEST_F(CpuTestHarness, addImmediate_T2UseHigestRegisterAndAddSmallestImmediate)
{
	code_gen().emit_ins16("00110dddiiiiiiii", R7, 0);
	setExpectedXPSRflags("nzcv");
	setExpectedRegisterValue(R7, 0x77777777U + 0U);
	step();
}

TEST_F(CpuTestHarness, addImmediate_T2ForceCarryByAdding1ToLargestInteger)
{
	code_gen().emit_ins16("00110dddiiiiiiii", R3, 1);
	setExpectedXPSRflags("nZCv");
	setRegisterValue(R3, 0xFFFFFFFFU);
	setExpectedRegisterValue(R3, 0);
	step();
}

TEST_F(CpuTestHarness, addImmediate_T2ForceOverflowPastLargestPositiveInteger)
{
	code_gen().emit_ins16("00110dddiiiiiiii", R3, 1);
	setExpectedXPSRflags("NzcV");
	setRegisterValue(R3, 0x7FFFFFFFU);
	setExpectedRegisterValue(R3, 0x7FFFFFFFU + 1);
	step();
}
