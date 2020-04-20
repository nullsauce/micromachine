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

#include "CpuTestFixture.hpp"


/* NOP
   Encoding: 1011 1111 0000 0000 */
MICROMACHINE_TEST_F(nop, BasicTest, CpuTestFixture) {
	code_gen().emit_ins16("1011111100000000");
	Step();
	ExpectThat()
		.InstructionExecutedWithoutBranch();
}


/* Unallocated hint encodings with OpB == 0 are treated as NOP as well. */
// TODO: Adapt this test because the doc says it should raise and UNDEFINED exception
/*
MICROMACHINE_TEST_F(nop, UnallocatedHints, CpuTestFixture)
{
    for (uint32_t opA = 5 ; opA < 16 ; opA++)
    {
        code_gen().emit_ins16("10111111aaaa0000", opA);
        step(&m_context);
        initContext();
    }
}*/
