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


/* LSR - Register (Logical Shift Right)
   Encoding: 010000 0011 Rm:3 Rdn:3 */
TEST_F(CpuTestHarness, lsrRegister_ShiftValue1by1_CarryOutFromLowestBit)
{
	code_gen().emit_ins16("0100000011mmmddd", registers::R0, registers::R7);
	setExpectedXPSRflags("nZC");
	setRegisterValue(registers::R0, 1);
	setRegisterValue(registers::R7, 1);
	setExpectedRegisterValue(registers::R7, 0);
	step();
}

TEST_F(CpuTestHarness, lsrRegister_ShiftValue1by0_MinimumShift_CarryUnmodified)
{
	code_gen().emit_ins16("0100000011mmmddd", registers::R0, registers::R7);
	setExpectedXPSRflags("nzc");
	clearCarry();
	setRegisterValue(registers::R7, 1);
	setRegisterValue(registers::R0, 0);
	setExpectedRegisterValue(registers::R7, 1);
	step();
}

TEST_F(CpuTestHarness, lsrRegister_ShiftValue2by1_NoCarry)
{
	code_gen().emit_ins16("0100000011mmmddd", registers::R3, registers::R2);
	setExpectedXPSRflags("nzc");
	setRegisterValue(registers::R2, 2);
	setRegisterValue(registers::R3, 1);
	setExpectedRegisterValue(registers::R2, 2 >> 1);
	step();
}

TEST_F(CpuTestHarness, lsrRegister_ShiftNegativeValueBy31)
{
	code_gen().emit_ins16("0100000011mmmddd", registers::R3, registers::R2);
	setExpectedXPSRflags("nzC");
	setRegisterValue(registers::R2, -1);
	setRegisterValue(registers::R3, 31);
	setExpectedRegisterValue(registers::R2, -1U >> 31);
	step();
}

TEST_F(CpuTestHarness, lsrRegister_ShiftNegativeValueBy32_CarryOutFromHighestBit)
{
	code_gen().emit_ins16("0100000011mmmddd", registers::R7, registers::R0);
	setExpectedXPSRflags("nZC");
	setRegisterValue(registers::R0, 0x80000000);
	setRegisterValue(registers::R7, 32);
	setExpectedRegisterValue(registers::R0, 0);
	step();
}

TEST_F(CpuTestHarness, lsrRegister_ShiftNegativeValueBy33_ResultIsZero_CarryClear)
{
	code_gen().emit_ins16("0100000011mmmddd", registers::R3, registers::R2);
	setExpectedXPSRflags("nZc");
	setRegisterValue(registers::R2, -1);
	setRegisterValue(registers::R3, 33);
	setExpectedRegisterValue(registers::R2, 0);
	step();
}

TEST_F(CpuTestHarness, lsrRegister_MaximumShiftOf255_ResultIsZero_CarryClear)
{
	code_gen().emit_ins16("0100000011mmmddd", registers::R3, registers::R2);
	setExpectedXPSRflags("nZc");
	setRegisterValue(registers::R2, -1);
	setRegisterValue(registers::R3, 255);
	setExpectedRegisterValue(registers::R2, 0);
	step();
}

TEST_F(CpuTestHarness, lsrRegister_ShiftOf256_ShouldBeTreatedAs0Shift_CarryUnmodified)
{
	code_gen().emit_ins16("0100000011mmmddd", registers::R7, registers::R0);
	setExpectedXPSRflags("NzC");
	setCarry();
	setRegisterValue(registers::R0, -1);
	setRegisterValue(registers::R7, 256);
	setExpectedRegisterValue(registers::R0, -1);
	step();
}
