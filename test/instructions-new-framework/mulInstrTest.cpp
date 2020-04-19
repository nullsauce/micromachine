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


/* MUL
   Encoding: 010000 1101 Rn:3 Rdm:3 */
TEST_F(CpuTestHarness, mul_UseLowestRegisterForAllArgs)
{
	code_gen().emit_ins16("0100001101nnnddd", registers::R0, registers::R0);
	setExpectedXPSRflags("nZ");
	setExpectedRegisterValue(registers::R0, 0U);
	step();
}

TEST_F(CpuTestHarness, mul_UseHigestRegisterForAllArgs_OnlyGetLower32bitsOfResult)
{
	code_gen().emit_ins16("0100001101nnnddd", registers::R7, registers::R7);
	setExpectedXPSRflags("Nz");
	setExpectedRegisterValue(registers::R7, 0x77777777U * 0x77777777U);
	step();
}

TEST_F(CpuTestHarness, mul_UseDifferentRegistersForEachArg)
{
	code_gen().emit_ins16("0100001101nnnddd", registers::R1, registers::R2);
	setRegisterValue(registers::R1, 0xA5A5);
	setRegisterValue(registers::R2, 2);
	setExpectedXPSRflags("nz");
	setExpectedRegisterValue(registers::R2, 0xA5A5U << 1U);
	step();
}

TEST_F(CpuTestHarness, mul_MultiplyBy16BitMaximumValues)
{
	code_gen().emit_ins16("0100001101nnnddd", registers::R1, registers::R2);
	setRegisterValue(registers::R1, 0xFFFF);
	setRegisterValue(registers::R2, 0xFFFF);
	setExpectedXPSRflags("Nz");
	setExpectedRegisterValue(registers::R2, 0xFFFE0001);
	step();
}
