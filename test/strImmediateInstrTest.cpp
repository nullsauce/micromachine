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


/* STR - Immediate Encoding T1
   Encoding: 011 0 0 Imm:5 Rn:3 Rt:3 */
TEST_F(pinkySimBase, strImmediate_T1UseAMixOfRegistersWithSmallestImmediateOffset)
{
	emitInstruction16("01100iiiiinnnttt", 0, R7, R0);
	setRegisterValue(R7, INITIAL_PC + 4);
	memory_write_32(m_context.pMemory, INITIAL_PC + 4, 0xBAADFEED, READ_WRITE);
	pinkySimStep(&m_context);
	EXPECT_EQ(0x00000000, memory_read_32(m_context.pMemory, INITIAL_PC + 4));
}

TEST_F(pinkySimBase, strImmediate_T1UseAnotherMixOfRegistersWithLargestImmediateOffset)
{
	emitInstruction16("01100iiiiinnnttt", 31, R0, R7);
	setRegisterValue(R0, INITIAL_PC);
	memory_write_32(m_context.pMemory, INITIAL_PC + 31 * 4, 0xBAADFEED, READ_WRITE);
	pinkySimStep(&m_context);
	EXPECT_EQ(0x77777777, memory_read_32(m_context.pMemory, INITIAL_PC + 31 * 4));
}

TEST_F(pinkySimBase, strImmediate_T1AttemptUnalignedStore)
{
	emitInstruction16("01100iiiiinnnttt", 0, R3, R2);
	setRegisterValue(R3, INITIAL_PC + 2);
	setExpectedExceptionTaken(PINKYSIM_STEP_HARDFAULT);
	pinkySimStep(&m_context);
}

TEST_F(pinkySimBase, strImmediate_T1AttemptStoreToInvalidAddress)
{
	emitInstruction16("01100iiiiinnnttt", 16, R3, R2);
	setRegisterValue(R3, 0xFFFFFFFC - 16 * 4);
	setExpectedExceptionTaken(PINKYSIM_STEP_HARDFAULT);
	pinkySimStep(&m_context);
}



/* STR - Immediate Encoding T2 (SP is base register)
   Encoding: 1001 0 Rt:3 Imm:8 */
TEST_F(pinkySimBase, strImmediate_T2HighestRegisterWithSmallestImmediateOffset)
{
	emitInstruction16("10010tttiiiiiiii", R7, 0);
	setRegisterValue(SP, INITIAL_PC + 1024);
	memory_write_32(m_context.pMemory, INITIAL_PC + 1024, 0xBAADFEED, READ_WRITE);
	pinkySimStep(&m_context);
	EXPECT_EQ(0x77777777, memory_read_32(m_context.pMemory, INITIAL_PC + 1024));
}

TEST_F(pinkySimBase, strImmediate_T2LowestRegisterWithLargestImmediateOffset)
{
	emitInstruction16("10010tttiiiiiiii", R0, 255);
	setRegisterValue(SP, INITIAL_PC + 1024);
	memory_write_32(m_context.pMemory, INITIAL_PC + 1024 + 255 * 4, 0xBAADFEED, READ_WRITE);
	pinkySimStep(&m_context);
	EXPECT_EQ(0x00000000, memory_read_32(m_context.pMemory, INITIAL_PC + 1024 + 255 * 4));
}
/*
TEST_SIM_ONLY(strImmediate, T2AttemptUnalignedStore)
{
    emitInstruction16("10010tttiiiiiiii", R2, 0);
    setRegisterValue(SP, INITIAL_PC + 1026);
    setExpectedExceptionHandled(PINKYSIM_STEP_HARDFAULT);
    setExpectedRegisterValue(PC, INITIAL_PC);
    memory_write_32(m_context.pMemory, INITIAL_PC + 1024, 0xBAADFEED, READ_WRITE);
    pinkySimStep(&m_context);
}
*/
/*
TEST_SIM_ONLY(strImmediate, T2AttemptStoreToInvalidAddress)
{
    emitInstruction16("10010tttiiiiiiii", R2, 0);
    setRegisterValue(SP, 0xFFFFFFFC);
    setExpectedExceptionHandled(PINKYSIM_STEP_HARDFAULT);
    pinkySimStep(&m_context);
}
*/