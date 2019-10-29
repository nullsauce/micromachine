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


/* POP
   Encoding: 1011 1 10 P:1 RegisterList:8 */
TEST_F(pinkySimBase, pop_JustPopPC)
{
	emitInstruction16("1011110Prrrrrrrr", 1, 0);
	setRegisterValue(SP, INITIAL_SP - 4);
	setExpectedRegisterValue(SP, INITIAL_SP);
	setExpectedRegisterValue(PC, INITIAL_PC + 16);
	memory_write_32(m_context.pMemory, INITIAL_SP - 4, (INITIAL_PC + 16) | 1, READ_ONLY);
	pinkySimStep(&m_context);
}

TEST_F(pinkySimBase, pop_JustPopR0)
{
	emitInstruction16("1011110Prrrrrrrr", 0, 1);
	setRegisterValue(SP, INITIAL_SP - 4);
	setExpectedRegisterValue(SP, INITIAL_SP);
	setExpectedRegisterValue(R0, 0xFFFFFFFF);
	memory_write_32(m_context.pMemory, INITIAL_SP - 4, 0xFFFFFFFF, READ_ONLY);
	pinkySimStep(&m_context);
}

TEST_F(pinkySimBase, pop_JustPopR7)
{
	emitInstruction16("1011110Prrrrrrrr", 0, (1 << 7));
	setRegisterValue(SP, INITIAL_SP - 4);
	setExpectedRegisterValue(SP, INITIAL_SP);
	setExpectedRegisterValue(R7, 0xFFFFFFFF);
	memory_write_32(m_context.pMemory, INITIAL_SP - 4, 0xFFFFFFFF, READ_ONLY);
	pinkySimStep(&m_context);
}

TEST_F(pinkySimBase, pop_PopAll)
{
	emitInstruction16("1011110Prrrrrrrr", 1, 0xFF);
	setRegisterValue(SP, INITIAL_SP - 4 * 9);
	setExpectedRegisterValue(SP, INITIAL_SP);
	setExpectedRegisterValue(R0, 9);
	setExpectedRegisterValue(R1, 8);
	setExpectedRegisterValue(R2, 7);
	setExpectedRegisterValue(R3, 6);
	setExpectedRegisterValue(R4, 5);
	setExpectedRegisterValue(R5, 4);
	setExpectedRegisterValue(R6, 3);
	setExpectedRegisterValue(R7, 2);
	setExpectedRegisterValue(PC, 1 & ~1);
	for (int i = 1; i <= 9; i++)
		memory_write_32(m_context.pMemory, INITIAL_SP - 4 * i, i, READ_ONLY);
	pinkySimStep(&m_context);
}

TEST_F(pinkySimBase, pop_PopToSetPCToEvenAddressWhichGeneratesHardFault)
{
	emitInstruction16("1011110Prrrrrrrr", 1, 0);
	setExpectedXPSRflags("t");
	setRegisterValue(SP, INITIAL_SP - 4);
	setExpectedRegisterValue(SP, INITIAL_SP);
	setExpectedRegisterValue(PC, INITIAL_PC + 16);
	memory_write_32(m_context.pMemory, INITIAL_SP - 4, INITIAL_PC + 16, READ_ONLY);
	pinkySimStep(&m_context);

	const uint16_t NOP = 0xBF00;
	memory_write_32(m_context.pMemory, INITIAL_PC + 16, NOP, READ_ONLY);
	setExpectedExceptionTaken(PINKYSIM_STEP_HARDFAULT);
	pinkySimStep(&m_context);
}
/*
TEST_F(pinkySimBase, pop_HardFaultFromInvalidMemoryRead)
{
    emitInstruction16("1011110Prrrrrrrr", 0, 1);
    setRegisterValue(SP, 0xFFFFFFFC);
    setExpectedExceptionHandled(PINKYSIM_STEP_HARDFAULT);
    pinkySimStep(&m_context);
}*/
/*
TEST_SIM_ONLY(pop, UnpredictableToPopNoRegisters)
{
    emitInstruction16("1011110Prrrrrrrr", 0, 0);
    setExpectedStepReturn(PINKYSIM_STEP_UNPREDICTABLE);
    pinkySimStep(&m_context);
}
*/