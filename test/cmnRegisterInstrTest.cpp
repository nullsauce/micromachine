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


/* CMN - Register (Compare Negative)
   Encoding: 010000 1011 Rm:3 Rn:3 */
TEST_F(pinkySimBase, cmnRegister_UseLowestRegisterForAllArgs)
{
	emitInstruction16("0100001011mmmnnn", R0, R0);
	setExpectedXPSRflags("nZcv");
	pinkySimStep(&m_context);
}

TEST_F(pinkySimBase, cmnRegister_UseHigestRegisterForAllArgs)
{
	emitInstruction16("0100001011mmmnnn", R7, R7);
	setExpectedXPSRflags("NzcV");
	pinkySimStep(&m_context);
}

TEST_F(pinkySimBase, cmnRegister_UseDifferentRegistersForEachArg)
{
	emitInstruction16("0100001011mmmnnn", R1, R2);
	setExpectedXPSRflags("nzcv");
	pinkySimStep(&m_context);
}

// Force APSR flags to be set which haven't already been covered above.
TEST_F(pinkySimBase, cmnRegister_ForceCarryWithNoOverflow)
{
	emitInstruction16("0100001011mmmnnn", R1, R2);
	setExpectedXPSRflags("nZCv");
	setRegisterValue(R1, -1);
	setRegisterValue(R2, 1);
	pinkySimStep(&m_context);
}

TEST_F(pinkySimBase, cmnRegister_ForceCarryAndOverflow)
{
	emitInstruction16("0100001011mmmnnn", R1, R2);
	setExpectedXPSRflags("nzCV");
	setRegisterValue(R1, -1);
	setRegisterValue(R2, 0x80000000U);
	pinkySimStep(&m_context);
}
