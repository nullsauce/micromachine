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


/* SUB registers::SP Minus Immediate
   Encoding: 1011 0000 1 Imm:7 */
TEST_F(CpuTestHarness, subSP_SmallestImmediate)
{
	code_gen().emit16bit("101100001iiiiiii", 0);
	setExpectedRegisterValue(registers::SP, INITIAL_SP - 0);
	step();
}

TEST_F(CpuTestHarness, subSP_LargestImmediate)
{
	code_gen().emit16bit("101100001iiiiiii", 127);
	setExpectedRegisterValue(registers::SP, INITIAL_SP - 127 * 4);
	step();
}

TEST_F(CpuTestHarness, subSP_UseIntermediateValues)
{
	code_gen().emit16bit("101100001iiiiiii", 64);
	setExpectedRegisterValue(registers::SP, INITIAL_SP - 64 * 4);
	step();
}
