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


/* MVN - Register (MOve Negative)
   Encoding: 010000 1111 Rm:3 Rd:3 */
/* NOTE: APSR_C state is maintained by this instruction. */
TEST_F(CpuTestHarness, mvnRegister_UseLowestRegisterForAllArgs)
{
	// Use a couple of tests to explicitly set/clear carry to verify both states are maintained.
	code_gen().emit_ins16("0100001111mmmddd", registers::R0, registers::R0);
	setExpectedXPSRflags("NzC");
	setCarry();
	setExpectedRegisterValue(registers::R0, ~0U);
	step();
}

TEST_F(CpuTestHarness, mvnRegister_UseHigestRegisterForAllArgs)
{
	code_gen().emit_ins16("0100001111mmmddd", registers::R7, registers::R7);
	setExpectedXPSRflags("Nzc");
	clearCarry();
	setExpectedRegisterValue(registers::R7, ~0x77777777U);
	step();
}

TEST_F(CpuTestHarness, mvnRegister_UseDifferentRegistersForEachArg)
{
	code_gen().emit_ins16("0100001111mmmddd", registers::R2, registers::R1);
	setExpectedXPSRflags("Nz");
	setExpectedRegisterValue(registers::R1, ~0x22222222U);
	step();
}

TEST_F(CpuTestHarness, mvnRegister_MoveANegationOfNegativeOne_ClearsNegativeFlagAndSetsZeroFlag)
{
	code_gen().emit_ins16("0100001111mmmddd", registers::R2, registers::R1);
	setRegisterValue(registers::R2, -1);
	setExpectedXPSRflags("nZ");
	setExpectedRegisterValue(registers::R1, 0U);
	step();
}
