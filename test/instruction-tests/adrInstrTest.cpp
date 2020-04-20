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


/* ADR (ADDress of label)
   Encoding: 1010 0 Rd:3 Imm:8 */
MICROMACHINE_TEST_F(adr, LowestRegisterWithLargestOffset, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);

	code_gen().emit_ins16("10100dddiiiiiiii", registers::R0, 255);
	Step();
	ExpectThat().Register(registers::R0).Equals(INITIAL_PC + 4 + 255 * 4);
}

MICROMACHINE_TEST_F(adr, HighesttRegisterWithSmallestOffset, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);

	code_gen().emit_ins16("10100dddiiiiiiii", registers::R7, 0);
	Step();
	ExpectThat().Register(registers::R7).Equals(INITIAL_PC + 4);
}

MICROMACHINE_TEST_F(adr, pcWillNeedToBeWordAlignedBeforeAdd, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	getCpu().regs().set(registers::PC, INITIAL_PC + 2);

	// Emit UNDEFINED 16-bit instruction.
	code_gen().emit_ins16("1101111000000000");
	// Emit actual test instruction at a 2-byte aligned address which isn't 4-byte aligned.
	code_gen().emit_ins16("10100dddiiiiiiii", registers::R3, 0);

	Step();
	ExpectThat().Register(registers::R3).Equals(INITIAL_PC + 4);
}
