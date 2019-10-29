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

TEST_GROUP_BASE(ldrRegister, pinkySimBase)
{
    void setup()
    {
        pinkySimBase::setup();
    }

    void teardown()
    {
        pinkySimBase::teardown();
    }
};


/* LDR - Register
   Encoding: 0101 100 Rm:3 Rn:3 Rt:3 */
PINKY_TEST(ldrRegister, UseAMixOfRegisters)
{
    emitInstruction16("0101100mmmnnnttt", R7, R3, R0);
    setRegisterValue(R3, INITIAL_PC);
    setRegisterValue(R7, 4);
    SimpleMemory_SetMemory(m_context.pMemory, INITIAL_PC + 4, 0xBAADFEED, READ_ONLY);
    setExpectedRegisterValue(R0, 0xBAADFEED);
    pinkySimStep(&m_context);
}

PINKY_TEST(ldrRegister, UseAnotherMixOfRegisters)
{
    emitInstruction16("0101100mmmnnnttt", R1, R0, R7);
    setRegisterValue(R0, INITIAL_PC);
    setRegisterValue(R1, 4);
    SimpleMemory_SetMemory(m_context.pMemory, INITIAL_PC + 4, 0xBAADFEED, READ_ONLY);
    setExpectedRegisterValue(R7, 0xBAADFEED);
    pinkySimStep(&m_context);
}

PINKY_TEST(ldrRegister, YetAnotherMixOfRegisters)
{
    emitInstruction16("0101100mmmnnnttt", R0, R7, R4);
    setRegisterValue(R7, INITIAL_PC);
    setRegisterValue(R0, 4);
    SimpleMemory_SetMemory(m_context.pMemory, INITIAL_PC + 4, 0xBAADFEED, READ_ONLY);
    setExpectedRegisterValue(R4, 0xBAADFEED);
    pinkySimStep(&m_context);
}

PINKY_TEST(ldrRegister, AttemptUnalignedLoad)
{
    emitInstruction16("0101100mmmnnnttt", R7, R3, R0);
    setRegisterValue(R3, INITIAL_PC);
    setRegisterValue(R7, 2);
	setExpectedExceptionTaken(PINKYSIM_STEP_HARDFAULT);
    pinkySimStep(&m_context);
}

PINKY_TEST(ldrRegister, AttemptLoadFromInvalidAddress)
{
    emitInstruction16("0101100mmmnnnttt", R7, R3, R0);
    setRegisterValue(R3, 0xFFFFFFFC);
    setRegisterValue(R7, 0);
	setExpectedExceptionTaken(PINKYSIM_STEP_HARDFAULT);
    pinkySimStep(&m_context);
}
