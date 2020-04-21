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


/* LDR - Immediate Encoding T1
   Encoding: 011 0 1 Imm:5 Rn:3 Rt:3 */
MICROMACHINE_TEST_F(ldrImmediate, T1UseAMixOfRegistersWithSmallestOffset, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("01101iiiiinnnttt", 0, registers::R7, registers::R0);
	getCpu().regs().set(registers::R7, INITIAL_PC + 4);
	getCpu().mem().write32(INITIAL_PC + 4, 0xBAADFEED);
	Step();
	ExpectThat().Register(registers::R0).Equals(0xBAADFEED);
}

MICROMACHINE_TEST_F(ldrImmediate, T1UseAnotherMixOfRegistersWithLargestOffset, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	getCpu().regs().set(registers::R7, 0x77777777U);
	code_gen().emit_ins16("01101iiiiinnnttt", 31, registers::R0, registers::R7);
	getCpu().regs().set(registers::R0, INITIAL_PC);
	getCpu().mem().write32(INITIAL_PC + 31 * 4, 0xBAADFEED);
	Step();
	ExpectThat().Register(registers::R7).Equals(0xBAADFEED);
}

MICROMACHINE_TEST_F(ldrImmediate, T1AttemptUnalignedLoad, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	getCpu().regs().set(registers::R2, 0x22222222U);
	code_gen().emit_ins16("01101iiiiinnnttt", 0, registers::R3, registers::R2);
	getCpu().regs().set(registers::R3, INITIAL_PC + 2);
	Step();
	ExpectThat().HardfaultHandlerReached();
	ExpectThat().Register(registers::R2).DidNotChange();
}

MICROMACHINE_TEST_F(ldrImmediate, T1AttemptLoadFromInvalidAddress, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	getCpu().regs().set(registers::R2, 0x22222222U);
	code_gen().emit_ins16("01101iiiiinnnttt", 16, registers::R3, registers::R2);
	getCpu().regs().set(registers::R3, 0xFFFFFFFC - 16 * 4);
	Step();
	ExpectThat().HardfaultHandlerReached();
	ExpectThat().Register(registers::R2).DidNotChange();
}

/* LDR - Immediate Encoding T2 (SP is base register)
   Encoding: 1001 1 Rt:3 Imm:8 */
MICROMACHINE_TEST_F(ldrImmediate, T2UseHighestRegisterWithSmallestOffset, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("10011tttiiiiiiii", registers::R7, 0);
	getCpu().regs().set(registers::SP, INITIAL_PC + 1024);
	getCpu().mem().write32(INITIAL_PC + 1024, 0xBAADFEED);
	Step();
	ExpectThat().Register(registers::R7).Equals(0xBAADFEED);
}

MICROMACHINE_TEST_F(ldrImmediate, T2UseLowestRegisterWithLargestOffset, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("10011tttiiiiiiii", registers::R0, 255);
	getCpu().regs().set(registers::SP, INITIAL_PC + 1024);
	getCpu().mem().write32(INITIAL_PC + 1024 + 255 * 4, 0xBAADFEED);
	Step();
	ExpectThat().Register(registers::R0).Equals(0xBAADFEED);
}
/*
 * This tst is disabled because the implementation of SP can't store unaligned values
 * TODO: Fix this
 *
MICROMACHINE_TEST_F(ldrImmediate, T2AttemptUnalignedLoad, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("10011tttiiiiiiii", registers::R2, 0);
	getCpu().regs().set(registers::SP, INITIAL_PC + 1026);
	getCpu().mem().write32(INITIAL_PC + 1024, 0xBAADFEED);
	Step();
	ExpectThat().HardfaultHandlerReached();
	ExpectThat().Register(registers::R2).DidNotChange();
}

MICROMACHINE_TEST_F(ldrImmediate, T2AttemptLoadFromInvalidAddress, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	getCpu().regs().set(registers::R2, 0x22222222U);
	code_gen().emit_ins16("10011tttiiiiiiii", registers::R2, 0);
	getCpu().regs().set(registers::R3, 0xFFFFFFFC);
	Step();
	ExpectThat().HardfaultHandlerReached();
	ExpectThat().Register(registers::R2).DidNotChange();
}
*/
