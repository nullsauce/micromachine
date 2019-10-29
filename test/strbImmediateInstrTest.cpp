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

/* STRB - Immediate
   Encoding: 011 1 0 Imm:5 Rn:3 Rt:3 */
TEST_F(CpuTestHelper, strbImmediate_UseAMixOfRegistersWordAlignedWithSmallestOffset)
{
	emitInstruction16("01110iiiiinnnttt", 0, R7, R0);
	setRegisterValue(R7, INITIAL_PC + 4);
	memory_write_32(m_context.pMemory, INITIAL_PC + 4, 0xBAADFEED, READ_WRITE);
	pinkySimStep(&m_context);
	EXPECT_EQ(0xBAADFE00, memory_read_32(m_context.pMemory, INITIAL_PC + 4));
}

TEST_F(CpuTestHelper, strbImmediate_UseAnotherMixOfRegistersSecondByteInWord)
{
	emitInstruction16("01110iiiiinnnttt", 1, R0, R7);
	setRegisterValue(R0, INITIAL_PC + 4);
	memory_write_32(m_context.pMemory, INITIAL_PC + 4, 0xBAADFEED, READ_WRITE);
	pinkySimStep(&m_context);
	EXPECT_EQ(0xBAAD77ED, memory_read_32(m_context.pMemory, INITIAL_PC + 4));
}

TEST_F(CpuTestHelper, strbImmediate_YetAnotherMixOfRegistersThirdByteInWord)
{
	emitInstruction16("01110iiiiinnnttt", 2, R3, R4);
	setRegisterValue(R3, INITIAL_PC + 4);
	memory_write_32(m_context.pMemory, INITIAL_PC + 4, 0xBAADFEED, READ_WRITE);
	pinkySimStep(&m_context);
	EXPECT_EQ(0xBA44FEED, memory_read_32(m_context.pMemory, INITIAL_PC + 4));
}

TEST_F(CpuTestHelper, strbImmediate_YetAnotherMixOfRegistersFourthByteInWord)
{
	emitInstruction16("01110iiiiinnnttt", 3, R1, R5);
	setRegisterValue(R1, INITIAL_PC + 4);
	memory_write_32(m_context.pMemory, INITIAL_PC + 4, 0xBAADFEED, READ_WRITE);
	pinkySimStep(&m_context);
	EXPECT_EQ(0x55ADFEED, memory_read_32(m_context.pMemory, INITIAL_PC + 4));
}

TEST_F(CpuTestHelper, strbImmediate_LargestOffset)
{
	emitInstruction16("01110iiiiinnnttt", 31, R2, R4);
	setRegisterValue(R2, INITIAL_PC);
	memory_write_32(m_context.pMemory, INITIAL_PC + 28, 0xBAADFEED, READ_WRITE);
	pinkySimStep(&m_context);
	EXPECT_EQ(0x44ADFEED, memory_read_32(m_context.pMemory, INITIAL_PC + 28));
}

TEST_F(CpuTestHelper, strbImmediate_AttemptStoreToInvalidAddress)
{
	emitInstruction16("01110iiiiinnnttt", 0, R3, R0);
	setRegisterValue(R3, 0xFFFFFFFC);
	setExpectedExceptionTaken(PINKYSIM_STEP_HARDFAULT);
	pinkySimStep(&m_context);
}
