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

//#include "CpuTestHarness.hpp"
#include "CpuTestFixture.hpp"
#include "RegisterMutationPredicate.hpp"


/* ADC - Register (ADd with Carry)
   Encoding: 010000 0101 Rm:3 Rdn:3 */
MICROMACHINE_TEST_F(adcRegister, UseR1ForAllArgs, CpuTestFixture)
{
	ExpectThat().XPSRFlagsEquals("c"); // carry must be clear

	getCpu().regs().set(registers::R1, 0x11111111U);

	code_gen().emit_ins16("0100000101mmmddd", registers::R1, registers::R1);

	Step();
	ExpectThat()
		.Register(registers::R1).Equals(0x11111111U + 0x11111111U)
		.InstructionExecutedWithoutBranch()
		.XPSRFlagsEquals("nzcv");

}
/*
TEST_F(CpuTestHarness, adcRegister_UseR1ForAllArgs)
{
	code_gen().emit_ins16("0100000101mmmddd", R1, R1);
	setExpectedXPSRflags("nzcv");
	setExpectedRegisterValue(R1, 0x11111111U + 0x11111111U);
	// Carry In state is important for ADC tests.
	clearCarry();
	step();
}

TEST_F(CpuTestHarness, adcRegister_UseLowestRegisterForAllArgs)
{
	code_gen().emit_ins16("0100000101mmmddd", R0, R0);
	setExpectedXPSRflags("nZcv");
	setExpectedRegisterValue(R0, 0U);
	clearCarry();
	step();
}

TEST_F(CpuTestHarness, adcRegister_UseHigestRegisterForAllArgsPositiveOverflow)
{
	code_gen().emit_ins16("0100000101mmmddd", R7, R7);
	setExpectedXPSRflags("NzcV");
	setExpectedRegisterValue(R7, 0x77777777U + 0x77777777U);
	clearCarry();
	step();
}

TEST_F(CpuTestHarness, adcRegister_UseDifferentRegistersForEachArg)
{
	code_gen().emit_ins16("0100000101mmmddd", R1, R2);
	setExpectedXPSRflags("nzcv");
	setExpectedRegisterValue(R2, 0x11111111U + 0x22222222U);
	clearCarry();
	step();
}

TEST_F(CpuTestHarness, adcRegister_Add0to0WithCarryInSetToGiveAResultOf1)
{
	code_gen().emit_ins16("0100000101mmmddd", R0, R0);
	setExpectedXPSRflags("nzcv");
	setExpectedRegisterValue(R0, 0U + 0U + 1U);
	setCarry();
	step();
}

// Force APSR flags to be set which haven't already been covered above.
TEST_F(CpuTestHarness, adcRegister_ForceCarryOut)
{
	code_gen().emit_ins16("0100000101mmmddd", R1, R2);
	setExpectedXPSRflags("nZCv");
	setRegisterValue(R1, -1);
	setRegisterValue(R2, 1);
	setExpectedRegisterValue(R2, -1 + 1);
	clearCarry();
	step();
}

TEST_F(CpuTestHarness, adcRegister_ForceCarryOutAndOverflow)
{
	code_gen().emit_ins16("0100000101mmmddd", R1, R2);
	setExpectedXPSRflags("nzCV");
	setRegisterValue(R1, -1);
	setRegisterValue(R2, 0x80000000U);
	setExpectedRegisterValue(R2, 0x7FFFFFFF);
	clearCarry();
	step();
}
*/
