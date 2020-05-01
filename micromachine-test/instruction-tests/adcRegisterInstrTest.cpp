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

/* ADC - Register (ADd with Carry)
   Encoding: 010000 0101 Rm:3 Rdn:3 */
MICROMACHINE_TEST_F(adcRegister, UserR1ForAllArgs, CpuTestFixture) {
	emitInstruction16("0100000101mmmddd", registers::R1, registers::R1);

	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzcv"),
													reg(registers::R1).equals(0x11111111U + 0x11111111U));
}

MICROMACHINE_TEST_F(adcRegister, UseLowestRegisterForAllArgs, CpuTestFixture) {
	emitInstruction16("0100000101mmmddd", registers::R0, registers::R0);

	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZcv"), reg(registers::R0).equals(0U));
}

MICROMACHINE_TEST_F(adcRegister, UseHigestRegisterForAllArgsPositiveOverflow, CpuTestFixture) {
	emitInstruction16("0100000101mmmddd", registers::R7, registers::R7);

	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("NzcV"),
													reg(registers::R7).equals(0x77777777U + 0x77777777U));
}

MICROMACHINE_TEST_F(adcRegister, UseDifferentRegistersForEachArg, CpuTestFixture) {
	emitInstruction16("0100000101mmmddd", registers::R1, registers::R2);

	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzcv"),
													reg(registers::R2).equals(0x11111111U + 0x22222222U));
}

MICROMACHINE_TEST_F(adcRegister, Add0to0WithCarryInSetToGiveAResultOf1, CpuTestFixture) {
	emitInstruction16("0100000101mmmddd", registers::R0, registers::R0);

	setCarryFlag(true);

	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzcv"), reg(registers::R0).equals(0U + 0U + 1U));
}

// Force APSR flags to be set which haven't already been covered above.
MICROMACHINE_TEST_F(adcRegister, ForceCarryOut, CpuTestFixture) {
	emitInstruction16("0100000101mmmddd", registers::R1, registers::R2);

	setReg(registers::R1, -1);
	setReg(registers::R2, 1);

	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZCv"), reg(registers::R2).equals(-1 + 1));
}

MICROMACHINE_TEST_F(adcRegister, ForceCarryOutAndOverflow, CpuTestFixture) {
	emitInstruction16("0100000101mmmddd", registers::R1, registers::R2);

	setReg(registers::R1, -1);
	setReg(registers::R2, 0x80000000U);

	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzCV"), reg(registers::R2).equals(0x7FFFFFFF));
}