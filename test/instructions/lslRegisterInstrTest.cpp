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


/* LSL - Register (Logical Shift Left)
   Encoding: 010000 0010 Rm:3 Rdn:3 */
TEST_F(CpuTestHarness, lslRegister_ShiftR7by0_MinimumShift_CarryShouldBeUnmodified)
{
	code_gen().emit_ins16("0100000010mmmddd", R0, R7);
	setExpectedXPSRflags("nzC");
	setCarry();
	setExpectedRegisterValue(R7, 0x77777777U);
	step();
}

TEST_F(CpuTestHarness, lslRegister_ShiftValue1by31_NegativeResult)
{
	code_gen().emit_ins16("0100000010mmmddd", R4, R3);
	setExpectedXPSRflags("Nzc");
	setRegisterValue(R3, 1);
	setRegisterValue(R4, 31);
	setExpectedRegisterValue(R3, 1 << 31);
	step();
}

TEST_F(CpuTestHarness, lslRegister_ShiftValue1by32_CarryOutFromLowestBit)
{
	code_gen().emit_ins16("0100000010mmmddd", R7, R0);
	setExpectedXPSRflags("nZC");
	setRegisterValue(R0, 1);
	setRegisterValue(R7, 32);
	setExpectedRegisterValue(R0, 0);
	step();
}

TEST_F(CpuTestHarness, lslRegister_ShiftNegativeValueBy1_CarryOutFromHighestBit)
{
	code_gen().emit_ins16("0100000010mmmddd", R3, R4);
	setExpectedXPSRflags("NzC");
	setRegisterValue(R4, -1);
	setRegisterValue(R3, 1);
	setExpectedRegisterValue(R4, 0xffffffff << 1);
	step();
}

TEST_F(CpuTestHarness, lslRegister_ShiftValue1by33_NoCarry)
{
	code_gen().emit_ins16("0100000010mmmddd", R7, R0);
	setExpectedXPSRflags("nZc");
	setRegisterValue(R0, 1);
	setRegisterValue(R7, 33);
	setExpectedRegisterValue(R0, 0);
	step();
}

TEST_F(CpuTestHarness, lslRegister_ShiftValuee1by255_MaximumShift)
{
	code_gen().emit_ins16("0100000010mmmddd", R7, R0);
	setExpectedXPSRflags("nZc");
	setRegisterValue(R0, 1);
	setRegisterValue(R7, 255);
	setExpectedRegisterValue(R0, 0);
	step();
}

TEST_F(CpuTestHarness, lslRegister_ShiftValue1by256_ShouldBeTreatedAs0Shift_CarryUnmodified)
{
	code_gen().emit_ins16("0100000010mmmddd", R7, R0);
	setExpectedXPSRflags("nzc");
	clearCarry();
	setRegisterValue(R0, 1);
	setRegisterValue(R7, 256);
	setExpectedRegisterValue(R0, 1);
	step();
}
