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


/* LDRSB - Register
   Encoding: 0101 011 Rm:3 Rn:3 Rt:3 */
MICROMACHINE_TEST_F(ldrsbRegister, UseAMixOfRegistersWordAligned_NegativeValue, CpuTestFixture) {
	constexpr uint32_t INITIAL_PC = 0x00001000;
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("0101011mmmnnnttt", registers::R7, registers::R3, registers::R0);
	getCpu().regs().set(registers::R3, INITIAL_PC);
	getCpu().regs().set(registers::R7, 4);
	getCpu().mem().write32(INITIAL_PC + 4, 0xBAADFEED);
	Step();
	ExpectThat().Register(registers::R0).Equals(0xFFFFFFED);
}

MICROMACHINE_TEST_F(ldrsbRegister, UseAnotherMixOfRegistersSecondByteInWord_NegativeValue, CpuTestFixture) {
	constexpr uint32_t INITIAL_PC = 0x00001000;
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("0101011mmmnnnttt", registers::R1, registers::R0, registers::R7);
	getCpu().regs().set(registers::R0, INITIAL_PC);
	getCpu().regs().set(registers::R1, 5);
	getCpu().mem().write32(INITIAL_PC + 4, 0xBAADFEED);
	Step();
	ExpectThat().Register(registers::R7).Equals(0xFFFFFFFE);
}

MICROMACHINE_TEST_F(ldrsbRegister, YetAnotherMixOfRegistersThirdByteInWord_NegativeValue, CpuTestFixture) {
	constexpr uint32_t INITIAL_PC = 0x00001000;
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("0101011mmmnnnttt", registers::R0, registers::R7, registers::R4);
	getCpu().regs().set(registers::R7, INITIAL_PC);
	getCpu().regs().set(registers::R0, 6);
	getCpu().mem().write32(INITIAL_PC + 4, 0xBAADFEED);
	Step();
	ExpectThat().Register(registers::R4).Equals(0xFFFFFFAD);
}

MICROMACHINE_TEST_F(ldrsbRegister, YetAnotherMixOfRegistersFourthByteInWord_NegativeValue, CpuTestFixture) {
	constexpr uint32_t INITIAL_PC = 0x00001000;
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("0101011mmmnnnttt", registers::R0, registers::R7, registers::R5);
	getCpu().regs().set(registers::R7, INITIAL_PC);
	getCpu().regs().set(registers::R0, 7);
	getCpu().mem().write32(INITIAL_PC + 4, 0xBAADFEED);
	Step();
	ExpectThat().Register(registers::R5).Equals(0xFFFFFFBA);
}

MICROMACHINE_TEST_F(ldrsbRegister, LoadAPositiveValue, CpuTestFixture) {
	constexpr uint32_t INITIAL_PC = 0x00001000;
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("0101011mmmnnnttt", registers::R7, registers::R3, registers::R0);
	getCpu().regs().set(registers::R3, INITIAL_PC);
	getCpu().regs().set(registers::R7, 4);
	getCpu().mem().write32(INITIAL_PC + 4, 0xFFFFFF7F);
	Step();
	ExpectThat().Register(registers::R0).Equals(0x7F);
}

MICROMACHINE_TEST_F(ldrsbRegister, AttemptLoadInvalidAddress, CpuTestFixture) {
	constexpr uint32_t INITIAL_PC = 0x00001000;
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("0101011mmmnnnttt", registers::R7, registers::R3, registers::R0);
	getCpu().regs().set(registers::R3, 0xFFFFFFFC);
	getCpu().regs().set(registers::R7, 0);
	Step();
	ExpectThat().HardfaultHandlerReached();
}
