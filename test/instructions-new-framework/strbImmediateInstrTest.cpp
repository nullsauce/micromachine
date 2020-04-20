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

/* STRB - Immediate
   Encoding: 011 1 0 Imm:5 Rn:3 Rt:3 */
MICROMACHINE_TEST_F(strbImmediate, UseAMixOfRegistersWordAlignedWithSmallestOffset, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	getCpu().regs().set(registers::R7, 0x77777777U);
	code_gen().emit_ins16("01110iiiiinnnttt", 0, registers::R7, registers::R0);
	getCpu().regs().set(registers::R7, INITIAL_PC + 4);
	getCpu().mem().write32(INITIAL_PC + 4, 0xBAADFEED);
	Step();
	EXPECT_EQ(0xBAADFE00, getCpu().mem().read32(INITIAL_PC + 4));
}

MICROMACHINE_TEST_F(strbImmediate, UseAnotherMixOfRegistersSecondByteInWord, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	getCpu().regs().set(registers::R7, 0x77777777U);
	code_gen().emit_ins16("01110iiiiinnnttt", 1, registers::R0, registers::R7);
	getCpu().regs().set(registers::R0, INITIAL_PC + 4);
	getCpu().mem().write32(INITIAL_PC + 4, 0xBAADFEED);
	Step();
	EXPECT_EQ(0xBAAD77ED, getCpu().mem().read32(INITIAL_PC + 4));
}

MICROMACHINE_TEST_F(strbImmediate, YetAnotherMixOfRegistersThirdByteInWord, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	getCpu().regs().set(registers::R4, 0x44444444U);
	code_gen().emit_ins16("01110iiiiinnnttt", 2, registers::R3, registers::R4);
	getCpu().regs().set(registers::R3, INITIAL_PC + 4);
	getCpu().mem().write32(INITIAL_PC + 4, 0xBAADFEED);
	Step();
	EXPECT_EQ(0xBA44FEED, getCpu().mem().read32(INITIAL_PC + 4));
}

MICROMACHINE_TEST_F(strbImmediate, YetAnotherMixOfRegistersFourthByteInWord, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	getCpu().regs().set(registers::R5, 0x55555555U);
	code_gen().emit_ins16("01110iiiiinnnttt", 3, registers::R1, registers::R5);
	getCpu().regs().set(registers::R1, INITIAL_PC + 4);
	getCpu().mem().write32(INITIAL_PC + 4, 0xBAADFEED);
	Step();
	EXPECT_EQ(0x55ADFEED, getCpu().mem().read32(INITIAL_PC + 4));
}

MICROMACHINE_TEST_F(strbImmediate, LargestOffset, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	getCpu().regs().set(registers::R4, 0x44444444U);
	code_gen().emit_ins16("01110iiiiinnnttt", 31, registers::R2, registers::R4);
	getCpu().regs().set(registers::R2, INITIAL_PC);
	getCpu().mem().write32(INITIAL_PC + 28, 0xBAADFEED);
	Step();
	EXPECT_EQ(0x44ADFEED, getCpu().mem().read32(INITIAL_PC + 28));
}

MICROMACHINE_TEST_F(strbImmediate, AttemptStoreToInvalidAddress, CpuTestFixture) {
	code_gen().emit_ins16("01110iiiiinnnttt", 0, registers::R3, registers::R0);
	getCpu().regs().set(registers::R3, 0xFFFFFFFC);
	Step();
	ExpectThat().HardfaultHandlerReached();
}
