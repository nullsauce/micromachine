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


/* STRH - Immediate
   Encoding: 1000 0 Imm:5 Rn:3 Rt:3 */
MICROMACHINE_TEST_F(strhImmediate, UseAMixOfRegistersWordAlignedWithSmallestOffset, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("10000iiiiinnnttt", 0, registers::R7, registers::R0);
	getCpu().regs().set(registers::R7, INITIAL_PC + 4);
	getCpu().mem().write32(INITIAL_PC + 4, 0xBAADFEED);
	Step();
	EXPECT_EQ(0xBAAD0000, getCpu().mem().read32(INITIAL_PC + 4));
}

MICROMACHINE_TEST_F(strhImmediate, AnotherMixOfRegistersNotWordAligned, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	getCpu().regs().set(registers::R7, 0x77777777U);
	code_gen().emit_ins16("10000iiiiinnnttt", 1, registers::R0, registers::R7);
	getCpu().regs().set(registers::R0, INITIAL_PC + 4);
	getCpu().mem().write32(INITIAL_PC + 4, 0xBAADFEED);
	Step();
	EXPECT_EQ(0x7777FEED, getCpu().mem().read32(INITIAL_PC + 4));
}

MICROMACHINE_TEST_F(strhImmediate, LargestOffset, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	getCpu().regs().set(registers::R6, 0x66666666U);
	code_gen().emit_ins16("10000iiiiinnnttt", 31, registers::R1, registers::R6);
	getCpu().regs().set(registers::R1, INITIAL_PC);
	getCpu().mem().write32(INITIAL_PC + 60, 0xBAADFEED);
	Step();
	EXPECT_EQ(0x6666FEED, getCpu().mem().read32(INITIAL_PC + 60));
}

MICROMACHINE_TEST_F(strhImmediate, AttemptStoreToInvalidAddress, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	getCpu().regs().set(registers::R1, 0x11111111U);
	code_gen().emit_ins16("10000iiiiinnnttt", 0, registers::R3, registers::R1);
	getCpu().regs().set(registers::R3, 0xFFFFFFFC);
	Step();
	ExpectThat().HardfaultHandlerReached();
}
