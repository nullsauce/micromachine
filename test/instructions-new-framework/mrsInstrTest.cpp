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


#define IPSR_VAL 0x20

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

namespace {

	void set_apsr_flags(apsr_reg& reg, bool neg, bool zero, bool carry, bool overflow) {
		reg.write_neg_flag(neg);
		reg.write_zero_flag(zero);
		reg.write_carry_flag(carry);
		reg.write_overflow_flag(overflow);
	}
};


/* MRS
   Encoding: 11110 0 1111 1 (0) (1)(1)(1)(1)
             10 (0) 0 Rd:4 SYSm:8 */
MICROMACHINE_TEST_F(mrs, FromAPSR, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins32("1111001111101111", "1000ddddssssssss", registers::R12, SYS_APSR);

	uint32_t expectedXpsr = 0;
	apsr_reg expectedApsr(expectedXpsr);
	set_apsr_flags(expectedApsr, true, false, true, false);

	set_apsr_flags(getCpu().regs().app_status_register(), true, false, true, false);
	getCpu().regs().interrupt_status_register().set_exception_number(exception::EXTI_08);
	getCpu().regs().set(registers::R12, 0xFFFFFFFF);

	Step();

	ExpectThat()
		.Register(registers::R12).Equals(expectedXpsr)
		.Register(registers::PC).WasIncrementedBy(4)
		.XPSRFlagsEquals("NzCv")
		.IPSRExceptionNumberIs(exception::EXTI_08)
		.ThumbBitIsSet();
}

MICROMACHINE_TEST_F(mrs, FromIAPSR, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins32("1111001111101111", "1000ddddssssssss", registers::R0, SYS_IAPSR);

	uint32_t expectedXpsr = 0;
	apsr_reg expectedApsr(expectedXpsr);
	ipsr_reg expectedIpsr(expectedXpsr);
	set_apsr_flags(expectedApsr, true, false, true, false);
	expectedIpsr.set_exception_number(exception::EXTI_08);

	set_apsr_flags(getCpu().regs().app_status_register(), true, false, true, false);
	getCpu().regs().interrupt_status_register().set_exception_number(exception::EXTI_08);
	getCpu().regs().set(registers::R0, 0xFFFFFFFF);

	Step();

	// Check that the IPSR bits loaded into the register are equal to what was set earlier
	ExpectThat()
		.Register(registers::R0).Equals(expectedXpsr)
		.Register(registers::PC).WasIncrementedBy(4)
		.XPSRFlagsEquals("NzCv")
		.IPSRExceptionNumberIs(exception::EXTI_08)
		.ThumbBitIsSet();
}

MICROMACHINE_TEST_F(mrs, FromEAPSR, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins32("1111001111101111", "1000ddddssssssss", registers::R12, SYS_EAPSR);

	uint32_t expectedXpsr = 0;
	apsr_reg expectedApsr(expectedXpsr);
	epsr_reg expectedEpsr(expectedXpsr);
	set_apsr_flags(expectedApsr, true, false, true, false);
	expectedEpsr.set_thumb_bit(false); // EPSR bits must read as zero


	set_apsr_flags(getCpu().regs().app_status_register(), true, false, true, false);
	getCpu().regs().interrupt_status_register().set_exception_number(exception::EXTI_08);
	getCpu().regs().set(registers::R12, 0xFFFFFFFF);

	Step();

	ExpectThat()
		.Register(registers::R12).Equals(expectedXpsr)
		.Register(registers::PC).WasIncrementedBy(4)
		.XPSRFlagsEquals("NzCv")
		.IPSRExceptionNumberIs(exception::EXTI_08)
		.ThumbBitIsSet();
}

MICROMACHINE_TEST_F(mrs, FromXPSR, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins32("1111001111101111", "1000ddddssssssss", registers::R12, SYS_XPSR);

	uint32_t expectedXpsr = 0;
	apsr_reg expectedApsr(expectedXpsr);
	ipsr_reg expectedIpsr(expectedXpsr);
	epsr_reg expectedEpsr(expectedXpsr);
	set_apsr_flags(expectedApsr, true, false, true, false);
	expectedIpsr.set_exception_number(exception::EXTI_08);
	expectedEpsr.set_thumb_bit(false); // EPSR bits must read as zero

	getCpu().regs().interrupt_status_register().set_exception_number(exception::EXTI_08);
	set_apsr_flags(getCpu().regs().app_status_register(), true, false, true, false);
	getCpu().regs().set(registers::R12, 0xFFFFFFFF);

	Step();
	ExpectThat()
		.Register(registers::PC).WasIncrementedBy(4)
		.Register(registers::R12).Equals(expectedXpsr)
		.XPSRFlagsEquals("NzCv")
		.IPSRExceptionNumberIs(exception::EXTI_08)
		.ThumbBitIsSet();
}

MICROMACHINE_TEST_F(mrs, FromIPSR, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins32("1111001111101111", "1000ddddssssssss", registers::R12, SYS_IPSR);

	uint32_t expectedXpsr = 0;
	ipsr_reg expectedIpsr(expectedXpsr);
	expectedIpsr.set_exception_number(exception::EXTI_08);

	getCpu().regs().interrupt_status_register().set_exception_number(exception::EXTI_08);
	set_apsr_flags(getCpu().regs().app_status_register(), true, false, true, false);
	getCpu().regs().set(registers::R12, 0xFFFFFFFF);

	Step();

	ExpectThat()
		.Register(registers::PC).WasIncrementedBy(4)
		.Register(registers::R12).Equals(expectedXpsr)
		.IPSRExceptionNumberIs(exception::EXTI_08)
		.XPSRFlagsEquals("NzCv")
		.ThumbBitIsSet();
}


MICROMACHINE_TEST_F(mrs, FromEPSR, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins32("1111001111101111", "1000ddddssssssss", registers::R12, SYS_EPSR);
	getCpu().regs().set(registers::R12, 0xFFFFFFFF);

	uint32_t expectedXpsr = 0;
	epsr_reg expectedEpsr(expectedXpsr);
	expectedEpsr.set_thumb_bit(false); // EPSR bits must read as zero

	getCpu().regs().interrupt_status_register().set_exception_number(exception::EXTI_08);
	set_apsr_flags(getCpu().regs().app_status_register(), true, false, true, false);
	getCpu().regs().set(registers::R12, 0xFFFFFFFF);

	Step();
	ExpectThat()
		.Register(registers::PC).WasIncrementedBy(4)
		.Register(registers::R12).Equals(expectedXpsr)
		.XPSRFlagsEquals("NzCv")
		.IPSRExceptionNumberIs(exception::EXTI_08)
		.ThumbBitIsSet();
}


MICROMACHINE_TEST_F(mrs, FromIEPSR, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins32("1111001111101111", "1000ddddssssssss", registers::R12, SYS_IEPSR);

	uint32_t expectedXpsr = 0;
	ipsr_reg expectedIpsr(expectedXpsr);
	epsr_reg expectedEpsr(expectedXpsr);
	expectedIpsr.set_exception_number(exception::EXTI_08);
	expectedEpsr.set_thumb_bit(false); // EPSR bits must read as zero

	getCpu().regs().interrupt_status_register().set_exception_number(exception::EXTI_08);
	set_apsr_flags(getCpu().regs().app_status_register(), true, false, true, false);
	getCpu().regs().set(registers::R12, 0xFFFFFFFF);

	Step();
	ExpectThat()
		.Register(registers::PC).WasIncrementedBy(4)
		.Register(registers::R12).Equals(expectedXpsr)
		.XPSRFlagsEquals("NzCv")
		.IPSRExceptionNumberIs(exception::EXTI_08)
		.ThumbBitIsSet();
}

MICROMACHINE_TEST_F(mrs, FromMSP, CpuTestFixture) {
	constexpr uint32_t INITIAL_SP = 0x00002000;
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins32("1111001111101111", "1000ddddssssssss", registers::R12, SYS_MSP);
	getCpu().regs().set(registers::R12, 0xFFFFFFFF);

	Step();
	ExpectThat()
		.Register(registers::R12).Equals(INITIAL_SP)
		.Register(registers::PC).WasIncrementedBy(4);
}

MICROMACHINE_TEST_F(mrs, FromPSP, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins32("1111001111101111", "1000ddddssssssss", registers::R12, SYS_PSP);
	getCpu().regs().set(registers::R12, 0xFFFFFFFF);

	Step();
	ExpectThat()
		.Register(registers::R12).Equals(0x0)
		.Register(registers::PC).WasIncrementedBy(4);
}

MICROMACHINE_TEST_F(mrs, FromPRIMASKsetTo1, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins32("1111001111101111", "1000ddddssssssss", registers::R12, SYS_PRIMASK);
	getCpu().regs().set(registers::R12, 0);
	getCpu().regs().primask_register().set_pm(true);

	Step();
	ExpectThat()
		.Register(registers::R12).Equals(1)
		.Register(registers::PC).WasIncrementedBy(4);
}


MICROMACHINE_TEST_F(mrs, PRIMASKto0, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins32("1111001111101111", "1000ddddssssssss", registers::R12, SYS_PRIMASK);
	getCpu().regs().set(registers::R12, 0xFFFFFFFF);

	getCpu().regs().primask_register().set_pm(false);

	Step();
	ExpectThat()
		.Register(registers::R12).Equals(0)
		.Register(registers::PC).WasIncrementedBy(4);
}


MICROMACHINE_TEST_F(mrs, FromCONTROL, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins32("1111001111101111", "1000ddddssssssss", registers::R12, SYS_CONTROL);
	getCpu().regs().set(registers::R12, 0);
	getCpu().regs().control_register().set_n_priv(true);
	getCpu().regs().control_register().set_sp_sel(true);

	Step();
	ExpectThat()
		.Register(registers::R12).Equals(0b11)
		.Register(registers::PC).WasIncrementedBy(4);
}

/*
TEST_F(CpuTestFixture, mrs_registers::R13IsUnpredictable)
{
    code_gen().emit_ins32("1111001111101111", "1000ddddssssssss", registers::SP, SYS_XPSR);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    ExpectThat().Register(registers::PC).Equals(INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_F(CpuTestFixture, mrs_registers::R15IsUnpredictable)
{
    code_gen().emit_ins32("1111001111101111", "1000ddddssssssss", registers::PC, SYS_XPSR);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    ExpectThat().Register(registers::PC).Equals(INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(mrs, UnpredictableSYSm)
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
            code_gen().emit_ins32("1111001111101111", "1000ddddssssssss", registers::R0, i);
            getCpu().regs().set(registers::R0, 0xFFFFFFFF);
            setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
            ExpectThat().Register(registers::PC).Equals(INITIAL_PC);
            pinkySimStep(&m_context);
        }
    }
}

TEST_SIM_ONLY(mrs, UnpredictableBecauseOfBit2_13)
{
    code_gen().emit_ins32("1111001111101111", "1010ddddssssssss", registers::R12, SYS_XPSR);
    getCpu().regs().set(registers::R0, 0xFFFFFFFF);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    ExpectThat().Register(registers::PC).Equals(INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(mrs, UnpredictableBecauseOfBit1_0)
{
    code_gen().emit_ins32("1111001111101110", "1000ddddssssssss", registers::R12, SYS_XPSR);
    getCpu().regs().set(registers::R0, 0xFFFFFFFF);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    ExpectThat().Register(registers::PC).Equals(INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(mrs, UnpredictableBecauseOfBit1_1)
{
    code_gen().emit_ins32("1111001111101101", "1000ddddssssssss", registers::R12, SYS_XPSR);
    getCpu().regs().set(registers::R0, 0xFFFFFFFF);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    ExpectThat().Register(registers::PC).Equals(INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(mrs, UnpredictableBecauseOfBit1_2)
{
    code_gen().emit_ins32("1111001111101011", "1000ddddssssssss", registers::R12, SYS_XPSR);
    getCpu().regs().set(registers::R0, 0xFFFFFFFF);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    ExpectThat().Register(registers::PC).Equals(INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(mrs, UnpredictableBecauseOfBit1_3)
{
    code_gen().emit_ins32("1111001111100111", "1000ddddssssssss", registers::R12, SYS_XPSR);
    getCpu().regs().set(registers::R0, 0xFFFFFFFF);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    ExpectThat().Register(registers::PC).Equals(INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(mrs, UnpredictableBecauseOfBit1_4)
{
    code_gen().emit_ins32("1111001111111111", "1000ddddssssssss", registers::R12, SYS_XPSR);
    getCpu().regs().set(registers::R0, 0xFFFFFFFF);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    ExpectThat().Register(registers::PC).Equals(INITIAL_PC);
    step(&m_context);
}
*/
