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

/* CMP - Register - Encoding T1
   Encoding: 010000 1010 Rm:3 Rn:3 */
MICROMACHINE_TEST_F(cmpRegister, T1UseLowestRegisterForAllArgs, CpuTestFixture) {
	emitInstruction16("0100001010mmmnnn", registers::R0, registers::R0);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZCv"));
}

MICROMACHINE_TEST_F(cmpRegister, T1UseHigestRegisterForAllArgs, CpuTestFixture) {

	emitInstruction16("0100001010mmmnnn", registers::R7, registers::R7);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZCv"));
}

MICROMACHINE_TEST_F(cmpRegister, T1RnLargerThanRm, CpuTestFixture) {

	emitInstruction16("0100001010mmmnnn", registers::R1, registers::R2);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzCv"));
}

MICROMACHINE_TEST_F(cmpRegister, T1RnSmallerThanRm, CpuTestFixture) {

	emitInstruction16("0100001010mmmnnn", registers::R1, registers::R0);
	setReg(registers::R1, 1);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("Nzcv"));
}

MICROMACHINE_TEST_F(cmpRegister, T1ForceNegativeOverflow, CpuTestFixture) {
	emitInstruction16("0100001010mmmnnn", registers::R1, registers::R2);
	setReg(registers::R2, 0x80000000U);
	setReg(registers::R1, 1U);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzCV"));
}

MICROMACHINE_TEST_F(cmpRegister, T1ForcePositiveOverflow, CpuTestFixture) {

	emitInstruction16("0100001010mmmnnn", registers::R1, registers::R2);
	setReg(registers::R2, 0x7FFFFFFFU);
	setReg(registers::R1, -1U);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("NzcV"));
}

/* CMP - Register - Encoding T2
   Encoding: 010001 01 N:1 Rm:4 Rn:3
   NOTE: At least one register must be high register, registers::R8 - registers::R14. */
MICROMACHINE_TEST_F(cmpRegister, T2CompareLowestRegisterToHighestRegister, CpuTestFixture) {
	emitInstruction16("01000101nmmmmnnn", registers::R0, registers::LR);
	setReg(registers::LR, 0xEEEEEEEE);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzcv"));
}

MICROMACHINE_TEST_F(cmpRegister, T2CompareHighestRegisterToLowestRegister, CpuTestFixture) {
	emitInstruction16("01000101nmmmmnnn", registers::LR, registers::R0);
	setReg(registers::LR, 0xEEEEEEEE);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("NzCv"));
}

MICROMACHINE_TEST_F(cmpRegister, T2CompareR8ToItself, CpuTestFixture) {

	emitInstruction16("01000101nmmmmnnn", registers::R8, registers::R8);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZCv"));
}

MICROMACHINE_TEST_F(cmpRegister, T2ForceNegativeOverflow, CpuTestFixture) {
	emitInstruction16("01000101nmmmmnnn", registers::R11, registers::R12);
	setReg(registers::R11, 0x80000000U);
	setReg(registers::R12, 1U);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzCV"));
}

MICROMACHINE_TEST_F(cmpRegister, T2ForcePositiveOverflow, CpuTestFixture) {
	emitInstruction16("01000101nmmmmnnn", registers::R11, registers::R12);
	setReg(registers::R11, 0x7FFFFFFFU);
	setReg(registers::R12, -1U);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("NzcV"));
}
/*
TEST_SIM_ONLY(cmpRegister, T2UnpredictableForBothArgsToBeLowRegisters)
{
	code_gen().emit_ins16("01000101nmmmmnnn", registers::R6, registers::R7);
	setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
	reg(registers::PC).equals(TestMachine::TestMachine::INITIAL_PC),
	pinkySimStep(&m_context);
}

TEST_SIM_ONLY(cmpRegister, T2UnpredictableForRnToBeR15)
{
	code_gen().emit_ins16("01000101nmmmmnnn", registers::PC, registers::R8);
	setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
	reg(registers::PC).equals(TestMachine::TestMachine::INITIAL_PC),
	pinkySimStep(&m_context);
}

TEST_SIM_ONLY(cmpRegister, T2UnpredictableForRmToBeR15)
{
	code_gen().emit_ins16("01000101nmmmmnnn", registers::R8, registers::PC);
	setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
	reg(registers::PC).equals(TestMachine::TestMachine::INITIAL_PC),
	step(&m_context);
}
*/
