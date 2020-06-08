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
#include "helpers/binops.hpp"

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

/* MSR
   Encoding: 11110 0 1110 0 (0) Rn:4
			 10 (0) 0 (1) (0) (0) (0) SYSm:8 */
MICROMACHINE_TEST_F(msr, ToAPSR, CpuTestFixture) {

	emitInstruction32("111100111000nnnn", "10001000ssssssss", registers::R12, SYS_APSR);
	setNegativeFlag(false);
	setZeroFlag(false);
	setCarryFlag(false);
	setOverflowFlag(false);
	setReg(registers::R12, 0xFFFFFFFF);

	StepAndExpectThatInstruction32IsExecutedAndThat(apsrFlagsEquals("NZCV"));
}

MICROMACHINE_TEST_F(msr, ToIAPSR, CpuTestFixture) {

	emitInstruction32("111100111000nnnn", "10001000ssssssss", registers::R12, SYS_IAPSR);
	setZeroFlag(false);
	setCarryFlag(false);
	setOverflowFlag(false);
	setReg(registers::R12, 0xFFFFFFFF);
	setNegativeFlag(false);

	StepAndExpectThatInstruction32IsExecutedAndThat(apsrFlagsEquals("NZCV"));
}

MICROMACHINE_TEST_F(msr, ToEAPSR, CpuTestFixture) {
	emitInstruction32("111100111000nnnn", "10001000ssssssss", registers::R12, SYS_EAPSR);

	setNegativeFlag(false);
	setZeroFlag(false);
	setCarryFlag(false);
	setOverflowFlag(false);
	setReg(registers::R12, 0xFFFFFFFF);

	StepAndExpectThatInstruction32IsExecutedAndThat(apsrFlagsEquals("NZCV"));
}

MICROMACHINE_TEST_F(msr, ToXPSR, CpuTestFixture) {
	emitInstruction32("111100111000nnnn", "10001000ssssssss", registers::R0, SYS_XPSR);
	setNegativeFlag(false);
	setZeroFlag(false);
	setCarryFlag(false);
	setOverflowFlag(false);
	setReg(registers::R0, 0xFFFFFFFF);

	StepAndExpectThatInstruction32IsExecutedAndThat(apsrFlagsEquals("NZCV"));
}

MICROMACHINE_TEST_F(msr, ToIPSRIsIgnored, CpuTestFixture) {
	emitInstruction32("111100111000nnnn", "10001000ssssssss", registers::R12, SYS_IPSR);
	setReg(registers::R12, 0xFFFFFFFF);

	StepAndExpectThatInstruction32IsExecutedAndThat();
}

MICROMACHINE_TEST_F(msr, ToEPSRIsIgnored, CpuTestFixture) {
	emitInstruction32("111100111000nnnn", "10001000ssssssss", registers::R12, SYS_EPSR);
	setReg(registers::R12, 0xFFFFFFFF);

	StepAndExpectThatInstruction32IsExecutedAndThat();
}

MICROMACHINE_TEST_F(msr, ToIEPSRIsIgnored, CpuTestFixture) {
	emitInstruction32("111100111000nnnn", "10001000ssssssss", registers::R12, SYS_IEPSR);
	setReg(registers::R12, 0xFFFFFFFF);

	StepAndExpectThatInstruction32IsExecutedAndThat();
}

MICROMACHINE_TEST_F(msr, ToMSP, CpuTestFixture) {

	emitInstruction32("111100111000nnnn", "10001000ssssssss", registers::R12, SYS_MSP);
	const uint32_t spValue = TestMachine::INITIAL_PC + 1024 + 2;
	setReg(registers::R12, spValue);
	// expect an sp value is aligned to lowest 4 bytes address boundary
	const uint32_t expectedSPMainValue = binops::aligned<4>(spValue);
	const uint32_t expectedSPProcessValue = 0U;

	StepAndExpectThatInstruction32IsExecutedAndThat(
		BankedStackPointerStatePredicate(expectedSPMainValue, expectedSPProcessValue)
	);
}

MICROMACHINE_TEST_F(msr, ToPSP, CpuTestFixture) {

	emitInstruction32("111100111000nnnn", "10001000ssssssss", registers::R12, SYS_PSP);
	const uint32_t spValue = TestMachine::INITIAL_PC + 1024 + 2;
	setReg(registers::R12, spValue);

	// expect an sp value is aligned to lowest 4 bytes address boundary
	const uint32_t expectedSPProcessValue = binops::aligned<4>(spValue);
	const uint32_t expectedSPMainValue = getReg(registers::SP);

	StepAndExpectThatInstruction32IsExecutedAndThat(
		BankedStackPointerStatePredicate(expectedSPMainValue, expectedSPProcessValue));
}

MICROMACHINE_TEST_F(msr, PRIMASKto1, CpuTestFixture) {
	emitInstruction32("111100111000nnnn", "10001000ssssssss", registers::R12, SYS_PRIMASK);
	setReg(registers::R12, 0xFFFFFFFF);

	StepAndExpectThatInstruction32IsExecutedAndThat(PrimaskRegStatePredicate(true));
}

MICROMACHINE_TEST_F(msr, PRIMASKto0, CpuTestFixture) {
	emitInstruction32("111100111000nnnn", "10001000ssssssss", registers::R12, SYS_PRIMASK);
	setReg(registers::R12, 0xFFFFFFFE);

	StepAndExpectThatInstruction32IsExecutedAndThat(PrimaskRegStatePredicate(false));
}

MICROMACHINE_TEST_F(msr, ToCONTROL, CpuTestFixture) {
	emitInstruction32("111100111000nnnn", "10001000ssssssss", registers::R12, SYS_CONTROL);
	setReg(registers::R12, 0xFFFFFFFF);
	setControl(false, false);

	StepAndExpectThatInstruction32IsExecutedAndThat(
		ControlRegStatePredicate(true, true)
	);
}

/*
TEST_SIM_ONLY(msr, registers::R13IsUnpredictable)
{
	code_gen().emit_ins32("111100111000nnnn", "10001000ssssssss", registers::SP, SYS_XPSR);
	setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
	reg(registers::PC).equals(TestMachine::INITIAL_PC),
	pinkySimStep(&m_context);
}

TEST_SIM_ONLY(msr, registers::R15IsUnpredictable)
{
	code_gen().emit_ins32("111100111000nnnn", "10001000ssssssss", registers::PC, SYS_XPSR);
	setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
	reg(registers::PC).equals(TestMachine::INITIAL_PC),
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
			reg(registers::PC).equals(TestMachine::INITIAL_PC),
			pinkySimStep(&m_context);
		}
	}
}

TEST_SIM_ONLY(msr, UnpredictableBecauseOfBit2_8)
{
	code_gen().emit_ins32("111100111000nnnn", "10001001ssssssss", registers::R0, SYS_XPSR);
	setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
	reg(registers::PC).equals(TestMachine::INITIAL_PC),
	pinkySimStep(&m_context);
}

TEST_SIM_ONLY(msr, UnpredictableBecauseOfBit2_9)
{
	code_gen().emit_ins32("111100111000nnnn", "10001010ssssssss", registers::R0, SYS_XPSR);
	setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
	reg(registers::PC).equals(TestMachine::INITIAL_PC),
	pinkySimStep(&m_context);
}

TEST_SIM_ONLY(msr, UnpredictableBecauseOfBit2_10)
{
	code_gen().emit_ins32("111100111000nnnn", "10001100ssssssss", registers::R0, SYS_XPSR);
	setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
	reg(registers::PC).equals(TestMachine::INITIAL_PC),
	pinkySimStep(&m_context);
}

TEST_SIM_ONLY(msr, UnpredictableBecauseOfBit2_11)
{
	code_gen().emit_ins32("111100111000nnnn", "10000000ssssssss", registers::R0, SYS_XPSR);
	setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
	reg(registers::PC).equals(TestMachine::INITIAL_PC),
	pinkySimStep(&m_context);
}

TEST_SIM_ONLY(msr, UnpredictableBecauseOfBit2_13)
{
	code_gen().emit_ins32("111100111000nnnn", "10101000ssssssss", registers::R0, SYS_XPSR);
	setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
	reg(registers::PC).equals(TestMachine::INITIAL_PC),
	pinkySimStep(&m_context);
}

TEST_SIM_ONLY(msr, UnpredictableBecauseOfBit1_4)
{
	code_gen().emit_ins32("111100111001nnnn", "10001000ssssssss", registers::R0, SYS_XPSR);
	setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
	reg(registers::PC).equals(TestMachine::INITIAL_PC),
	step(&m_context);
}
*/
