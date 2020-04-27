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


/* ROR - Register (ROtate Right)
   Encoding: 010000 0111 Rm:3 Rdn:3 */
TEST_F(CpuTestHarness, rorRegister_Rotate1by1_CarryOutFromLowestBit)
{
	code_gen().emit_ins16("0100000111mmmddd", R0, R7);
	setExpectedXPSRflags("NzC");
	setRegisterValue(R0, 1);
	setRegisterValue(R7, 1);
	setExpectedRegisterValue(R7, 0x80000000);
	step();
}

TEST_F(CpuTestHarness, rorRegister_Rotate1by0_MinimumShift_CarryUnmodified)
{
	code_gen().emit_ins16("0100000111mmmddd", R7, R0);
	setExpectedXPSRflags("nz");
	setRegisterValue(R0, 1);
	setRegisterValue(R7, 0);
	setExpectedRegisterValue(R0, 1);
	step();
}

TEST_F(CpuTestHarness, rorRegister_Rotate2by1_NoCarry)
{
	code_gen().emit_ins16("0100000111mmmddd", R3, R2);
	setExpectedXPSRflags("nzc");
	setRegisterValue(R2, 2);
	setRegisterValue(R3, 1);
	setExpectedRegisterValue(R2, 2 >> 1);
	step();
}

TEST_F(CpuTestHarness, rorRegister_Rotate16Bits)
{
	code_gen().emit_ins16("0100000111mmmddd", R3, R2);
	setExpectedXPSRflags("nzc");
	setRegisterValue(R2, 0x12345678);
	setRegisterValue(R3, 16);
	setExpectedRegisterValue(R2, 0x56781234);
	step();
}

TEST_F(CpuTestHarness, rorRegister_RotateWithShiftOf31)
{
	code_gen().emit_ins16("0100000111mmmddd", R3, R2);
	setExpectedXPSRflags("nzc");
	setRegisterValue(R2, 0x80000000);
	setRegisterValue(R3, 31);
	setExpectedRegisterValue(R2, 0x00000001);
	step();
}

TEST_F(CpuTestHarness, rorRegister_RotateBy32_CarryOutHighestBit)
{
	code_gen().emit_ins16("0100000111mmmddd", R7, R0);
	setExpectedXPSRflags("NzC");
	setRegisterValue(R0, 0x80000000);
	setRegisterValue(R7, 32);
	setExpectedRegisterValue(R0, 0x80000000);
	step();
}

TEST_F(CpuTestHarness, rorRegister_RotateBy33)
{
	code_gen().emit_ins16("0100000111mmmddd", R3, R2);
	setExpectedXPSRflags("NzC");
	setRegisterValue(R2, 0x80000001);
	setRegisterValue(R3, 33);
	setExpectedRegisterValue(R2, 0xC0000000);
	step();
}

TEST_F(CpuTestHarness, rorRegister_RotateWithMaximumShiftOf255)
{
	code_gen().emit_ins16("0100000111mmmddd", R3, R2);
	setExpectedXPSRflags("nzc");
	setRegisterValue(R2, 0x80000000);
	setRegisterValue(R3, 255);
	setExpectedRegisterValue(R2, 0x00000001);
	step();
}

TEST_F(CpuTestHarness, rorRegister_RotateWithShiftOf256_ShouldBeTreatedAs0Shift_CarryUnmodified)
{
	code_gen().emit_ins16("0100000111mmmddd", R7, R0);
	setExpectedXPSRflags("Nz");
	setRegisterValue(R0, 0x80000000);
	setRegisterValue(R7, 256);
	setExpectedRegisterValue(R0, 0x80000000);
	step();
}

TEST_F(CpuTestHarness, rorRegister_Rotate0by16)
{
	code_gen().emit_ins16("0100000111mmmddd", R7, R0);
	setExpectedXPSRflags("nZc");
	setRegisterValue(R0, 0);
	setRegisterValue(R7, 16);
	setExpectedRegisterValue(R0, 0);
	step();
}
