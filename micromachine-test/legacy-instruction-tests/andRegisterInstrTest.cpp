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


/* AND - Register
   Encoding: 010000 0000 Rm:3 Rdn:3 */
/* NOTE: APSR_C state is maintained by this instruction. */
TEST_F(CpuTestHarness, andRegister_UseLowestRegisterForBothArgs)
{
	code_gen().emit16bit("0100000000mmmddd", R0, R0);
	setExpectedXPSRflags("nZc");
	// Use a couple of tests to explicitly set/clear carry to verify both states are maintained.
	clearCarry();
	setExpectedRegisterValue(R0, 0);
	step();
}

TEST_F(CpuTestHarness, andRegister_UseHighestRegisterForBothArgs)
{
	code_gen().emit16bit("0100000000mmmddd", R7, R7);
	setExpectedXPSRflags("nzC");
	setCarry();
	step();
}

TEST_F(CpuTestHarness, andRegister_AndR3andR7)
{
	code_gen().emit16bit("0100000000mmmddd", R3, R7);
	setExpectedXPSRflags("nz");
	setExpectedRegisterValue(R7, 0x33333333);
	step();
}

TEST_F(CpuTestHarness, andRegister_UseAndToJustKeepNegativeSignBit)
{
	code_gen().emit16bit("0100000000mmmddd", R6, R1);
	setRegisterValue(R1, -1);
	setRegisterValue(R6, 0x80000000);
	setExpectedXPSRflags("Nz");
	setExpectedRegisterValue(R1, 0x80000000);
	step();
}

TEST_F(CpuTestHarness, andRegister_HaveAndResultNotBeSameAsEitherSource)
{
	code_gen().emit16bit("0100000000mmmddd", R5, R2);
	setRegisterValue(R2, 0x12345678);
	setRegisterValue(R5, 0xF0F0F0F0);
	setExpectedXPSRflags("nz");
	setExpectedRegisterValue(R2, 0x10305070);
	step();
}
