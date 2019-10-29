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


/* LDRSB - Register
   Encoding: 0101 011 Rm:3 Rn:3 Rt:3 */
TEST_F(CpuTestHelper, ldrsbRegister_UseAMixOfRegistersWordAligned_NegativeValue)
{
	emitInstruction16("0101011mmmnnnttt", R7, R3, R0);
	setRegisterValue(R3, INITIAL_PC);
	setRegisterValue(R7, 4);
	memory_write_32(INITIAL_PC + 4, 0xBAADFEED);
	setExpectedRegisterValue(R0, 0xFFFFFFED);
	step();
}

TEST_F(CpuTestHelper, ldrsbRegister_UseAnotherMixOfRegistersSecondByteInWord_NegativeValue)
{
	emitInstruction16("0101011mmmnnnttt", R1, R0, R7);
	setRegisterValue(R0, INITIAL_PC);
	setRegisterValue(R1, 5);
	memory_write_32(INITIAL_PC + 4, 0xBAADFEED);
	setExpectedRegisterValue(R7, 0xFFFFFFFE);
	step();
}

TEST_F(CpuTestHelper, ldrsbRegister_YetAnotherMixOfRegistersThirdByteInWord_NegativeValue)
{
	emitInstruction16("0101011mmmnnnttt", R0, R7, R4);
	setRegisterValue(R7, INITIAL_PC);
	setRegisterValue(R0, 6);
	memory_write_32(INITIAL_PC + 4, 0xBAADFEED);
	setExpectedRegisterValue(R4, 0xFFFFFFAD);
	step();
}

TEST_F(CpuTestHelper, ldrsbRegister_YetAnotherMixOfRegistersFourthByteInWord_NegativeValue)
{
	emitInstruction16("0101011mmmnnnttt", R0, R7, R5);
	setRegisterValue(R7, INITIAL_PC);
	setRegisterValue(R0, 7);
	memory_write_32(INITIAL_PC + 4, 0xBAADFEED);
	setExpectedRegisterValue(R5, 0xFFFFFFBA);
	step();
}

TEST_F(CpuTestHelper, ldrsbRegister_LoadAPositiveValue)
{
	emitInstruction16("0101011mmmnnnttt", R7, R3, R0);
	setRegisterValue(R3, INITIAL_PC);
	setRegisterValue(R7, 4);
	memory_write_32(INITIAL_PC + 4, 0xFFFFFF7F);
	setExpectedRegisterValue(R0, 0x7F);
	step();
}

TEST_F(CpuTestHelper, ldrsbRegister_AttemptLoadInvalidAddress)
{
	emitInstruction16("0101011mmmnnnttt", R7, R3, R0);
	setRegisterValue(R3, 0xFFFFFFFC);
	setRegisterValue(R7, 0);
	setExpectedExceptionTaken(PINKYSIM_STEP_HARDFAULT);
	step();
}
