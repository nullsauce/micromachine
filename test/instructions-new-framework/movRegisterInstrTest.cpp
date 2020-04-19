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


/* MOV - Register Encoding 1
   Encoding: 010001 10 D:1 Rm:4 Rd:3
   NOTE: This encoding doesn't update the APSR flags. */
MICROMACHINE_TEST_F(movRegister, UseLowestRegisterForAllArgs, CpuTestFixture) {
	code_gen().emit_ins16("01000110dmmmmddd", registers::R0, registers::R0);
	Step();
}

MICROMACHINE_TEST_F(movRegister, UseHighRegisterForAllArgs, CpuTestFixture) {
	code_gen().emit_ins16("01000110dmmmmddd", registers::LR, registers::LR);
	Step();
}

MICROMACHINE_TEST_F(movRegister, MoveHighRegisterToLowRegister, CpuTestFixture) {
	code_gen().emit_ins16("01000110dmmmmddd", registers::R7, registers::R12);
	Step();
	ExpectThat().Register(registers::R7).Equals(0xCCCCCCCC);
}

MICROMACHINE_TEST_F(movRegister, MoveLowRegisterToLHighRegister, CpuTestFixture) {
	code_gen().emit_ins16("01000110dmmmmddd", registers::R12, registers::R7);
	Step();
	ExpectThat().Register(registers::R12).Equals(0x77777777);
}

MICROMACHINE_TEST_F(movRegister, MoveOddAddressIntoPCAndMakeSureLSbitIsCleared, CpuTestFixture) {
	constexpr uint32_t INITIAL_PC = 0x00001000;
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("01000110dmmmmddd", registers::PC, registers::R1);
	getCpu().regs().set(registers::R1, INITIAL_PC + 1025);
	Step();
	ExpectThat().Register(registers::PC).Equals(INITIAL_PC + 1024);
}

MICROMACHINE_TEST_F(movRegister, MoveEvenAddressIntoPC, CpuTestFixture) {
	constexpr uint32_t INITIAL_PC = 0x00001000;
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("01000110dmmmmddd", registers::PC, registers::R2);
	getCpu().regs().set(registers::R2, INITIAL_PC + 1024);
	Step();
	ExpectThat().Register(registers::PC).Equals(INITIAL_PC + 1024);
}

MICROMACHINE_TEST_F(movRegister, MovePCintoOtherRegister, CpuTestFixture) {
	constexpr uint32_t INITIAL_PC = 0x00001000;
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("01000110dmmmmddd", registers::R3, registers::PC);
	ExpectThat().Register(registers::R3).Equals(INITIAL_PC + 4);
	Step();
}
