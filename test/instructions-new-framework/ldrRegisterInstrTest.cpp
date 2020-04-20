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


/* LDR - Register
   Encoding: 0101 100 Rm:3 Rn:3 Rt:3 */
MICROMACHINE_TEST_F(ldrRegister, UseAMixOfRegisters, CpuTestFixture) {
	code_gen().emit_ins16("0101100mmmnnnttt", registers::R7, registers::R3, registers::R0);
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	getCpu().regs().set(registers::R3, INITIAL_PC);
	getCpu().regs().set(registers::R7, 4);
	getCpu().mem().write32(INITIAL_PC + 4, 0xBAADFEED);
	Step();
	ExpectThat().Register(registers::R0).Equals(0xBAADFEED);
}

MICROMACHINE_TEST_F(ldrRegister, UseAnotherMixOfRegisters, CpuTestFixture) {
	code_gen().emit_ins16("0101100mmmnnnttt", registers::R1, registers::R0, registers::R7);
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	getCpu().regs().set(registers::R0, INITIAL_PC);
	getCpu().regs().set(registers::R1, 4);
	getCpu().mem().write32(INITIAL_PC + 4, 0xBAADFEED);
	Step();
	ExpectThat().Register(registers::R7).Equals(0xBAADFEED);
}

MICROMACHINE_TEST_F(ldrRegister, YetAnotherMixOfRegisters, CpuTestFixture) {
	code_gen().emit_ins16("0101100mmmnnnttt", registers::R0, registers::R7, registers::R4);
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	getCpu().regs().set(registers::R7, INITIAL_PC);
	getCpu().regs().set(registers::R0, 4);
	getCpu().mem().write32(INITIAL_PC + 4, 0xBAADFEED);
	Step();
	ExpectThat().Register(registers::R4).Equals(0xBAADFEED);
}

MICROMACHINE_TEST_F(ldrRegister, AttemptUnalignedLoad, CpuTestFixture) {
	code_gen().emit_ins16("0101100mmmnnnttt", registers::R7, registers::R3, registers::R0);
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	getCpu().regs().set(registers::R3, INITIAL_PC);
	getCpu().regs().set(registers::R7, 2);
	Step();
	ExpectThat().HardfaultHandlerReached();
}

MICROMACHINE_TEST_F(ldrRegister, AttemptLoadFromInvalidAddress, CpuTestFixture) {
	code_gen().emit_ins16("0101100mmmnnnttt", registers::R7, registers::R3, registers::R0);
	getCpu().regs().set(registers::R3, 0xFFFFFFFC);
	getCpu().regs().set(registers::R7, 0);
	Step();
	ExpectThat().HardfaultHandlerReached();
}
