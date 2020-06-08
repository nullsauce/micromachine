/*  Copyright (C) 2014  Adam Green (https://github.com/adamgreen): Original implementation
    Copyright (C) 2019  Flavio Roth (https://github.com/flavioroth): Adaptation

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
#include "exception/exception.hpp"

#define IPSR_VAL 0x20

/* SYSm field values for MSR and MRS instructions. */
#define SYS_APSR 0
#define SYS_IAPSR 1
#define SYS_EAPSR 2
#define SYS_XPSR 3
#define SYS_IPSR 5
#define SYS_EPSR 6
#define SYS_IEPSR 7
#define SYS_MSP 8
#define SYS_PSP 9
#define SYS_PRIMASK 16
#define SYS_CONTROL 20

/* MRS
   Encoding: 11110 0 1111 1 (0) (1)(1)(1)(1)
			 10 (0) 0 Rd:4 SYSm:8 */
MICROMACHINE_TEST_F(mrs, FromAPSR, CpuTestFixture) {

	emitInstruction32("1111001111101111", "1000ddddssssssss", registers::R12, SYS_APSR);
	setXPSR(makeXPSRFlagsFrom("NzCv", exception::EXTI_08, true));
	setReg(registers::R12, 0xFFFFFFFF);

	// A PSR -> pick only Flags
	uint32_t expectedXPSR = makeXPSRFlagsFrom("NzCv", exception::INVALID, false);
	StepAndExpectThatInstruction32IsExecutedAndThat(reg(registers::R12).equals(expectedXPSR),
													apsrFlagsEquals("NzCv"),
													IPSRStatePredicate(micromachine::system::exception::EXTI_08),
													thumbFlagIsSet());
}

MICROMACHINE_TEST_F(mrs, FromIAPSR, CpuTestFixture) {

	emitInstruction32("1111001111101111", "1000ddddssssssss", registers::R0, SYS_IAPSR);
	setXPSR(makeXPSRFlagsFrom("NzCv", exception::EXTI_08, true));
	setReg(registers::R0, 0xFFFFFFFF);

	// IA PSR -> pick only Interrupt | Flags
	uint32_t expectedXPSR = makeXPSRFlagsFrom("NzCv", exception::EXTI_08, false);

	StepAndExpectThatInstruction32IsExecutedAndThat(reg(registers::R0).equals(expectedXPSR),
													apsrFlagsEquals("NzCv"),
													IPSRStatePredicate(micromachine::system::exception::EXTI_08),
													thumbFlagIsSet());
}

MICROMACHINE_TEST_F(mrs, FromEAPSR, CpuTestFixture) {

	emitInstruction32("1111001111101111", "1000ddddssssssss", registers::R12, SYS_EAPSR);
	setXPSR(makeXPSRFlagsFrom("NzCv", exception::EXTI_08, true));
	setReg(registers::R12, 0xFFFFFFFF);

	// EA PSR -> pick only Thumb-flag | Flags
	// but ignore Thumb-flag as it must read as zero
	uint32_t expectedXPSR = makeXPSRFlagsFrom("NzCv", exception::INVALID, false);

	StepAndExpectThatInstruction32IsExecutedAndThat(reg(registers::R12).equals(expectedXPSR),
					  apsrFlagsEquals("NzCv"),
					  IPSRStatePredicate(micromachine::system::exception::EXTI_08),
					  thumbFlagIsSet());
}

MICROMACHINE_TEST_F(mrs, FromXPSR, CpuTestFixture) {

	emitInstruction32("1111001111101111", "1000ddddssssssss", registers::R12, SYS_XPSR);
	setXPSR(makeXPSRFlagsFrom("NzCv", exception::EXTI_08, true));
	setReg(registers::R12, 0xFFFFFFFF);

	// XPSR -> everything
	// but ignore Thumb-flag as it must read as zero
	uint32_t expectedXPSR = makeXPSRFlagsFrom("NzCv", exception::EXTI_08, false);

	StepAndExpectThatInstruction32IsExecutedAndThat(reg(registers::R12).equals(expectedXPSR),
					  apsrFlagsEquals("NzCv"),
					  IPSRStatePredicate(micromachine::system::exception::EXTI_08),
					  thumbFlagIsSet());
}

MICROMACHINE_TEST_F(mrs, FromIPSR, CpuTestFixture) {

	emitInstruction32("1111001111101111", "1000ddddssssssss", registers::R12, SYS_IPSR);
	setXPSR(makeXPSRFlagsFrom("NzCv", exception::EXTI_08, true));
	setReg(registers::R12, 0xFFFFFFFF);

	// IPSR -> pick only Interrupt
	uint32_t expectedXPSR = makeXPSRFlagsFrom("nzcv", exception::EXTI_08, false);

	StepAndExpectThatInstruction32IsExecutedAndThat(reg(registers::R12).equals(expectedXPSR),
					  apsrFlagsEquals("NzCv"),
					  IPSRStatePredicate(micromachine::system::exception::EXTI_08),
					  thumbFlagIsSet());
}

MICROMACHINE_TEST_F(mrs, FromEPSR, CpuTestFixture) {

	emitInstruction32("1111001111101111", "1000ddddssssssss", registers::R12, SYS_EPSR);
	setXPSR(makeXPSRFlagsFrom("NzCv", exception::EXTI_08, true));
	setReg(registers::R12, 0xFFFFFFFF);

	// EPSR -> pick only Thumb-flag
	// but ignore Thumb-flag as it must read as zero
	uint32_t expectedXPSR = makeXPSRFlagsFrom("nzcv", exception::INVALID, false);

	StepAndExpectThatInstruction32IsExecutedAndThat(reg(registers::R12).equals(expectedXPSR),
													apsrFlagsEquals("NzCv"),
													IPSRStatePredicate(micromachine::system::exception::EXTI_08),
													thumbFlagIsSet());
}

MICROMACHINE_TEST_F(mrs, FromIEPSR, CpuTestFixture) {

	emitInstruction32("1111001111101111", "1000ddddssssssss", registers::R12, SYS_IEPSR);
	setXPSR(makeXPSRFlagsFrom("NzCv", exception::EXTI_08, true));
	setReg(registers::R12, 0xFFFFFFFF);

	// IEPSR -> pick only Interrupt | Thumb-flag
	// but ignore Thumb-flag as it must read as zero
	uint32_t expectedXPSR = makeXPSRFlagsFrom("nzcv", exception::EXTI_08, false);

	StepAndExpectThatInstruction32IsExecutedAndThat(reg(registers::R12).equals(expectedXPSR),
													apsrFlagsEquals("NzCv"),
													IPSRStatePredicate(micromachine::system::exception::EXTI_08),
													thumbFlagIsSet());
}

MICROMACHINE_TEST_F(mrs, FromMSP, CpuTestFixture) {

	emitInstruction32("1111001111101111", "1000ddddssssssss", registers::R12, SYS_MSP);
	setReg(registers::R12, 0xFFFFFFFF);

	StepAndExpectThatInstruction32IsExecutedAndThat(reg(registers::R12).equals(TestMachine::INITIAL_SP));
}

MICROMACHINE_TEST_F(mrs, FromPSP, CpuTestFixture) {

	emitInstruction32("1111001111101111", "1000ddddssssssss", registers::R12, SYS_PSP);
	setReg(registers::R12, 0xFFFFFFFF);

	StepAndExpectThatInstruction32IsExecutedAndThat(reg(registers::R12).equals(0x0));
}

MICROMACHINE_TEST_F(mrs, FromPRIMASKsetTo1, CpuTestFixture) {

	emitInstruction32("1111001111101111", "1000ddddssssssss", registers::R12, SYS_PRIMASK);
	setReg(registers::R12, 0);
	setPrimaskFlag(true);

	StepAndExpectThatInstruction32IsExecutedAndThat(reg(registers::R12).equals(1));
}

MICROMACHINE_TEST_F(mrs, PRIMASKto0, CpuTestFixture) {

	emitInstruction32("1111001111101111", "1000ddddssssssss", registers::R12, SYS_PRIMASK);
	setReg(registers::R12, 0xFFFFFFFF);
	setPrimaskFlag(false);

	StepAndExpectThatInstruction32IsExecutedAndThat(reg(registers::R12).equals(0));
}

MICROMACHINE_TEST_F(mrs, FromCONTROL, CpuTestFixture) {

	emitInstruction32("1111001111101111", "1000ddddssssssss", registers::R12, SYS_CONTROL);
	setReg(registers::R12, 0);
	setControl(true, true);
	StepAndExpectThat(reg(registers::R12).equals(0b11), instruction32Executed());
}

/*
TEST_F(CpuTestFixture, mrs_registers::R13IsUnpredictable)
{
	code_gen().emit_ins32("1111001111101111", "1000ddddssssssss", registers::SP, SYS_XPSR);
	setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
	reg(registers::PC).equals(TestMachine::INITIAL_PC),
	pinkySimStep(&m_context);
}

TEST_F(CpuTestFixture, mrs_registers::R15IsUnpredictable)
{
	code_gen().emit_ins32("1111001111101111", "1000ddddssssssss", registers::PC, SYS_XPSR);
	setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
	reg(registers::PC).equals(TestMachine::INITIAL_PC),
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
			setReg(registers::R0, 0xFFFFFFFF);
			setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
			reg(registers::PC).equals(TestMachine::INITIAL_PC),
			pinkySimStep(&m_context);
		}
	}
}

TEST_SIM_ONLY(mrs, UnpredictableBecauseOfBit2_13)
{
	code_gen().emit_ins32("1111001111101111", "1010ddddssssssss", registers::R12, SYS_XPSR);
	setReg(registers::R0, 0xFFFFFFFF);
	setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
	reg(registers::PC).equals(TestMachine::INITIAL_PC),
	pinkySimStep(&m_context);
}

TEST_SIM_ONLY(mrs, UnpredictableBecauseOfBit1_0)
{
	code_gen().emit_ins32("1111001111101110", "1000ddddssssssss", registers::R12, SYS_XPSR);
	setReg(registers::R0, 0xFFFFFFFF);
	setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
	reg(registers::PC).equals(TestMachine::INITIAL_PC),
	pinkySimStep(&m_context);
}

TEST_SIM_ONLY(mrs, UnpredictableBecauseOfBit1_1)
{
	code_gen().emit_ins32("1111001111101101", "1000ddddssssssss", registers::R12, SYS_XPSR);
	setReg(registers::R0, 0xFFFFFFFF);
	setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
	reg(registers::PC).equals(TestMachine::INITIAL_PC),
	pinkySimStep(&m_context);
}

TEST_SIM_ONLY(mrs, UnpredictableBecauseOfBit1_2)
{
	code_gen().emit_ins32("1111001111101011", "1000ddddssssssss", registers::R12, SYS_XPSR);
	setReg(registers::R0, 0xFFFFFFFF);
	setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
	reg(registers::PC).equals(TestMachine::INITIAL_PC),
	pinkySimStep(&m_context);
}

TEST_SIM_ONLY(mrs, UnpredictableBecauseOfBit1_3)
{
	code_gen().emit_ins32("1111001111100111", "1000ddddssssssss", registers::R12, SYS_XPSR);
	setReg(registers::R0, 0xFFFFFFFF);
	setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
	reg(registers::PC).equals(TestMachine::INITIAL_PC),
	pinkySimStep(&m_context);
}

TEST_SIM_ONLY(mrs, UnpredictableBecauseOfBit1_4)
{
	code_gen().emit_ins32("1111001111111111", "1000ddddssssssss", registers::R12, SYS_XPSR);
	setReg(registers::R0, 0xFFFFFFFF);
	setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
	reg(registers::PC).equals(TestMachine::INITIAL_PC),
	step(&m_context);
}
*/
