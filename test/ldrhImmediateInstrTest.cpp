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


/* LDRH - Immediate
   Encoding: 1000 1 Imm:5 Rn:3 Rt:3 */
TEST_F(CpuTestHelper, ldrhImmediate_UseAMixOfRegistersWordAligned)
{
	emitInstruction16("10001iiiiinnnttt", 0, R7, R0);
	setRegisterValue(R7, INITIAL_PC + 4);
	memory_write_32(INITIAL_PC + 4, 0xBAADFEED);
	setExpectedRegisterValue(R0, 0xFEED);
	step();
}

TEST_F(CpuTestHelper, ldrhImmediate_UseAnotherMixOfRegistersNotWordAligned)
{
	emitInstruction16("10001iiiiinnnttt", 1, R0, R7);
	setRegisterValue(R0, INITIAL_PC + 4);
	memory_write_32(INITIAL_PC + 4, 0xBAADFEED);
	setExpectedRegisterValue(R7, 0xBAAD);
	step();
}

TEST_F(CpuTestHelper, ldrhImmediate_LargestOffset)
{
	emitInstruction16("10001iiiiinnnttt", 31, R1, R6);
	setRegisterValue(R1, INITIAL_PC);
	memory_write_32(INITIAL_PC + 60, 0xBAADFEED);
	setExpectedRegisterValue(R6, 0xBAAD);
	step();
}

TEST_F(CpuTestHelper, ldrhImmediate_AttemptLoadFromInvalidAddress)
{
	emitInstruction16("10001iiiiinnnttt", 0, R3, R0);
	setRegisterValue(R3, 0xFFFFFFFC);
	setExpectedExceptionTaken(PINKYSIM_STEP_HARDFAULT);
	step();
}
