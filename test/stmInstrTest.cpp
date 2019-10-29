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


/* PUSH
   Encoding: 1100 0 Rn:3 RegisterList:8 */
TEST_F(CpuTestHelper, stm_JustPushR0WithR7AsAddress)
{
	emitInstruction16("11000nnnrrrrrrrr", R7, (1 << 0));
	setRegisterValue(R7, INITIAL_PC + 16);
	setExpectedRegisterValue(R7, INITIAL_PC + 16 + 1 * 4);
	memory_write_32(m_context.pMemory, INITIAL_PC + 16, 0xFFFFFFFF, READ_WRITE);
	pinkySimStep(&m_context);
	EXPECT_EQ(0x0, memory_read_32(m_context.pMemory, INITIAL_PC + 16));
}

TEST_F(CpuTestHelper, stm_JustPushR7WithR0AsAddress)
{
	emitInstruction16("11000nnnrrrrrrrr", R0, (1 << 7));
	setRegisterValue(R0, INITIAL_PC + 16);
	setExpectedRegisterValue(R0, INITIAL_PC + 16 + 1 * 4);
	memory_write_32(m_context.pMemory, INITIAL_PC + 16, 0xFFFFFFFF, READ_WRITE);
	pinkySimStep(&m_context);
	EXPECT_EQ(0x77777777, memory_read_32(m_context.pMemory, INITIAL_PC + 16));
}

TEST_F(CpuTestHelper, stm_PushAllWithR0AsAddress)
{
	emitInstruction16("11000nnnrrrrrrrr", R0, 0xFF);
	setRegisterValue(R0, INITIAL_PC + 16);
	setExpectedRegisterValue(R0, INITIAL_PC + 16 + 8 * 4);
	for (int i = 0; i < 8; i++)
		memory_write_32(m_context.pMemory, INITIAL_PC + 16 + 4 * i, 0xFFFFFFFF, READ_WRITE);
	pinkySimStep(&m_context);
	EXPECT_EQ(INITIAL_PC + 16, memory_read_32(m_context.pMemory, INITIAL_PC + 16 + 4 * 0));
	for (int i = 1; i < 8; i++)
		EXPECT_EQ(0x11111111U * i, memory_read_32(m_context.pMemory, INITIAL_PC + 16 + 4 * i));
}

TEST_F(CpuTestHelper, stm_PushAllButR7WithR7AsAddress)
{
	emitInstruction16("11000nnnrrrrrrrr", R7, 0x7F);
	setRegisterValue(R7, INITIAL_PC + 16);
	setExpectedRegisterValue(R7, INITIAL_PC + 16 + 7 * 4);
	for (int i = 0; i < 7; i++)
		memory_write_32(m_context.pMemory, INITIAL_PC + 16 + 4 * i, 0xFFFFFFFF, READ_WRITE);
	pinkySimStep(&m_context);
	for (int i = 0; i < 7; i++)
		EXPECT_EQ(0x11111111U * i, memory_read_32(m_context.pMemory, INITIAL_PC + 16 + 4 * i));
}

TEST_F(CpuTestHelper, stm_HardFaultFromInvalidMemoryWrite)
{
	emitInstruction16("11000nnnrrrrrrrr", R0, 1 << 0);
	setRegisterValue(R0, 0xFFFFFFFC);
	setExpectedExceptionTaken(PINKYSIM_STEP_HARDFAULT);
	pinkySimStep(&m_context);
}
/*
TEST_SIM_ONLY(stm, UnpredictableToPushNoRegisters)
{
    emitInstruction16("11000nnnrrrrrrrr", R0, 0);
    setExpectedStepReturn(PINKYSIM_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(PC, INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(stm, UnpredictableToPushWritebackRegisterWhichIsntFirstSaved)
{
    emitInstruction16("11000nnnrrrrrrrr", R7, 0xFF);
    setRegisterValue(R7, INITIAL_PC + 16);
    setExpectedStepReturn(PINKYSIM_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(PC, INITIAL_PC);
    pinkySimStep(&m_context);
}*/
