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

#include "framework/pinkySimBaseTest.hpp"


/* LSR - Register (Logical Shift Right)
   Encoding: 010000 0011 Rm:3 Rdn:3 */
TEST_F(CpuTestHelper, lsrRegister_ShiftValue1by1_CarryOutFromLowestBit)
{
	emitInstruction16("0100000011mmmddd", R0, R7);
	setExpectedXPSRflags("nZC");
	setRegisterValue(R0, 1);
	setRegisterValue(R7, 1);
	setExpectedRegisterValue(R7, 0);
	step();
}

TEST_F(CpuTestHelper, lsrRegister_ShiftValue1by0_MinimumShift_CarryUnmodified)
{
	emitInstruction16("0100000011mmmddd", R0, R7);
	setExpectedXPSRflags("nzc");
	clearCarry();
	setRegisterValue(R7, 1);
	setRegisterValue(R0, 0);
	setExpectedRegisterValue(R7, 1);
	step();
}

TEST_F(CpuTestHelper, lsrRegister_ShiftValue2by1_NoCarry)
{
	emitInstruction16("0100000011mmmddd", R3, R2);
	setExpectedXPSRflags("nzc");
	setRegisterValue(R2, 2);
	setRegisterValue(R3, 1);
	setExpectedRegisterValue(R2, 2 >> 1);
	step();
}

TEST_F(CpuTestHelper, lsrRegister_ShiftNegativeValueBy31)
{
	emitInstruction16("0100000011mmmddd", R3, R2);
	setExpectedXPSRflags("nzC");
	setRegisterValue(R2, -1);
	setRegisterValue(R3, 31);
	setExpectedRegisterValue(R2, -1U >> 31);
	step();
}

TEST_F(CpuTestHelper, lsrRegister_ShiftNegativeValueBy32_CarryOutFromHighestBit)
{
	emitInstruction16("0100000011mmmddd", R7, R0);
	setExpectedXPSRflags("nZC");
	setRegisterValue(R0, 0x80000000);
	setRegisterValue(R7, 32);
	setExpectedRegisterValue(R0, 0);
	step();
}

TEST_F(CpuTestHelper, lsrRegister_ShiftNegativeValueBy33_ResultIsZero_CarryClear)
{
	emitInstruction16("0100000011mmmddd", R3, R2);
	setExpectedXPSRflags("nZc");
	setRegisterValue(R2, -1);
	setRegisterValue(R3, 33);
	setExpectedRegisterValue(R2, 0);
	step();
}

TEST_F(CpuTestHelper, lsrRegister_MaximumShiftOf255_ResultIsZero_CarryClear)
{
	emitInstruction16("0100000011mmmddd", R3, R2);
	setExpectedXPSRflags("nZc");
	setRegisterValue(R2, -1);
	setRegisterValue(R3, 255);
	setExpectedRegisterValue(R2, 0);
	step();
}

TEST_F(CpuTestHelper, lsrRegister_ShiftOf256_ShouldBeTreatedAs0Shift_CarryUnmodified)
{
	emitInstruction16("0100000011mmmddd", R7, R0);
	setExpectedXPSRflags("NzC");
	setCarry();
	setRegisterValue(R0, -1);
	setRegisterValue(R7, 256);
	setExpectedRegisterValue(R0, -1);
	step();
}
