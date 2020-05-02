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

/* ADD - Register - Encoding T1
   Encoding: 000 11 0 0 Rm:3 Rn:3 Rd:3 */
MICROMACHINE_TEST_F(addRegister, T1UseLowestRegisterForAllArgs, CpuTestFixture) {
	emitInstruction16("0001100mmmnnnddd", registers::R0, registers::R0, registers::R0);
	StepAndExpectThatInstruction16IsExecutedAndThat(
		apsrFlagsEquals("nZcv"),
		reg(registers::R0).equals(0U));
}

MICROMACHINE_TEST_F(addRegister, T1UseHigestRegisterForAllArgs, CpuTestFixture) {
	emitInstruction16("0001100mmmnnnddd", registers::R7, registers::R7, registers::R7);

	StepAndExpectThatInstruction16IsExecutedAndThat(
		apsrFlagsEquals("NzcV"),
		reg(registers::R7).equals(INITIAL_R7 + INITIAL_R7));
}

MICROMACHINE_TEST_F(addRegister, T1UseDifferentRegistersForEachArg, CpuTestFixture) {
	emitInstruction16("0001100mmmnnnddd", registers::R1, registers::R2, registers::R3);



	StepAndExpectThatInstruction16IsExecutedAndThat(
		apsrFlagsEquals("nzcv"),
		reg(registers::R3).equals(INITIAL_R1 + INITIAL_R2));
}

// Force APSR flags to be set which haven't already been covered above.
MICROMACHINE_TEST_F(addRegister, T1ForceCarryWithNoOverflow, CpuTestFixture) {
	emitInstruction16("0001100mmmnnnddd", registers::R1, registers::R2, registers::R0);

	setReg(registers::R1, -1);
	setReg(registers::R2, 1);

	StepAndExpectThatInstruction16IsExecutedAndThat(
		apsrFlagsEquals("nZCv"),
		reg(registers::R0).equals(-1 + 1));
}

MICROMACHINE_TEST_F(addRegister, T1ForceCarryAndOverflow, CpuTestFixture) {
	emitInstruction16("0001100mmmnnnddd", registers::R1, registers::R2, registers::R0);

	setReg(registers::R1, -1);
	setReg(registers::R2, 0x80000000U);

	StepAndExpectThatInstruction16IsExecutedAndThat(
		apsrFlagsEquals("nzCV"),
		reg(registers::R0).equals(0x7FFFFFFFU));
}

/* ADD - Register - Encoding T2
   Encoding: 010001 00 DN:1 Rm:4 Rdn:3
   NOTE: Shouldn't modify any of the APSR flags.*/
MICROMACHINE_TEST_F(addRegister, T2UseR1ForAllArgs, CpuTestFixture) {
	emitInstruction16("01000100dmmmmddd", registers::R1, registers::R1);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R1).equals(INITIAL_R1 + INITIAL_R1));
}

MICROMACHINE_TEST_F(addRegister, T2UseLowestRegisterForAllArgs, CpuTestFixture) {
	emitInstruction16("01000100dmmmmddd", registers::R0, registers::R0);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R0).equals(0));
}

MICROMACHINE_TEST_F(addRegister, T2UseForR12AllArgs, CpuTestFixture) {
	emitInstruction16("01000100dmmmmddd", registers::R12, registers::R12);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R12).equals(INITIAL_R12 + INITIAL_R12));
}

MICROMACHINE_TEST_F(addRegister, T2UseDifferentRegistersForEachArg, CpuTestFixture) {
	emitInstruction16("01000100dmmmmddd", registers::R2, registers::R1);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R2).equals(INITIAL_R1 + INITIAL_R2));
}

MICROMACHINE_TEST_F(addRegister, T2WrapAroundTo0, CpuTestFixture) {
	emitInstruction16("01000100dmmmmddd", registers::R2, registers::R1);
	setReg(registers::R1, -1);
	setReg(registers::R2, 1);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R2).equals(-1 + 1));
}

MICROMACHINE_TEST_F(addRegister, T2OverflowFromLowestNegativeValue, CpuTestFixture) {
	emitInstruction16("01000100dmmmmddd", registers::R11, registers::R10);
	setReg(registers::R10, -1);
	setReg(registers::R11, 0x80000000U);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R11).equals(0x7FFFFFFF));
}

MICROMACHINE_TEST_F(addRegister, T2Add4ToSP, CpuTestFixture) {

	emitInstruction16("01000100dmmmmddd", registers::SP, registers::R1);
	setReg(registers::SP, TestMachine::INITIAL_SP - 4);
	setReg(registers::R1, 4);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::SP).equals(TestMachine::INITIAL_SP - 4 + 4));
}

MICROMACHINE_TEST_F(addRegister, T2Subtract4FromSP, CpuTestFixture) {
	emitInstruction16("01000100dmmmmddd", registers::SP, registers::R1);
	setReg(registers::R1, -4);

	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::SP).equals(TestMachine::TestMachine::INITIAL_SP - 4));
}

MICROMACHINE_TEST_F(addRegister, T2Add1ToLR, CpuTestFixture) {
	emitInstruction16("01000100dmmmmddd", registers::LR, registers::R1);

	setReg(registers::R1, 1);

	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::LR).equals(INITIAL_LR + 1));
}

MICROMACHINE_TEST_F(addRegister, T2Add1ToPCWhichWillBeOddAndRoundedDown, CpuTestFixture) {
	emitInstruction16("01000100dmmmmddd", registers::PC, registers::R1);
	setReg(registers::R1, 1);
	StepAndExpectThat(
		reg(registers::PC).equals((TestMachine::INITIAL_PC + 4 + 1) & 0xFFFFFFFE)
	);
}

MICROMACHINE_TEST_F(addRegister, T2Add2ToPC, CpuTestFixture) {
	emitInstruction16("01000100dmmmmddd", registers::PC, registers::R1);
	setReg(registers::PC, TestMachine::INITIAL_PC);
	setReg(registers::R1, 2);
	StepAndExpectThat(reg(registers::PC).equals((TestMachine::INITIAL_PC + 4 + 2) & 0xFFFFFFFE));
}

/*
TEST_SIM_ONLY(addRegister, T2ItIsUnpredictableToHaveBothArgsBePC)
{
	code_gen().emit_add_t2(registers::PC, registers::PC);
	setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
	reg(registers::PC).equals(TestMachine::INITIAL_PC),
	step(&m_context);
}
*/
