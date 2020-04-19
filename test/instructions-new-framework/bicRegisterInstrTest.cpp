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


/* BIC - Register
   Encoding: 010000 1110 Rm:3 Rdn:3 */
/* NOTE: APSR_C state is maintained by this instruction. */
TEST_F(CpuTestHarness, bicRegister_UseLowestRegisterForBothArgs)
{
	code_gen().emit_ins16("0100001110mmmddd", registers::R0, registers::R0);
	// Use a couple of tests to explicitly set/clear carry to verify both states are maintained.
	setExpectedXPSRflags("nZc");
	clearCarry();
	setExpectedRegisterValue(registers::R0, 0);
	step();
}

TEST_F(CpuTestHarness, bicRegister_UseHighestRegisterForBothArgs)
{
	code_gen().emit_ins16("0100001110mmmddd", registers::R7, registers::R7);
	setExpectedXPSRflags("nZC");
	setCarry();
	setExpectedRegisterValue(registers::R7, 0);
	step();
}

TEST_F(CpuTestHarness, bicRegister_UseR3andR7)
{
	code_gen().emit_ins16("0100001110mmmddd", registers::R3, registers::R7);
	setExpectedXPSRflags("nz");
	setExpectedRegisterValue(registers::R7, 0x77777777 & ~0x33333333);
	step();
}

TEST_F(CpuTestHarness, bicRegister_UseBicToClearLSbit)
{
	code_gen().emit_ins16("0100001110mmmddd", registers::R6, registers::R1);
	setRegisterValue(registers::R1, -1);
	setRegisterValue(registers::R6, 1);
	setExpectedXPSRflags("Nz");
	setExpectedRegisterValue(registers::R1, -1U & ~1);
	step();
}
