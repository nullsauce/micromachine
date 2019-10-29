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


/* REVSH
   Encoding: 1011 1010 11 Rm:3 Rd:3 */
TEST_F(CpuTestHelper, revsh_RevR0toR7)
{
	emitInstruction16("1011101011mmmddd", R0, R7);
	setRegisterValue(R0, 0x12345678);
	setExpectedRegisterValue(R7, 0x7856);
	step();
}

TEST_F(CpuTestHelper, revsh_RevR7toR0)
{
	emitInstruction16("1011101011mmmddd", R7, R0);
	setRegisterValue(R7, 0x12345678);
	setExpectedRegisterValue(R0, 0x7856);
	step();
}

TEST_F(CpuTestHelper, revsh_PositiveValue)
{
	emitInstruction16("1011101011mmmddd", R7, R0);
	setRegisterValue(R7, 0xFF7F);
	setExpectedRegisterValue(R0, 0x7FFF);
	step();
}

TEST_F(CpuTestHelper, revsh_NegativeValue)
{
	emitInstruction16("1011101011mmmddd", R7, R0);
	setRegisterValue(R7, 0x0080);
	setExpectedRegisterValue(R0, 0xFFFF8000);
	step();
}
