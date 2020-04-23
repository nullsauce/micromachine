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
#include "binops.hpp"

/* SYSm field values for MSR and MRS instructions. */
#define SYS_APSR    0
#define SYS_IAPSR   1
#define SYS_EAPSR   2
#define SYS_XPSR    3
#define SYS_IPSR    5
#define SYS_EPSR    6
#define SYS_IEPSR   7
#define SYS_MSP     8
#define SYS_PSP     9
#define SYS_PRIMASK 16
#define SYS_CONTROL 20


/* MSR
   Encoding: 11110 0 1110 0 (0) Rn:4
             10 (0) 0 (1) (0) (0) (0) SYSm:8 */
MICROMACHINE_TEST_F(msr, ToAPSR, CpuTestFixture) {

	code_gen().emit_ins32("111100111000nnnn", "10001000ssssssss", registers::R12, SYS_APSR);
	getCpu().special_regs().app_status_register().write_neg_flag(false);
	getCpu().special_regs().app_status_register().write_zero_flag(false);
	getCpu().special_regs().app_status_register().write_carry_flag(false);
	getCpu().special_regs().app_status_register().write_overflow_flag(false);
	getCpu().regs().set(registers::R12, 0xFFFFFFFF);

	Step();
	ExpectThat()
		.Register(registers::PC).WasIncrementedBy(4)
		.APSRFlagsMatches("NZCV");
}

MICROMACHINE_TEST_F(msr, ToIAPSR, CpuTestFixture) {

	code_gen().emit_ins32("111100111000nnnn", "10001000ssssssss", registers::R12, SYS_IAPSR);
	getCpu().special_regs().app_status_register().write_zero_flag(false);
	getCpu().special_regs().app_status_register().write_carry_flag(false);
	getCpu().special_regs().app_status_register().write_overflow_flag(false);
	getCpu().regs().set(registers::R12, 0xFFFFFFFF);
	getCpu().special_regs().app_status_register().write_neg_flag(false);

	Step();
	ExpectThat()
		.Register(registers::PC).WasIncrementedBy(4)
		.APSRFlagsMatches("NZCV");
}

MICROMACHINE_TEST_F(msr, ToEAPSR, CpuTestFixture) {
	code_gen().emit_ins32("111100111000nnnn", "10001000ssssssss", registers::R12, SYS_EAPSR);

	getCpu().special_regs().app_status_register().write_neg_flag(false);
	getCpu().special_regs().app_status_register().write_zero_flag(false);
	getCpu().special_regs().app_status_register().write_carry_flag(false);
	getCpu().special_regs().app_status_register().write_overflow_flag(false);
	getCpu().regs().set(registers::R12, 0xFFFFFFFF);

	Step();
	ExpectThat()
		.Register(registers::PC).WasIncrementedBy(4)
		.APSRFlagsMatches("NZCV");
}

MICROMACHINE_TEST_F(msr, ToXPSR, CpuTestFixture) {
	code_gen().emit_ins32("111100111000nnnn", "10001000ssssssss", registers::R0, SYS_XPSR);
	getCpu().special_regs().app_status_register().write_neg_flag(false);
	getCpu().special_regs().app_status_register().write_zero_flag(false);
	getCpu().special_regs().app_status_register().write_carry_flag(false);
	getCpu().special_regs().app_status_register().write_overflow_flag(false);
	getCpu().regs().set(registers::R0, 0xFFFFFFFF);

	Step();
	ExpectThat()
		.Register(registers::PC).WasIncrementedBy(4)
		.APSRFlagsMatches("NZCV");
}

MICROMACHINE_TEST_F(msr, ToIPSRIsIgnored, CpuTestFixture) {
	code_gen().emit_ins32("111100111000nnnn", "10001000ssssssss", registers::R12, SYS_IPSR);
	getCpu().regs().set(registers::R12, 0xFFFFFFFF);

	Step();
	ExpectThat()
		.Register(registers::PC).WasIncrementedBy(4)
		.IPSRFlagsDidNotChange();
}

MICROMACHINE_TEST_F(msr, ToEPSRIsIgnored, CpuTestFixture) {
	code_gen().emit_ins32("111100111000nnnn", "10001000ssssssss", registers::R12, SYS_EPSR);
	getCpu().regs().set(registers::R12, 0xFFFFFFFF);

	Step();
	ExpectThat()
		.Register(registers::PC).WasIncrementedBy(4)
		.EPSRFlagsDidNotChange();
}

MICROMACHINE_TEST_F(msr, ToIEPSRIsIgnored, CpuTestFixture) {
	code_gen().emit_ins32("111100111000nnnn", "10001000ssssssss", registers::R12, SYS_IEPSR);
	getCpu().regs().set(registers::R12, 0xFFFFFFFF);

	Step();
	ExpectThat()
		.Register(registers::PC).WasIncrementedBy(4)
		.EPSRFlagsDidNotChange()
		.IPSRFlagsDidNotChange();
}

MICROMACHINE_TEST_F(msr, ToMSP, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	const uint32_t spValue = INITIAL_PC + 1024 + 2;
	code_gen().emit_ins32("111100111000nnnn", "10001000ssssssss", registers::R12, SYS_MSP);
	getCpu().regs().set(registers::R12, spValue);

	// the sp value is aligned to lowest 4 bytes address boundary
	uint32_t expectedSpValue = binops::aligned<4>(spValue);

	Step();
	EXPECT_EQ(expectedSpValue, getCpu().regs().sp_register().get_specific_banked_sp(
				  micromachine::system::sp_reg::StackType::Main));
	EXPECT_EQ(0U, getCpu().regs().sp_register().get_specific_banked_sp(
				  micromachine::system::sp_reg::StackType::Process));
	ExpectThat()
		.Register(registers::SP).Equals(expectedSpValue)
		.Register(registers::PC).WasIncrementedBy(4);
}

MICROMACHINE_TEST_F(msr, ToPSP, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	const uint32_t spValue = INITIAL_PC + 1024 + 2;
	code_gen().emit_ins32("111100111000nnnn", "10001000ssssssss", registers::R12, SYS_PSP);
	getCpu().regs().set(registers::R12, spValue);

	// the sp value is aligned to lowest 4 bytes address boundary
	uint32_t expectedSpValue = binops::aligned<4>(spValue);

	Step();
	EXPECT_EQ(expectedSpValue, getCpu().regs().sp_register().get_specific_banked_sp(
				  micromachine::system::sp_reg::StackType::Process));
	ExpectThat()
		.Register(registers::PC).WasIncrementedBy(4)
		.Register(registers::SP).DidNotChange();
}


MICROMACHINE_TEST_F(msr, PRIMASKto1, CpuTestFixture) {
	code_gen().emit_ins32("111100111000nnnn", "10001000ssssssss", registers::R12, SYS_PRIMASK);
	getCpu().regs().set(registers::R12, 0xFFFFFFFF);

	Step();
	ExpectThat()
		.Register(registers::PC).WasIncrementedBy(4)
		.PrimaskStatusIs(true);
}

MICROMACHINE_TEST_F(msr, PRIMASKto0, CpuTestFixture) {
	code_gen().emit_ins32("111100111000nnnn", "10001000ssssssss", registers::R12, SYS_PRIMASK);
	getCpu().regs().set(registers::R12, 0xFFFFFFFE);

	Step();
	ExpectThat()
		.Register(registers::PC).WasIncrementedBy(4)
		.PrimaskStatusIs(false);
}


MICROMACHINE_TEST_F(msr, ToCONTROL, CpuTestFixture) {
	code_gen().emit_ins32("111100111000nnnn", "10001000ssssssss", registers::R12, SYS_CONTROL);
	getCpu().regs().set(registers::R12, 0xFFFFFFFF);
	getCpu().special_regs().control_register().set_n_priv(false);
	getCpu().special_regs().control_register().set_sp_sel(false);

	Step();
	EXPECT_TRUE(getCpu().special_regs().control_register().n_priv());
	EXPECT_TRUE(getCpu().special_regs().control_register().sp_sel());
	ExpectThat()
		.Register(registers::PC).WasIncrementedBy(4);

}

/*
TEST_SIM_ONLY(msr, registers::R13IsUnpredictable)
{
    code_gen().emit_ins32("111100111000nnnn", "10001000ssssssss", registers::SP, SYS_XPSR);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    ExpectThat().Register(registers::PC).Equals(INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(msr, registers::R15IsUnpredictable)
{
    code_gen().emit_ins32("111100111000nnnn", "10001000ssssssss", registers::PC, SYS_XPSR);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    ExpectThat().Register(registers::PC).Equals(INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(msr, UnpredictableSYSm)
{
    uint32_t predictables[] = {0, 1, 2, 3, 5, 6, 7, 8, 9, 16, 20};
    size_t   nextSkip = 0;

    for (uint32_t i = 0 ; i < 256 ; i++)
    {
        if (nextSkip < sizeof(predictables)/sizeof(predictables[0]) && i == predictables[nextSkip])
        {
            nextSkip++;
        }
        else
        {
            initContext();
            code_gen().emit_ins32("111100111000nnnn", "10001000ssssssss", registers::R0, i);
            setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
            ExpectThat().Register(registers::PC).Equals(INITIAL_PC);
            pinkySimStep(&m_context);
        }
    }
}

TEST_SIM_ONLY(msr, UnpredictableBecauseOfBit2_8)
{
    code_gen().emit_ins32("111100111000nnnn", "10001001ssssssss", registers::R0, SYS_XPSR);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    ExpectThat().Register(registers::PC).Equals(INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(msr, UnpredictableBecauseOfBit2_9)
{
    code_gen().emit_ins32("111100111000nnnn", "10001010ssssssss", registers::R0, SYS_XPSR);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    ExpectThat().Register(registers::PC).Equals(INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(msr, UnpredictableBecauseOfBit2_10)
{
    code_gen().emit_ins32("111100111000nnnn", "10001100ssssssss", registers::R0, SYS_XPSR);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    ExpectThat().Register(registers::PC).Equals(INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(msr, UnpredictableBecauseOfBit2_11)
{
    code_gen().emit_ins32("111100111000nnnn", "10000000ssssssss", registers::R0, SYS_XPSR);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    ExpectThat().Register(registers::PC).Equals(INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(msr, UnpredictableBecauseOfBit2_13)
{
    code_gen().emit_ins32("111100111000nnnn", "10101000ssssssss", registers::R0, SYS_XPSR);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    ExpectThat().Register(registers::PC).Equals(INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(msr, UnpredictableBecauseOfBit1_4)
{
    code_gen().emit_ins32("111100111001nnnn", "10001000ssssssss", registers::R0, SYS_XPSR);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    ExpectThat().Register(registers::PC).Equals(INITIAL_PC);
    step(&m_context);
}
*/
