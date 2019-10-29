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


/* LDRB - Immediate
   Encoding: 011 1 1 Imm:5 Rn:3 Rt:3 */
TEST_F(CpuTestHelper, ldrbImmediate_UseAMixOfRegistersWordAligned)
{
	emitInstruction16("01111iiiiinnnttt", 0, R7, R0);
	setRegisterValue(R7, INITIAL_PC + 4);
	memory_write_32(m_context.pMemory, INITIAL_PC + 4, 0xBAADFEED, READ_ONLY);
	setExpectedRegisterValue(R0, 0xED);
	pinkySimStep(&m_context);
}

TEST_F(CpuTestHelper, ldrbImmediate_UseAnotherMixOfRegistersSecondByteInWord)
{
	emitInstruction16("01111iiiiinnnttt", 1, R0, R7);
	setRegisterValue(R0, INITIAL_PC + 4);
	memory_write_32(m_context.pMemory, INITIAL_PC + 4, 0xBAADFEED, READ_ONLY);
	setExpectedRegisterValue(R7, 0xFE);
	pinkySimStep(&m_context);
}

TEST_F(CpuTestHelper, ldrbImmediate_YetAnotherMixOfRegistersThirdByteInWord)
{
	emitInstruction16("01111iiiiinnnttt", 2, R1, R4);
	setRegisterValue(R1, INITIAL_PC + 4);
	memory_write_32(m_context.pMemory, INITIAL_PC + 4, 0xBAADFEED, READ_ONLY);
	setExpectedRegisterValue(R4, 0xAD);
	pinkySimStep(&m_context);
}

TEST_F(CpuTestHelper, ldrbImmediate_YetAnotherMixOfRegistersFourthByteInWord)
{
	emitInstruction16("01111iiiiinnnttt", 3, R2, R5);
	setRegisterValue(R2, INITIAL_PC + 4);
	memory_write_32(m_context.pMemory, INITIAL_PC + 4, 0xBAADFEED, READ_ONLY);
	setExpectedRegisterValue(R5, 0xBA);
	pinkySimStep(&m_context);
}

TEST_F(CpuTestHelper, ldrbImmediate_UseLargestOffset)
{
	emitInstruction16("01111iiiiinnnttt", 31, R3, R0);
	setRegisterValue(R3, INITIAL_PC);
	memory_write_32(m_context.pMemory, INITIAL_PC + 28, 0x12345678, READ_ONLY);
	setExpectedRegisterValue(R0, 0x12);
	pinkySimStep(&m_context);
}

TEST_F(CpuTestHelper, ldrbImmediate_LoadAPositiveValue)
{
	emitInstruction16("01111iiiiinnnttt", 0, R3, R0);
	setRegisterValue(R3, INITIAL_PC + 4);
	memory_write_32(m_context.pMemory, INITIAL_PC + 4, 0xFFFFFF7F, READ_ONLY);
	setExpectedRegisterValue(R0, 0x7F);
	pinkySimStep(&m_context);
}

TEST_F(CpuTestHelper, ldrbImmediate_AttemptLoadInvalidAddress)
{
	emitInstruction16("01111iiiiinnnttt", 0, R3, R0);
	setRegisterValue(R3, 0xFFFFFFFC);
	setExpectedExceptionTaken(PINKYSIM_STEP_HARDFAULT);
	pinkySimStep(&m_context);
}
