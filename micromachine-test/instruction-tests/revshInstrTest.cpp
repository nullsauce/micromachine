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
using namespace micromachine::testing;

/* REVSH
   Encoding: 1011 1010 11 Rm:3 Rd:3 */
MICROMACHINE_TEST_F(revsh, RevR0toR7, CpuTestFixture) {
	emitInstruction16("1011101011mmmddd", registers::R0, registers::R7);
	setReg(registers::R0, 0x12345678);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R7).equals(0x7856));
}

MICROMACHINE_TEST_F(revsh, RevR7toR0, CpuTestFixture) {
	emitInstruction16("1011101011mmmddd", registers::R7, registers::R0);
	setReg(registers::R7, 0x12345678);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R0).equals(0x7856));
}

MICROMACHINE_TEST_F(revsh, PositiveValue, CpuTestFixture) {
	emitInstruction16("1011101011mmmddd", registers::R7, registers::R0);
	setReg(registers::R7, 0xFF7F);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R0).equals(0x7FFF));
}

MICROMACHINE_TEST_F(revsh, NegativeValue, CpuTestFixture) {
	emitInstruction16("1011101011mmmddd", registers::R7, registers::R0);
	setReg(registers::R7, 0x0080);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R0).equals(0xFFFF8000));
}
