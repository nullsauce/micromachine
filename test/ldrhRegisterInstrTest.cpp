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


/* LDRH - Register
   Encoding: 0101 101 Rm:3 Rn:3 Rt:3 */
TEST_F(pinkySimBase, ldrhRegister_UseAMixOfRegistersWordAligned)
{
	emitInstruction16("0101101mmmnnnttt", R7, R3, R0);
	setRegisterValue(R3, INITIAL_PC);
	setRegisterValue(R7, 4);
	memory_write_32(m_context.pMemory, INITIAL_PC + 4, 0xBAADFEED, READ_ONLY);
	setExpectedRegisterValue(R0, 0xFEED);
	pinkySimStep(&m_context);
}

TEST_F(pinkySimBase, ldrhRegister_UseAnotherMixOfRegistersWordAligned)
{
	emitInstruction16("0101101mmmnnnttt", R1, R0, R7);
	setRegisterValue(R0, INITIAL_PC);
	setRegisterValue(R1, 4);
	memory_write_32(m_context.pMemory, INITIAL_PC + 4, 0xBAADFEED, READ_ONLY);
	setExpectedRegisterValue(R7, 0xFEED);
	pinkySimStep(&m_context);
}

TEST_F(pinkySimBase, ldrhRegister_YetAnotherMixOfRegistersNotWordAligned)
{
	emitInstruction16("0101101mmmnnnttt", R0, R7, R4);
	setRegisterValue(R7, INITIAL_PC);
	setRegisterValue(R0, 6);
	memory_write_32(m_context.pMemory, INITIAL_PC + 4, 0xBAADFEED, READ_ONLY);
	setExpectedRegisterValue(R4, 0xBAAD);
	pinkySimStep(&m_context);
}

TEST_F(pinkySimBase, ldrhRegister_AttemptUnalignedLoad)
{
	emitInstruction16("0101101mmmnnnttt", R7, R3, R0);
	setRegisterValue(R3, INITIAL_PC);
	setRegisterValue(R7, 1);
	setExpectedExceptionTaken(PINKYSIM_STEP_HARDFAULT);
	pinkySimStep(&m_context);
}

TEST_F(pinkySimBase, ldrhRegister_AttemptLoadFromInvalidAddress)
{
	emitInstruction16("0101101mmmnnnttt", R7, R3, R0);
	setRegisterValue(R3, 0xFFFFFFFC);
	setRegisterValue(R7, 0);
	setExpectedExceptionTaken(PINKYSIM_STEP_HARDFAULT);
	pinkySimStep(&m_context);
}
