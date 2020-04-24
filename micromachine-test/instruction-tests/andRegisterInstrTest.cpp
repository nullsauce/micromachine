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

/* AND - Register
   Encoding: 010000 0000 Rm:3 Rdn:3 */
/* NOTE: APSR_C state is maintained by this instruction. */
MICROMACHINE_TEST_F(andRegister, UseLowestRegisterForBothArgs, CpuTestFixture) {
	emitInstruction16("0100000000mmmddd", registers::R0, registers::R0);
	// Use a couple of tests to explicitly set/clear carry to verify both states are maintained.
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZcv"), reg(registers::R0).equals(0));
}

MICROMACHINE_TEST_F(andRegister, UseHighestRegisterForBothArgs, CpuTestFixture) {
	emitInstruction16("0100000000mmmddd", registers::R7, registers::R7);

	// check that carry remains set
	setCarryFlag(true);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R7).equals(0x77777777), apsrFlagsEquals("nzCv"));
}

MICROMACHINE_TEST_F(andRegister, AndR3andR7, CpuTestFixture) {
	emitInstruction16("0100000000mmmddd", registers::R3, registers::R7);

	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzcv"), reg(registers::R7).equals(0x33333333));
}

MICROMACHINE_TEST_F(andRegister, UseAndToJustKeepNegativeSignBit, CpuTestFixture) {
	emitInstruction16("0100000000mmmddd", registers::R6, registers::R1);
	setReg(registers::R1, -1);
	setReg(registers::R6, 0x80000000);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("Nzcv"), reg(registers::R1).equals(0x80000000));
}

MICROMACHINE_TEST_F(andRegister, HaveAndResultNotBeSameAsEitherSource, CpuTestFixture) {
	emitInstruction16("0100000000mmmddd", registers::R5, registers::R2);
	setReg(registers::R2, 0x12345678);
	setReg(registers::R5, 0xF0F0F0F0);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzcv"), reg(registers::R2).equals(0x10305070));
}
