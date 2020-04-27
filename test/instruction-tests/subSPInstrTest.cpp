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


/* SUB registers::SP Minus Immediate
   Encoding: 1011 0000 1 Imm:7 */
MICROMACHINE_TEST_F(subSP, SmallestImmediate, CpuTestFixture) {
	constexpr uint32_t INITIAL_SP = 0x00002000;
	getCpu().regs().set_sp(INITIAL_SP);
	code_gen().emit_ins16("101100001iiiiiii", 0);
	Step();
	ExpectThat().Register(registers::SP).Equals(INITIAL_SP - 0);
}

MICROMACHINE_TEST_F(subSP, LargestImmediate, CpuTestFixture) {
	constexpr uint32_t INITIAL_SP = 0x00002000;
	getCpu().regs().set_sp(INITIAL_SP);
	code_gen().emit_ins16("101100001iiiiiii", 127);
	Step();
	ExpectThat().Register(registers::SP).Equals(INITIAL_SP - 127 * 4);
}

MICROMACHINE_TEST_F(subSP, UseIntermediateValues, CpuTestFixture) {
	constexpr uint32_t INITIAL_SP = 0x00002000;
	getCpu().regs().set_sp(INITIAL_SP);
	code_gen().emit_ins16("101100001iiiiiii", 64);
	Step();
	ExpectThat().Register(registers::SP).Equals(INITIAL_SP - 64 * 4);
}
