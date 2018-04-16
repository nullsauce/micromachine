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
#include "pinkySimBaseTest.h"

TEST_GROUP_BASE(undefined, pinkySimBase)
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


PINKY_TEST(undefined, Undedfined16BitWithAllZeroesForImmedaite)
{
    emitInstruction16("11011110iiiiiiii", 0);
	setExpectedExceptionTaken(PINKYSIM_STEP_HARDFAULT);
    pinkySimStep(&m_context);
}

PINKY_TEST(undefined, Undedfined16BitWithAllOnesForImmedaite)
{
    emitInstruction16("11011110iiiiiiii", -1);
	setExpectedExceptionTaken(PINKYSIM_STEP_HARDFAULT);
    pinkySimStep(&m_context);
}

PINKY_TEST(undefined, Undefined32BitWithAllZeroesForImmediate)
{
    emitInstruction32("111101111111iiii", "1010iiiiiiiiiiii", 0, 0);
	setExpectedExceptionTaken(PINKYSIM_STEP_HARDFAULT);
    pinkySimStep(&m_context);
}

PINKY_TEST(undefined, Undefined32BitWithAllOnesForImmediate)
{
    emitInstruction32("111101111111iiii", "1010iiiiiiiiiiii", -1, -1);
	setExpectedExceptionTaken(PINKYSIM_STEP_HARDFAULT);
    pinkySimStep(&m_context);
}
