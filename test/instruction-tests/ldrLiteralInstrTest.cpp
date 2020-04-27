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


/* LDR - Literal
   Encoding: 01001 Rt:3 Imm:8 */
MICROMACHINE_TEST_F(ldrLiteral, LoadOffset0IntoHighestRegister, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("01001tttiiiiiiii", registers::R7, 0);
	getCpu().mem().write32(INITIAL_PC + 4, 0xBAADFEED);
	Step();
	ExpectThat().Register(registers::R7).Equals(0xBAADFEED);
}

MICROMACHINE_TEST_F(ldrLiteral, LoadOffset0IntoHighestRegisterNot4ByteAligned, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	// Emit UNDEFINED 16-bit instruction.
	code_gen().emit_ins16("1101111000000000");
	// Emit actual test instruction at a 2-byte aligned address which isn't 4-byte aligned.
	code_gen().emit_ins16("01001tttiiiiiiii", registers::R7, 0);
	getCpu().mem().write32(INITIAL_PC + 4, 0xBAADFEED);
	// Move PC to point to second instruction.
	getCpu().regs().set(registers::PC, getCpu().regs().get_pc() + 2);
	Step();
	ExpectThat().Register(registers::R7).Equals(0xBAADFEED);
}

MICROMACHINE_TEST_F(ldrLiteral, LoadMaximumOffsetIntoLowestRegister, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("01001tttiiiiiiii", registers::R0, 255);
	getCpu().mem().write32(INITIAL_PC + 4 + 255 * 4, 0xBAADFEED);
	Step();
	ExpectThat().Register(registers::R0).Equals(0xBAADFEED);
}

MICROMACHINE_TEST_F(ldrLiteral, AttemptToLoadFromInvalidAddress, CpuTestFixture) {
	// load a pc-relative address at end of memory - 128, with an offset of 255
	// this should overflow memory by 127 and hard fault
	getCpu().regs().set_pc(TestSystem::MEMORY_SIZE - 128);
	code_gen().set_write_address(TestSystem::MEMORY_SIZE - 128);
	code_gen().emit_ins16("01001tttiiiiiiii", registers::R0, 255);

	Step();
	ExpectThat().HardfaultHandlerReached();
}
