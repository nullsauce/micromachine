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


/* LDRB - Immediate
   Encoding: 011 1 1 Imm:5 Rn:3 Rt:3 */
MICROMACHINE_TEST_F(ldrbImmediate, UseAMixOfRegistersWordAligned, CpuTestFixture) {
	constexpr uint32_t INITIAL_PC = 0x00001000;
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("01111iiiiinnnttt", 0, registers::R7, registers::R0);
	getCpu().regs().set(registers::R7, INITIAL_PC + 4);
	getCpu().mem().write32(INITIAL_PC + 4, 0xBAADFEED);
	Step();
	ExpectThat().Register(registers::R0).Equals(0xED);
}

MICROMACHINE_TEST_F(ldrbImmediate, UseAnotherMixOfRegistersSecondByteInWord, CpuTestFixture) {
	constexpr uint32_t INITIAL_PC = 0x00001000;
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("01111iiiiinnnttt", 1, registers::R0, registers::R7);
	getCpu().regs().set(registers::R0, INITIAL_PC + 4);
	getCpu().mem().write32(INITIAL_PC + 4, 0xBAADFEED);
	Step();
	ExpectThat().Register(registers::R7).Equals(0xFE);
}

MICROMACHINE_TEST_F(ldrbImmediate, YetAnotherMixOfRegistersThirdByteInWord, CpuTestFixture) {
	constexpr uint32_t INITIAL_PC = 0x00001000;
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("01111iiiiinnnttt", 2, registers::R1, registers::R4);
	getCpu().regs().set(registers::R1, INITIAL_PC + 4);
	getCpu().mem().write32(INITIAL_PC + 4, 0xBAADFEED);
	Step();
	ExpectThat().Register(registers::R4).Equals(0xAD);
}

MICROMACHINE_TEST_F(ldrbImmediate, YetAnotherMixOfRegistersFourthByteInWord, CpuTestFixture) {
	constexpr uint32_t INITIAL_PC = 0x00001000;
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("01111iiiiinnnttt", 3, registers::R2, registers::R5);
	getCpu().regs().set(registers::R2, INITIAL_PC + 4);
	getCpu().mem().write32(INITIAL_PC + 4, 0xBAADFEED);
	Step();
	ExpectThat().Register(registers::R5).Equals(0xBA);
}

MICROMACHINE_TEST_F(ldrbImmediate, UseLargestOffset, CpuTestFixture) {
	constexpr uint32_t INITIAL_PC = 0x00001000;
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("01111iiiiinnnttt", 31, registers::R3, registers::R0);
	getCpu().regs().set(registers::R3, INITIAL_PC);
	getCpu().mem().write32(INITIAL_PC + 28, 0x12345678);
	Step();
	ExpectThat().Register(registers::R0).Equals(0x12);
}

MICROMACHINE_TEST_F(ldrbImmediate, LoadAPositiveValue, CpuTestFixture) {
	constexpr uint32_t INITIAL_PC = 0x00001000;
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("01111iiiiinnnttt", 0, registers::R3, registers::R0);
	getCpu().regs().set(registers::R3, INITIAL_PC + 4);
	getCpu().mem().write32(INITIAL_PC + 4, 0xFFFFFF7F);
	Step();
	ExpectThat().Register(registers::R0).Equals(0x7F);
}

MICROMACHINE_TEST_F(ldrbImmediate, AttemptLoadInvalidAddress, CpuTestFixture) {
	constexpr uint32_t INITIAL_PC = 0x00001000;
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("01111iiiiinnnttt", 0, registers::R3, registers::R0);
	getCpu().regs().set(registers::R3, 0xFFFFFFFC);
	Step();
	ExpectThat().HardfaultHandlerReached();
}
