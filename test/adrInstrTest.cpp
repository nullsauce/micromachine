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


/* ADR (ADDress of label)
   Encoding: 1010 0 Rd:3 Imm:8 */
TEST_F(CpuTestHelper, adr_LowestRegisterWithLargestOffset)
{
	emitInstruction16("10100dddiiiiiiii", R0, 255);
	setExpectedRegisterValue(R0, INITIAL_PC + 4 + 255 * 4);
	pinkySimStep(&m_context);
}

TEST_F(CpuTestHelper, adr_HighesttRegisterWithSmallestOffset)
{
	emitInstruction16("10100dddiiiiiiii", R7, 0);
	setExpectedRegisterValue(R7, INITIAL_PC + 4);
	pinkySimStep(&m_context);
}

TEST_F(CpuTestHelper, adr_pcWillNeedToBeWordAlignedBeforeAdd)
{
	// Emit UNDEFINED 16-bit instruction.
	emitInstruction16("1101111000000000");
	// Emit actual test instruction at a 2-byte aligned address which isn't 4-byte aligned.
	emitInstruction16("10100dddiiiiiiii", R3, 0);
	setRegisterValue(PC, INITIAL_PC + 2);
	setExpectedRegisterValue(R3, INITIAL_PC + 4);
	pinkySimStep(&m_context);
}
