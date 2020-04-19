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


/* ORR - Register
   Encoding: 010000 1100 Rm:3 Rdn:3 */
/* NOTE: APSR_C state is maintained by this instruction. */
TEST_F(CpuTestHarness, orrRegister_UseLowestRegisterForBothArgs)
{
	code_gen().emit_ins16("0100001100mmmddd", registers::R0, registers::R0);
	// Use a couple of tests to explicitly set/clear carry to verify both states are maintained.
	setExpectedXPSRflags("nZc");
	clearCarry();
	setExpectedRegisterValue(registers::R0, 0);
	step();
}

TEST_F(CpuTestHarness, orrRegister_UseHighestRegisterForBothArgs)
{
	code_gen().emit_ins16("0100001100mmmddd", registers::R7, registers::R7);
	setExpectedXPSRflags("nzC");
	setCarry();
	step();
}

TEST_F(CpuTestHarness, orrRegister_OrR3andR7)
{
	code_gen().emit_ins16("0100001100mmmddd", registers::R3, registers::R7);
	setExpectedXPSRflags("nz");
	setExpectedRegisterValue(registers::R7, 0x33333333 | 0x77777777);
	step();
}

TEST_F(CpuTestHarness, orrRegister_UseOrToTurnOnNegativeSignBit)
{
	code_gen().emit_ins16("0100001100mmmddd", registers::R7, registers::R0);
	setRegisterValue(registers::R0, 0x7FFFFFFF);
	setRegisterValue(registers::R7, 0x80000000);
	setExpectedXPSRflags("Nz");
	setExpectedRegisterValue(registers::R0, 0x7FFFFFFF | 0x80000000);
	step();
}

TEST_F(CpuTestHarness, orrRegister_HaveAndResultNotBeSameAsEitherSource)
{
	code_gen().emit_ins16("0100001100mmmddd", registers::R0, registers::R7);
	setRegisterValue(registers::R0, 0x12345678);
	setRegisterValue(registers::R7, 0xF0F0F0F0);
	setExpectedXPSRflags("Nz");
	setExpectedRegisterValue(registers::R7, 0xF2F4F6F8);
	step();
}
