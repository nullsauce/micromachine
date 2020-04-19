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


/* ASR - Register (Arithmetic Shift Right)
   Encoding: 010000 0100 Rm:3 Rdn:3 */
TEST_F(CpuTestHarness, asrRegister_Shift1by1_CarryOutFromLowestBit)
{
	code_gen().emit_ins16("0100000100mmmddd", registers::R0, registers::R7);
	setRegisterValue(registers::R7, 1);
	setRegisterValue(registers::R0, 1);
	setExpectedXPSRflags("nZC");
	setExpectedRegisterValue(registers::R7, (int32_t) 1U >> 1U);
	step();
}

TEST_F(CpuTestHarness, asrRegister_Shift1by0_MinimumShift_CarryUnmodified)
{
	code_gen().emit_ins16("0100000100mmmddd", registers::R0, registers::R7);
	setRegisterValue(registers::R7, 1);
	setRegisterValue(registers::R0, 0);
	setExpectedXPSRflags("nz");
	setExpectedRegisterValue(registers::R7, (int32_t) 1 >> 0);
	step();
}

TEST_F(CpuTestHarness, asrRegister_Shift2by1_NoCarryFromLowestBit)
{
	code_gen().emit_ins16("0100000100mmmddd", registers::R3, registers::R2);
	setRegisterValue(registers::R2, 2);
	setRegisterValue(registers::R3, 1);
	setExpectedXPSRflags("nzc");
	setExpectedRegisterValue(registers::R2, (int32_t) 2 >> 1);
	step();
}

TEST_F(CpuTestHarness, asrRegister_ShiftNegativeNumberby31)
{
	code_gen().emit_ins16("0100000100mmmddd", registers::R3, registers::R2);
	setRegisterValue(registers::R2, -1);
	setRegisterValue(registers::R3, 31);
	setExpectedXPSRflags("NzC");
	setExpectedRegisterValue(registers::R2, (int32_t) -1 >> 31);
	step();
}

TEST_F(CpuTestHarness, asrRegister_ShiftMaximumNegativeValueBy32_CarryOutFromHighestBit)
{
	code_gen().emit_ins16("0100000100mmmddd", registers::R7, registers::R0);
	setRegisterValue(registers::R0, 0x80000000);
	setRegisterValue(registers::R7, 32);
	setExpectedXPSRflags("NzC");
	setExpectedRegisterValue(registers::R0, -1);
	step();
}

TEST_F(CpuTestHarness, asrRegister_ShiftNegativeValueby33)
{
	code_gen().emit_ins16("0100000100mmmddd", registers::R3, registers::R2);
	setRegisterValue(registers::R2, -1);
	setRegisterValue(registers::R3, 33);
	setExpectedXPSRflags("NzC");
	setExpectedRegisterValue(registers::R2, -1);
	step();
}

TEST_F(CpuTestHarness, asrRegister_ShiftPositiveValueby33)
{
	code_gen().emit_ins16("0100000100mmmddd", registers::R3, registers::R2);
	setRegisterValue(registers::R2, 0x7FFFFFFF);
	setRegisterValue(registers::R3, 33);
	setExpectedXPSRflags("nZc");
	setExpectedRegisterValue(registers::R2, 0);
	step();
}

TEST_F(CpuTestHarness, asrRegister_ShiftNegativeValueByMaximumShiftOf255)
{
	code_gen().emit_ins16("0100000100mmmddd", registers::R3, registers::R2);
	setRegisterValue(registers::R2, -1);
	setRegisterValue(registers::R3, 255);
	setExpectedXPSRflags("NzC");
	setExpectedRegisterValue(registers::R2, -1);
	step();
}

TEST_F(CpuTestHarness, asrRegister_ShiftOf256ShouldBeTreatedAsShiftOf0_CarryUnmodified)
{
	code_gen().emit_ins16("0100000100mmmddd", registers::R7, registers::R0);
	setRegisterValue(registers::R0, -1);
	setRegisterValue(registers::R7, 256);
	setExpectedXPSRflags("Nz");
	setExpectedRegisterValue(registers::R0, (int32_t) -1 >> 0);
	step();
}

TEST_F(CpuTestHarness, asrRegister_ShiftLargestPositiveNumberBy31)
{
	code_gen().emit_ins16("0100000100mmmddd", registers::R2, registers::R3);
	setRegisterValue(registers::R3, 0x7FFFFFFF);
	setRegisterValue(registers::R2, 31);
	setExpectedXPSRflags("nZC");
	setExpectedRegisterValue(registers::R3, (int32_t) 0x7FFFFFFF >> 31);
	step();
}

TEST_F(CpuTestHarness, asrRegister_ShiftLargestNegativeNumberBy1)
{
	code_gen().emit_ins16("0100000100mmmddd", registers::R2, registers::R3);
	setRegisterValue(registers::R3, 0x80000000);
	setRegisterValue(registers::R2, 1);
	setExpectedXPSRflags("Nzc");
	setExpectedRegisterValue(registers::R3, (int32_t) 0x80000000 >> 1);
	step();
}
