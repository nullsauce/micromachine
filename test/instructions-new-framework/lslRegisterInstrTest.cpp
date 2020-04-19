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
	code_gen().emit_ins16("0100000010mmmddd", registers::R0, registers::R7);
	setExpectedXPSRflags("nzC");
	setCarry();
	setExpectedRegisterValue(registers::R7, 0x77777777U);
	step();
}

TEST_F(CpuTestHarness, lslRegister_ShiftValue1by31_NegativeResult)
{
	code_gen().emit_ins16("0100000010mmmddd", registers::R4, registers::R3);
	setExpectedXPSRflags("Nzc");
	setRegisterValue(registers::R3, 1);
	setRegisterValue(registers::R4, 31);
	setExpectedRegisterValue(registers::R3, 1 << 31);
	step();
}

TEST_F(CpuTestHarness, lslRegister_ShiftValue1by32_CarryOutFromLowestBit)
{
	code_gen().emit_ins16("0100000010mmmddd", registers::R7, registers::R0);
	setExpectedXPSRflags("nZC");
	setRegisterValue(registers::R0, 1);
	setRegisterValue(registers::R7, 32);
	setExpectedRegisterValue(registers::R0, 0);
	step();
}

TEST_F(CpuTestHarness, lslRegister_ShiftNegativeValueBy1_CarryOutFromHighestBit)
{
	code_gen().emit_ins16("0100000010mmmddd", registers::R3, registers::R4);
	setExpectedXPSRflags("NzC");
	setRegisterValue(registers::R4, -1);
	setRegisterValue(registers::R3, 1);
	setExpectedRegisterValue(registers::R4, 0xffffffff << 1);
	step();
}

TEST_F(CpuTestHarness, lslRegister_ShiftValue1by33_NoCarry)
{
	code_gen().emit_ins16("0100000010mmmddd", registers::R7, registers::R0);
	setExpectedXPSRflags("nZc");
	setRegisterValue(registers::R0, 1);
	setRegisterValue(registers::R7, 33);
	setExpectedRegisterValue(registers::R0, 0);
	step();
}

TEST_F(CpuTestHarness, lslRegister_ShiftValuee1by255_MaximumShift)
{
	code_gen().emit_ins16("0100000010mmmddd", registers::R7, registers::R0);
	setExpectedXPSRflags("nZc");
	setRegisterValue(registers::R0, 1);
	setRegisterValue(registers::R7, 255);
	setExpectedRegisterValue(registers::R0, 0);
	step();
}

TEST_F(CpuTestHarness, lslRegister_ShiftValue1by256_ShouldBeTreatedAs0Shift_CarryUnmodified)
{
	code_gen().emit_ins16("0100000010mmmddd", registers::R7, registers::R0);
	setExpectedXPSRflags("nzc");
	clearCarry();
	setRegisterValue(registers::R0, 1);
	setRegisterValue(registers::R7, 256);
	setExpectedRegisterValue(registers::R0, 1);
	step();
}
