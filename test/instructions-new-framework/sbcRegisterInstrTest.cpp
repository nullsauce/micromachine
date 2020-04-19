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


/* SBC - Register (SUBtract with Carry)
   Encoding: 010000 0110 Rm:3 Rdn:3 */
TEST_F(CpuTestHarness, sbcRegister_UseLowestRegisterForAllArgsAndShouldBeZeroWithCarrySetForNoBorrow)
{
	code_gen().emit_ins16("0100000110mmmddd", registers::R0, registers::R0);
	setExpectedXPSRflags("nZCv");
	setExpectedRegisterValue(registers::R0, 0);
	setCarry();
	step();
}

TEST_F(CpuTestHarness, sbcRegister_UseHigestRegisterForAllArgsAndShouldBeZeroWithCarrySetForNoBorrow)
{
	code_gen().emit_ins16("0100000110mmmddd", registers::R7, registers::R7);
	setExpectedXPSRflags("nZCv");
	setExpectedRegisterValue(registers::R7, 0);
	setCarry();
	step();
}

TEST_F(CpuTestHarness,
	   sbcRegister_UseDifferentRegistersForEachArgAndOnlyCarryShouldBeSetToIndicateNoBorrow)
{
	code_gen().emit_ins16("0100000110mmmddd", registers::R1, registers::R2);
	setExpectedXPSRflags("nzCv");
	setExpectedRegisterValue(registers::R2, 0x22222222U - 0x11111111U);
	setCarry();
	step();
}

TEST_F(CpuTestHarness, sbcRegister_ForceCarryClearToIndicateBorrowAndResultWillBeNegative)
{
	code_gen().emit_ins16("0100000110mmmddd", registers::R1, registers::R0);
	setExpectedXPSRflags("Nzcv");
	setRegisterValue(registers::R1, 1);
	setExpectedRegisterValue(registers::R0, 0U - 1U);
	setCarry();
	step();
}

TEST_F(CpuTestHarness, sbcRegister_ForceNegativeOverflow)
{
	code_gen().emit_ins16("0100000110mmmddd", registers::R1, registers::R2);
	setExpectedXPSRflags("nzCV");
	setRegisterValue(registers::R2, 0x80000000U);
	setRegisterValue(registers::R1, 1U);
	setExpectedRegisterValue(registers::R2, (int32_t) 0x80000000U - 1U);
	setCarry();
	step();
}

TEST_F(CpuTestHarness, sbcRegister_ForcePositiveOverflow)
{
	code_gen().emit_ins16("0100000110mmmddd", registers::R1, registers::R2);
	setExpectedXPSRflags("NzcV");
	setRegisterValue(registers::R2, 0x7FFFFFFFU);
	setRegisterValue(registers::R1, -1U);
	setExpectedRegisterValue(registers::R2, 0x7FFFFFFF + 1U);
	setCarry();
	step();
}

TEST_F(CpuTestHarness, sbcRegister_ClearCarryToCauseABorrowToOccur)
{
	code_gen().emit_ins16("0100000110mmmddd", registers::R1, registers::R2);
	setExpectedXPSRflags("nzCv");
	setExpectedRegisterValue(registers::R2, 0x22222222U - 1U - 0x11111111U);
	clearCarry(); // Causes borrow.
	step();
}
