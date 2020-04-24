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


/* SUB - Immediate - Encoding T1
   Encoding: 000 11 1 1 Imm:3 Rn:3 Rd:3 */
TEST_F(CpuTestHarness, subImmediate_T1UseLowestRegisterOnly_SmallestImmediate)
{
	code_gen().emit16bit("0001111iiinnnddd", 0, R0, R0);
	setExpectedXPSRflags("nZCv");
	setExpectedRegisterValue(R0, 0U);
	step();
}

TEST_F(CpuTestHarness, subImmediate_T1UseHigestRegisterOnly_LargestImmediate)
{
	code_gen().emit16bit("0001111iiinnnddd", 7, R7, R7);
	setExpectedXPSRflags("nzCv");
	setExpectedRegisterValue(R7, 0x77777777U - 7U);
	step();
}

TEST_F(CpuTestHarness, subImmediate_T1UseDifferentRegistersForEachArg)
{
	code_gen().emit16bit("0001111iiinnnddd", 3, R0, R2);
	setExpectedXPSRflags("Nzcv");
	setExpectedRegisterValue(R2, 0U - 3U);
	step();
}

TEST_F(CpuTestHarness, subImmediate_T1ForceOverflowPastLargestNegativeInteger)
{
	code_gen().emit16bit("0001111iiinnnddd", 1, R1, R6);
	setExpectedXPSRflags("nzCV");
	setRegisterValue(R1, 0x80000000);
	setExpectedRegisterValue(R6, 0x80000000U - 1U);
	step();
}



/* SUB - Immediate - Encoding T2
   Encoding: 001 11 Rdn:3 Imm:8 */
TEST_F(CpuTestHarness, subImmediate_T2LowestRegister_SmallestImmediate)
{
	code_gen().emit16bit("00111dddiiiiiiii", R0, 0);
	setExpectedXPSRflags("nZCv");
	setExpectedRegisterValue(R0, 0U);
	step();
}

TEST_F(CpuTestHarness, subImmediate_T2HigestRegister_LargestImmediate)
{
	code_gen().emit16bit("00111dddiiiiiiii", R7, 255);
	setExpectedXPSRflags("nzCv");
	setExpectedRegisterValue(R7, 0x77777777U - 255U);
	step();
}

TEST_F(CpuTestHarness, subImmediate_T2Subtract127FromR0CausesNoCarryToIndicateBorrowAndNegativeResult)
{
	code_gen().emit16bit("00111dddiiiiiiii", R0, 127);
	setExpectedXPSRflags("Nzcv");
	setExpectedRegisterValue(R0, 0U - 127U);
	step();
}

TEST_F(CpuTestHarness, subImmediate_T2ForceOverflowPastLargestNegativeInteger)
{
	code_gen().emit16bit("00111dddiiiiiiii", R3, 1);
	setExpectedXPSRflags("nzCV");
	setRegisterValue(R3, 0x80000000);
	setExpectedRegisterValue(R3, 0x80000000U - 1U);
	step();
}
