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


/* ADC - Register (ADd with Carry)
   Encoding: 010000 0101 Rm:3 Rdn:3 */
MICROMACHINE_TEST_F(adcRegister, UserR1ForAllArgs, CpuTestFixture) {

	getCpu().regs().set(registers::R1, 0x11111111U);

	code_gen().emit_adc(registers::R1, registers::R1);

	ExpectThat().APSRFlagsMatches("c"); // carry must be clear

	Step();

	ExpectThat()
		.Register(registers::R1).Equals(0x11111111U + 0x11111111U)
		.InstructionExecutedWithoutBranch()
		.APSRFlagsMatches("nzcv");
}


MICROMACHINE_TEST_F(adcRegister, UseLowestRegisterForAllArgs, CpuTestFixture) {

	code_gen().emit_adc(registers::R0, registers::R0);

	ExpectThat().APSRFlagsMatches("c"); // carry must be clear
	Step();

	ExpectThat()
		.APSRFlagsMatches("nZcv")
		.Register(registers::R0).Equals(0U);
}

MICROMACHINE_TEST_F(adcRegister, UseHigestRegisterForAllArgsPositiveOverflow, CpuTestFixture) {
	code_gen().emit_adc(registers::R7, registers::R7);

	getCpu().regs().set(registers::R7, 0x77777777U);

	ExpectThat().APSRFlagsMatches("c"); // carry must be clear
	Step();

	ExpectThat()
		.APSRFlagsMatches("NzcV")
		.Register(registers::R7).Equals(0x77777777U + 0x77777777U);
}

MICROMACHINE_TEST_F(adcRegister, UseDifferentRegistersForEachArg, CpuTestFixture) {
	code_gen().emit_adc(registers::R1, registers::R2);

	getCpu().regs().set(registers::R1, 0x11111111U);
	getCpu().regs().set(registers::R2, 0x22222222U);

	ExpectThat().APSRFlagsMatches("c"); // carry must be clear
	Step();

	ExpectThat()
		.APSRFlagsMatches("nzcv")
		.Register(registers::R2).Equals(0x11111111U + 0x22222222U);
}

MICROMACHINE_TEST_F(adcRegister, Add0to0WithCarryInSetToGiveAResultOf1, CpuTestFixture) {
	code_gen().emit_adc(registers::R0, registers::R0);

	// set the carry flag
	getCpu().special_regs().app_status_register().write_carry_flag(true);

	Step();

	ExpectThat()
		.APSRFlagsMatches("nzcv")
		.Register(registers::R0).Equals(0U + 0U + 1U);
}

// Force APSR flags to be set which haven't already been covered above.
MICROMACHINE_TEST_F(adcRegister, ForceCarryOut, CpuTestFixture) {
	code_gen().emit_adc(registers::R1, registers::R2);

	getCpu().regs().set(registers::R1, -1);
	getCpu().regs().set(registers::R2, 1);

	ExpectThat().APSRFlagsMatches("c"); // carry must be clear
	Step();

	ExpectThat()
		.Register(registers::R2).Equals(-1 + 1)
		.APSRFlagsMatches("nZCv");
}

MICROMACHINE_TEST_F(adcRegister, ForceCarryOutAndOverflow, CpuTestFixture) {
	code_gen().emit_adc(registers::R1, registers::R2);

	getCpu().regs().set(registers::R1, -1);
	getCpu().regs().set(registers::R2, 0x80000000U);

	ExpectThat().APSRFlagsMatches("c"); // carry must be clear
	Step();

	ExpectThat()
		.Register(registers::R2).Equals(0x7FFFFFFF)
		.APSRFlagsMatches("nzCV");
}

