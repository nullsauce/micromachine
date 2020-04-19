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


/* BL (Branch with Link)
   Encoding: 11110 S Imm:10
             11 J1:1 1 J2:1 Imm:11
    Note: J1 and J2 are translated to immediate bits via I? = NOT(J? XOR S) */
MICROMACHINE_TEST_F(bl, OffsetOf0, CpuTestFixture) {
	constexpr uint32_t INITIAL_PC = 0x00001000;
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins32("11110Siiiiiiiiii", "11j1kiiiiiiiiiii", 0, 0, 1, 1, 0);
	Step();
	ExpectThat().Register(registers::PC).Equals(INITIAL_PC + 4);
	ExpectThat().Register(registers::LR).Equals((INITIAL_PC + 4) | 1);
}

MICROMACHINE_TEST_F(bl, MaximumPositiveOffset, CpuTestFixture) {
	constexpr uint32_t INITIAL_PC = 0x00001000;
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins32("11110Siiiiiiiiii", "11j1kiiiiiiiiiii", 0, 0x3FF, 0, 0, 0x7FF);
	Step();
	ExpectThat().Register(registers::PC).Equals(INITIAL_PC + 4 + 16777214);
	ExpectThat().Register(registers::LR).Equals((INITIAL_PC + 4) | 1);
}

MICROMACHINE_TEST_F(bl, MaximumNegativeOffset, CpuTestFixture) {
	constexpr uint32_t INITIAL_PC = 0x00001000;
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins32("11110Siiiiiiiiii", "11j1kiiiiiiiiiii", 1, 0, 0, 0, 0);
	Step();
	ExpectThat().Register(registers::PC).Equals(INITIAL_PC + 4 - 16777216);
	ExpectThat().Register(registers::LR).Equals((INITIAL_PC + 4) | 1);
}
