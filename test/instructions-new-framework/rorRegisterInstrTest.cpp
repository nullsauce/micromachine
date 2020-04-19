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
	code_gen().emit_ins16("0100000111mmmddd", registers::R0, registers::R7);
	setExpectedXPSRflags("NzC");
	setRegisterValue(registers::R0, 1);
	setRegisterValue(registers::R7, 1);
	setExpectedRegisterValue(registers::R7, 0x80000000);
	step();
}

TEST_F(CpuTestHarness, rorRegister_Rotate1by0_MinimumShift_CarryUnmodified)
{
	code_gen().emit_ins16("0100000111mmmddd", registers::R7, registers::R0);
	setExpectedXPSRflags("nz");
	setRegisterValue(registers::R0, 1);
	setRegisterValue(registers::R7, 0);
	setExpectedRegisterValue(registers::R0, 1);
	step();
}

TEST_F(CpuTestHarness, rorRegister_Rotate2by1_NoCarry)
{
	code_gen().emit_ins16("0100000111mmmddd", registers::R3, registers::R2);
	setExpectedXPSRflags("nzc");
	setRegisterValue(registers::R2, 2);
	setRegisterValue(registers::R3, 1);
	setExpectedRegisterValue(registers::R2, 2 >> 1);
	step();
}

TEST_F(CpuTestHarness, rorRegister_Rotate16Bits)
{
	code_gen().emit_ins16("0100000111mmmddd", registers::R3, registers::R2);
	setExpectedXPSRflags("nzc");
	setRegisterValue(registers::R2, 0x12345678);
	setRegisterValue(registers::R3, 16);
	setExpectedRegisterValue(registers::R2, 0x56781234);
	step();
}

TEST_F(CpuTestHarness, rorRegister_RotateWithShiftOf31)
{
	code_gen().emit_ins16("0100000111mmmddd", registers::R3, registers::R2);
	setExpectedXPSRflags("nzc");
	setRegisterValue(registers::R2, 0x80000000);
	setRegisterValue(registers::R3, 31);
	setExpectedRegisterValue(registers::R2, 0x00000001);
	step();
}

TEST_F(CpuTestHarness, rorRegister_RotateBy32_CarryOutHighestBit)
{
	code_gen().emit_ins16("0100000111mmmddd", registers::R7, registers::R0);
	setExpectedXPSRflags("NzC");
	setRegisterValue(registers::R0, 0x80000000);
	setRegisterValue(registers::R7, 32);
	setExpectedRegisterValue(registers::R0, 0x80000000);
	step();
}

TEST_F(CpuTestHarness, rorRegister_RotateBy33)
{
	code_gen().emit_ins16("0100000111mmmddd", registers::R3, registers::R2);
	setExpectedXPSRflags("NzC");
	setRegisterValue(registers::R2, 0x80000001);
	setRegisterValue(registers::R3, 33);
	setExpectedRegisterValue(registers::R2, 0xC0000000);
	step();
}

TEST_F(CpuTestHarness, rorRegister_RotateWithMaximumShiftOf255)
{
	code_gen().emit_ins16("0100000111mmmddd", registers::R3, registers::R2);
	setExpectedXPSRflags("nzc");
	setRegisterValue(registers::R2, 0x80000000);
	setRegisterValue(registers::R3, 255);
	setExpectedRegisterValue(registers::R2, 0x00000001);
	step();
}

TEST_F(CpuTestHarness, rorRegister_RotateWithShiftOf256_ShouldBeTreatedAs0Shift_CarryUnmodified)
{
	code_gen().emit_ins16("0100000111mmmddd", registers::R7, registers::R0);
	setExpectedXPSRflags("Nz");
	setRegisterValue(registers::R0, 0x80000000);
	setRegisterValue(registers::R7, 256);
	setExpectedRegisterValue(registers::R0, 0x80000000);
	step();
}

TEST_F(CpuTestHarness, rorRegister_Rotate0by16)
{
	code_gen().emit_ins16("0100000111mmmddd", registers::R7, registers::R0);
	setExpectedXPSRflags("nZc");
	setRegisterValue(registers::R0, 0);
	setRegisterValue(registers::R7, 16);
	setExpectedRegisterValue(registers::R0, 0);
	step();
}
