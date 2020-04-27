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


/* ADD - Register - Encoding T1
   Encoding: 000 11 0 0 Rm:3 Rn:3 Rd:3 */
MICROMACHINE_TEST_F(addRegister, T1UseLowestRegisterForAllArgs, CpuTestFixture) {
	code_gen().emit_add_t1(registers::R0, registers::R0, registers::R0);
	Step();
	ExpectThat()
		.APSRFlagsMatches("nZcv")
		.Register(registers::R0).Equals(0U);
}

MICROMACHINE_TEST_F(addRegister, T1UseHigestRegisterForAllArgs, CpuTestFixture) {
	code_gen().emit_add_t1(registers::R7, registers::R7, registers::R7);

	getCpu().regs().set(registers::R7, 0x77777777U);

	Step();

	ExpectThat()
		.APSRFlagsMatches("NzcV")
		.Register(registers::R7).Equals(0x77777777U + 0x77777777U);
}

MICROMACHINE_TEST_F(addRegister, T1UseDifferentRegistersForEachArg, CpuTestFixture) {
	code_gen().emit_add_t1(registers::R1, registers::R2, registers::R3);

	getCpu().regs().set(registers::R1, 0x11111111U);
	getCpu().regs().set(registers::R2, 0x22222222U);

	Step();

	ExpectThat()
		.APSRFlagsMatches("nzcv")
		.Register(registers::R3).Equals(0x11111111U + 0x22222222U);
}

// Force APSR flags to be set which haven't already been covered above.
MICROMACHINE_TEST_F(addRegister, T1ForceCarryWithNoOverflow, CpuTestFixture) {
	code_gen().emit_add_t1(registers::R1, registers::R2, registers::R0);

	getCpu().regs().set(registers::R1, -1);
	getCpu().regs().set(registers::R2, 1);

	Step();

	ExpectThat()
		.APSRFlagsMatches("nZCv")
		.Register(registers::R0).Equals(-1 + 1);
}

MICROMACHINE_TEST_F(addRegister, T1ForceCarryAndOverflow, CpuTestFixture) {
	code_gen().emit_add_t1(registers::R1, registers::R2, registers::R0);

	getCpu().regs().set(registers::R1, -1);
	getCpu().regs().set(registers::R2, 0x80000000U);

	Step();

	ExpectThat()
		.APSRFlagsMatches("nzCV")
		.Register(registers::R0).Equals(0x7FFFFFFFU);
}



/* ADD - Register - Encoding T2
   Encoding: 010001 00 DN:1 Rm:4 Rdn:3
   NOTE: Shouldn't modify any of the APSR flags.*/
MICROMACHINE_TEST_F(addRegister, T2UseR1ForAllArgs, CpuTestFixture) {

	code_gen().emit_add_t2(registers::R1, registers::R1);

	getCpu().regs().set(registers::R1, 0x11111111U);

	Step();

	ExpectThat()
		.Register(registers::R1).Equals(0x11111111U + 0x11111111U)
		.APSRFlagsDidNotChange();

}

MICROMACHINE_TEST_F(addRegister, T2UseLowestRegisterForAllArgs, CpuTestFixture) {
	code_gen().emit_add_t2(registers::R0, registers::R0);

	Step();

	ExpectThat()
		.Register(registers::R1).Equals(0)
		.APSRFlagsDidNotChange();
}

MICROMACHINE_TEST_F(addRegister, T2UseForR12AllArgs, CpuTestFixture) {
	code_gen().emit_add_t2(registers::R12, registers::R12);

	getCpu().regs().set(registers::R12, 0xCCCCCCCCU);

	Step();

	ExpectThat()
		.Register(registers::R12).Equals(0xCCCCCCCCU + 0xCCCCCCCCU)
		.APSRFlagsDidNotChange();
}

MICROMACHINE_TEST_F(addRegister, T2UseDifferentRegistersForEachArg, CpuTestFixture) {
	code_gen().emit_add_t2(registers::R2, registers::R1);

	getCpu().regs().set(registers::R1, 0x11111111U);
	getCpu().regs().set(registers::R2, 0x22222222U);

	Step();

	ExpectThat()
		.Register(registers::R2).Equals(0x11111111U + 0x22222222U)
		.APSRFlagsDidNotChange();
}

MICROMACHINE_TEST_F(addRegister, T2WrapAroundTo0, CpuTestFixture) {
	code_gen().emit_add_t2(registers::R2, registers::R1);
	getCpu().regs().set(registers::R1, -1);
	getCpu().regs().set(registers::R2, 1);

	Step();

	ExpectThat()
		.Register(registers::R2).Equals(-1 + 1)
		.APSRFlagsDidNotChange();
}

MICROMACHINE_TEST_F(addRegister, T2OverflowFromLowestNegativeValue, CpuTestFixture) {
	code_gen().emit_add_t2(registers::R11, registers::R10);
	getCpu().regs().set(registers::R10, -1);
	getCpu().regs().set(registers::R11, 0x80000000U);

	Step();

	ExpectThat()
		.Register(registers::R11).Equals(0x7FFFFFFF)
		.APSRFlagsDidNotChange();
}

MICROMACHINE_TEST_F(addRegister, T2Add4ToSP, CpuTestFixture) {
	constexpr uint32_t INITIAL_SP = 0x00002000;

	code_gen().emit_add_t2(registers::SP, registers::R1);
	getCpu().regs().set(registers::SP, INITIAL_SP - 4);
	getCpu().regs().set(registers::R1, 4);

	Step();

	ExpectThat()
		.Register(registers::SP).Equals(INITIAL_SP - 4 + 4)
		.APSRFlagsDidNotChange();
}

MICROMACHINE_TEST_F(addRegister, T2Subtract4FromSP, CpuTestFixture) {
	code_gen().emit_add_t2(registers::SP, registers::R1);
	getCpu().regs().set(registers::R1, -4);

	Step();

	ExpectThat()
		.Register(registers::SP).Equals(TestSystem::INITIAL_SP - 4)
		.APSRFlagsDidNotChange();

}

MICROMACHINE_TEST_F(addRegister, T2Add1ToLR, CpuTestFixture) {
	code_gen().emit_add_t2(registers::LR, registers::R1);

	constexpr uint32_t INITIAL_LR = 0xFFFFFFFF;
	getCpu().regs().set(registers::LR, INITIAL_LR);

	getCpu().regs().set(registers::R1, 1);

	Step();

	ExpectThat()
		.Register(registers::LR).Equals(INITIAL_LR + 1)
		.APSRFlagsDidNotChange();
}

MICROMACHINE_TEST_F(addRegister, T2Add1ToPCWhichWillBeOddAndRoundedDown, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();

	code_gen().emit_add_t2(registers::PC, registers::R1);
	getCpu().regs().set(registers::PC, INITIAL_PC);

	getCpu().regs().set(registers::R1, 1);

	Step();

	ExpectThat()
		.Register(registers::PC).Equals((INITIAL_PC + 4 + 1) & 0xFFFFFFFE)
		.APSRFlagsDidNotChange();
}

MICROMACHINE_TEST_F(addRegister, T2Add2ToPC, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();

	code_gen().emit_add_t2(registers::PC, registers::R1);
	getCpu().regs().set(registers::PC, INITIAL_PC);

	getCpu().regs().set(registers::R1, 2);

	Step();

	ExpectThat()
		.Register(registers::PC).Equals((INITIAL_PC + 4 + 2) & 0xFFFFFFFE)
		.APSRFlagsDidNotChange();;
}

/*
TEST_SIM_ONLY(addRegister, T2ItIsUnpredictableToHaveBothArgsBePC)
{
    code_gen().emit_add_t2(registers::PC, registers::PC);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    .Register(registers::PC).Equals(INITIAL_PC);
    step(&m_context);
}
*/
