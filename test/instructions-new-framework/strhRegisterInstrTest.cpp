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


/* STRH - Register
   Encoding: 0101 001 Rm:3 Rn:3 Rt:3 */
MICROMACHINE_TEST_F(strhRegister, UseAMixOfRegistersWordAligned, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("0101001mmmnnnttt", registers::R7, registers::R3, registers::R0);
	getCpu().regs().set(registers::R3, INITIAL_PC);
	getCpu().regs().set(registers::R7, 4);
	getCpu().mem().write32(INITIAL_PC + 4, 0xBAADFEED);
	Step();
	EXPECT_EQ(0xBAAD0000, getCpu().mem().read32(INITIAL_PC + 4));
}

MICROMACHINE_TEST_F(strhRegister, UseAnotherMixOfRegistersWordAligned, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	getCpu().regs().set(registers::R7, 0x77777777);
	code_gen().emit_ins16("0101001mmmnnnttt", registers::R1, registers::R0, registers::R7);
	getCpu().regs().set(registers::R0, INITIAL_PC);
	getCpu().regs().set(registers::R1, 4);
	getCpu().mem().write32(INITIAL_PC + 4, 0xBAADFEED);
	Step();
	EXPECT_EQ(0xBAAD7777, getCpu().mem().read32(INITIAL_PC + 4));
}

MICROMACHINE_TEST_F(strhRegister, YetAnotherMixOfRegistersNotWordAligned, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	getCpu().regs().set(registers::R4, 0x44444444);
	code_gen().emit_ins16("0101001mmmnnnttt", registers::R0, registers::R7, registers::R4);
	getCpu().regs().set(registers::R7, INITIAL_PC);
	getCpu().regs().set(registers::R0, 6);
	getCpu().mem().write32(INITIAL_PC + 4, 0xBAADFEED);
	Step();
	EXPECT_EQ(0x4444FEED, getCpu().mem().read32(INITIAL_PC + 4));
}

MICROMACHINE_TEST_F(strhRegister, AttemptUnalignedStore, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("0101001mmmnnnttt", registers::R7, registers::R3, registers::R0);
	getCpu().regs().set(registers::R3, INITIAL_PC + 1024);
	getCpu().regs().set(registers::R7, 1);
	getCpu().mem().write32(INITIAL_PC + 1024, 0xBAADFEED);
	Step();
	ExpectThat().HardfaultHandlerReached();
}

MICROMACHINE_TEST_F(strhRegister, AttemptStoreToInvalidAddress, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("0101001mmmnnnttt", registers::R7, registers::R3, registers::R0);
	getCpu().regs().set(registers::R3, 0xFFFFFFFC);
	getCpu().regs().set(registers::R7, 0);
	Step();
	ExpectThat().HardfaultHandlerReached();
}
