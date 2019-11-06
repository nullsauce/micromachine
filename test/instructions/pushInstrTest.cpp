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


/* PUSH
   Encoding: 1011 0 10 M:1 RegisterList:8 */
TEST_F(CpuTestHarness, push_JustPushLR)
{
	code_gen().emit_ins16("1011010Mrrrrrrrr", 1, 0);
	setExpectedRegisterValue(SP, INITIAL_SP - 4);
	memory_write_32(INITIAL_SP - 4, 0x0);
	step();
	EXPECT_EQ(INITIAL_LR, memory_read_32(INITIAL_SP - 4));
}

TEST_F(CpuTestHarness, push_JustPushR0)
{
	code_gen().emit_ins16("1011010Mrrrrrrrr", 0, 1);
	setExpectedRegisterValue(SP, INITIAL_SP - 4);
	memory_write_32(INITIAL_SP - 4, 0xFFFFFFFF);
	step();
	EXPECT_EQ(0x0, memory_read_32(INITIAL_SP - 4));
}

TEST_F(CpuTestHarness, push_JustPushR7)
{
	code_gen().emit_ins16("1011010Mrrrrrrrr", 0, 1 << 7);
	setExpectedRegisterValue(SP, INITIAL_SP - 4);
	memory_write_32(INITIAL_SP - 4, 0xFFFFFFFF);
	step();
	EXPECT_EQ(0x77777777, memory_read_32(INITIAL_SP - 4));
}

TEST_F(CpuTestHarness, push_PushAll)
{
	code_gen().emit_ins16("1011010Mrrrrrrrr", 1, 0xFF);
	setExpectedRegisterValue(SP, INITIAL_SP - 4 * 9);
	for (int i = 1; i <= 9; i++)
		memory_write_32(INITIAL_SP - 4 * i, 0xFFFFFFFF);
	step();
	for (int i = 0; i < 8; i++)
		EXPECT_EQ(0x11111111U * i, memory_read_32(INITIAL_SP - 4 * (9 - i)));
	EXPECT_EQ(INITIAL_LR, memory_read_32(INITIAL_SP - 4));
}
/*
TEST_SIM_ONLY(push, HardFaultFromInvalidMemoryWrite)
{
    code_gen().emit_ins16("1011010Mrrrrrrrr", 0, 1);
    setRegisterValue(SP, 0xFFFFFFFC);
    setExpectedExceptionHandled(CPU_STEP_HARDFAULT);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(push, UnpredictableToPushNoRegisters)
{
    code_gen().emit_ins16("1011010Mrrrrrrrr", 0, 0);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    step(&m_context);
}
*/