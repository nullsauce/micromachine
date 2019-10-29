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


/* SVC
   Encoding: 1101 1111 Imm:8 */
TEST_F(pinkySimBase, svc_SmallestImmediate)
{
    emitInstruction16("11011111iiiiiiii", 0);
	setExpectedExceptionTaken(PINKYSIM_STEP_SVC);
    pinkySimStep(&m_context);
}

TEST_F(pinkySimBase, svc_LargestImmediate)
{
    emitInstruction16("11011111iiiiiiii", 255);
	setExpectedExceptionTaken(PINKYSIM_STEP_SVC);
    pinkySimStep(&m_context);
}
