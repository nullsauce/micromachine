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


/* TST - Register
   Encoding: 010000 1000 Rm:3 Rn:3 */
/* NOTE: APSR_C state is maintained by this instruction. */
TEST_F(CpuTestHarness, tstRegister_UseLowestRegisterForBothArgsAndResultShouldBeZero)
{
	code_gen().emit_ins16("0100001000mmmnnn", registers::R0, registers::R0);
	// Use a couple of tests to explicitly set/clear carry to verify both states are maintained.
	setExpectedXPSRflags("nZc");
	clearCarry();
	step();
}

TEST_F(CpuTestHarness, tstRegister_UseHighestRegisterForBothArgsAndRegisterWillBeUnchanged)
{
	code_gen().emit_ins16("0100001000mmmnnn", registers::R7, registers::R7);
	setExpectedXPSRflags("nzC");
	setCarry();
	step();
}

TEST_F(CpuTestHarness, tstRegister_AndR3andR7)
{
	code_gen().emit_ins16("0100001000mmmnnn", registers::R3, registers::R7);
	setExpectedXPSRflags("nz");
	step();
}

TEST_F(CpuTestHarness, tstRegister_UseAndToJustKeepNegativeSignBit)
{
	code_gen().emit_ins16("0100001000mmmnnn", registers::R7, registers::R0);
	setRegisterValue(registers::R0, -1);
	setRegisterValue(registers::R7, 0x80000000);
	setExpectedXPSRflags("Nz");
	step();
}
