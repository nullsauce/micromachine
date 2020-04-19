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


/* ADD SP Plus Immediate - Encoding T1
   Encoding: 1010 1 Rd:3 Imm:8 */
MICROMACHINE_TEST_F(addSP, T1UseHighestRegisterAddSmallestImmediate, CpuTestFixture) {
	static uint32_t INITIAL_SP = 0x00002000;
	getCpu().regs().set(registers::SP, INITIAL_SP);

	code_gen().emit_ins16("10101dddiiiiiiii", registers::R7, 0);
	Step();
	ExpectThat().Register(registers::R7).Equals(INITIAL_SP + 0);
}

MICROMACHINE_TEST_F(addSP, T1UseLowestRegisterAddLargestImmediate, CpuTestFixture) {
	static uint32_t INITIAL_SP = 0x00002000;
	getCpu().regs().set(registers::SP, INITIAL_SP);

	code_gen().emit_ins16("10101dddiiiiiiii", registers::R0, 255);
	Step();
	ExpectThat().Register(registers::R0).Equals(INITIAL_SP + 255 * 4);
}

MICROMACHINE_TEST_F(addSP, T1UseIntermediateValues, CpuTestFixture) {
	static uint32_t INITIAL_SP = 0x00002000;
	getCpu().regs().set(registers::SP, INITIAL_SP);

	code_gen().emit_ins16("10101dddiiiiiiii", registers::R3, 128);
	Step();
	ExpectThat().Register(registers::R3).Equals(INITIAL_SP + 128 * 4);
}

/* ADD SP Plus Immediate - Encoding T2
   Encoding: 1011 0000 0 Imm:7 */
MICROMACHINE_TEST_F(addSP, T2SmallestImmediate, CpuTestFixture) {
	static uint32_t INITIAL_PC = 0x00001000;
	getCpu().regs().set_pc(INITIAL_PC);
	getCpu().regs().set(registers::SP, INITIAL_PC + 1024);

	code_gen().emit_ins16("101100000iiiiiii", 0);

	Step();
	ExpectThat().Register(registers::SP).Equals(INITIAL_PC + 1024 + 0);
}

MICROMACHINE_TEST_F(addSP, T2LargestImmediate, CpuTestFixture) {
	static uint32_t INITIAL_PC = 0x00001000;
	getCpu().regs().set_pc(INITIAL_PC);
	getCpu().regs().set(registers::SP, INITIAL_PC + 1024);

	code_gen().emit_ins16("101100000iiiiiii", 127);

	Step();
	ExpectThat().Register(registers::SP).Equals(INITIAL_PC + 1024 + 127 * 4);
}

MICROMACHINE_TEST_F(addSP, T2IntermediateValues, CpuTestFixture) {
	static uint32_t INITIAL_PC = 0x00001000;
	getCpu().regs().set_pc(INITIAL_PC);
	getCpu().regs().set(registers::SP, INITIAL_PC + 1024);

	code_gen().emit_ins16("101100000iiiiiii", 64);

	Step();
	ExpectThat().Register(registers::SP).Equals(INITIAL_PC + 1024 + 64 * 4);
}
