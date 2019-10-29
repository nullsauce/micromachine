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


/* CMP - Immediate
   Encoding: 001 01 Rn:3 Imm:8 */
TEST_F(pinkySimBase, cmpImmediate_CompareLowestRegisterToEqualValue)
{
	emitInstruction16("00101nnniiiiiiii", R0, 0);
	setExpectedXPSRflags("nZCv");
	pinkySimStep(&m_context);
}

TEST_F(pinkySimBase, cmpImmediate_CompareHighestRegisterToImmediateWhichIsSmaller)
{
	emitInstruction16("00101nnniiiiiiii", R7, 127);
	setExpectedXPSRflags("nzCv");
	pinkySimStep(&m_context);
}

TEST_F(pinkySimBase, cmpImmediate_CompareRegisterToLargestImmediateWhichIsLarger)
{
	emitInstruction16("00101nnniiiiiiii", R0, 255);
	setExpectedXPSRflags("Nzcv");
	pinkySimStep(&m_context);
}

TEST_F(pinkySimBase, cmpImmediate_CompareRegisterToImmediateWhichWillGenerateNegativeOverflow)
{
	emitInstruction16("00101nnniiiiiiii", R3, 1);
	setRegisterValue(R3, 0x80000000);
	setExpectedXPSRflags("nzCV");
	pinkySimStep(&m_context);
}
