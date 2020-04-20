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


/* DMB
   Encoding: 11110 0 111 01 1 (1)(1)(1)(1)
             10 (0) 0 (1)(1)(1)(1) 0101 option:4 */
MICROMACHINE_TEST_F(dmb, OptionSetTo15, CpuTestFixture) {
	static uint32_t INITIAL_PC = 0x00001000;
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins32("1111001110111111", "100011110101oooo", 15);
	Step();
	ExpectThat().Register(registers::PC).WasIncrementedBy(4);
}

MICROMACHINE_TEST_F(dmb, OptionSetTo0, CpuTestFixture) {
	static uint32_t INITIAL_PC = 0x00001000;
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins32("1111001110111111", "100011110101oooo", 0);
	Step();
	ExpectThat().Register(registers::PC).WasIncrementedBy(4);
}
