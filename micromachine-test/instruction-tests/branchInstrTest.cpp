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

/* Condition codes */
#define COND_EQ 0x0
#define COND_NE (COND_EQ | 1)
#define COND_CS 0x2
#define COND_CC (COND_CS | 1)
#define COND_MI 0x4
#define COND_PL (COND_MI | 1)
#define COND_VS 0x6
#define COND_VC (COND_VS | 1)
#define COND_HI 0x8
#define COND_LS (COND_HI | 1)
#define COND_GE 0xA
#define COND_LT (COND_GE | 1)
#define COND_GT 0xC
#define COND_LE (COND_GT | 1)
#define COND_AL 0xE

/* B - Encoding T1 (Conditional)
   Encoding: 1101 Cond:4 Imm:8 */
MICROMACHINE_TEST_F(b, BEQ_NotTaken, CpuTestFixture) {
	emitInstruction16("1101cccciiiiiiii", COND_EQ, 0);
	// These tests set the APSR flags to specific value and expect them to be unmodified upon return.
	setZeroFlag(false);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzcv"));
}

MICROMACHINE_TEST_F(b, BEQ_Taken, CpuTestFixture) {
	emitInstruction16("1101cccciiiiiiii", COND_EQ, 0);
	setZeroFlag(true);
	StepAndExpectThat(apsrFlagsEquals("nZcv"), reg(registers::PC).wasIncrementedBy(4));
}

MICROMACHINE_TEST_F(b, BNE_NotTaken, CpuTestFixture) {
	emitInstruction16("1101cccciiiiiiii", COND_NE, 0);
	setZeroFlag(true);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZcv"));
}

MICROMACHINE_TEST_F(b, BNE_Taken, CpuTestFixture) {

	emitInstruction16("1101cccciiiiiiii", COND_NE, 0);
	setZeroFlag(false);
	StepAndExpectThat(apsrFlagsEquals("nzcv"), reg(registers::PC).wasIncrementedBy(4));
}

MICROMACHINE_TEST_F(b, BCS_NotTaken, CpuTestFixture) {
	emitInstruction16("1101cccciiiiiiii", COND_CS, 0);
	setCarryFlag(false);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzcv"));
}

MICROMACHINE_TEST_F(b, BCS_Taken, CpuTestFixture) {

	emitInstruction16("1101cccciiiiiiii", COND_CS, 0);
	setCarryFlag(true);
	StepAndExpectThat(apsrFlagsEquals("nzCv"), reg(registers::PC).wasIncrementedBy(4));
}

MICROMACHINE_TEST_F(b, BCC_NotTaken, CpuTestFixture) {

	emitInstruction16("1101cccciiiiiiii", COND_CC, 0);
	setCarryFlag(true);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzCv"));
}

MICROMACHINE_TEST_F(b, BCC_Taken, CpuTestFixture) {

	emitInstruction16("1101cccciiiiiiii", COND_CC, 0);
	setCarryFlag(false);
	StepAndExpectThat(apsrFlagsEquals("nzcv"), reg(registers::PC).wasIncrementedBy(4));
}

MICROMACHINE_TEST_F(b, BMI_NotTaken, CpuTestFixture) {
	emitInstruction16("1101cccciiiiiiii", COND_MI, 0);
	setNegativeFlag(false);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzcv"));
}

MICROMACHINE_TEST_F(b, BMI_Taken, CpuTestFixture) {
	emitInstruction16("1101cccciiiiiiii", COND_MI, 0);
	setNegativeFlag(true);
	StepAndExpectThat(apsrFlagsEquals("Nzcv"), reg(registers::PC).wasIncrementedBy(4));
}

MICROMACHINE_TEST_F(b, BPL_NotTaken, CpuTestFixture) {
	emitInstruction16("1101cccciiiiiiii", COND_PL, 0);
	setNegativeFlag(true);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("Nzcv"));
}

MICROMACHINE_TEST_F(b, BPL_Taken, CpuTestFixture) {
	emitInstruction16("1101cccciiiiiiii", COND_PL, 0);
	setNegativeFlag(false);
	StepAndExpectThat(apsrFlagsEquals("nzcv"), reg(registers::PC).wasIncrementedBy(4));
}

MICROMACHINE_TEST_F(b, BVS_NotTaken, CpuTestFixture) {
	emitInstruction16("1101cccciiiiiiii", COND_VS, 0);
	setOverflowFlag(false);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzcv"));
}

MICROMACHINE_TEST_F(b, BVS_Taken, CpuTestFixture) {

	emitInstruction16("1101cccciiiiiiii", COND_VS, 0);
	setOverflowFlag(true);
	StepAndExpectThat(apsrFlagsEquals("nzcV"), reg(registers::PC).wasIncrementedBy(4));
}

MICROMACHINE_TEST_F(b, BVC_NotTaken, CpuTestFixture) {
	emitInstruction16("1101cccciiiiiiii", COND_VC, 0);
	setOverflowFlag(true);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzcV"));
}

MICROMACHINE_TEST_F(b, BVC_Taken, CpuTestFixture) {

	emitInstruction16("1101cccciiiiiiii", COND_VC, 0);
	setOverflowFlag(false);
	StepAndExpectThat(apsrFlagsEquals("nzcv"), reg(registers::PC).wasIncrementedBy(4));
}

MICROMACHINE_TEST_F(b, BHI_NotTaken, CpuTestFixture) {
	emitInstruction16("1101cccciiiiiiii", COND_HI, 0);
	setCarryFlag(false);
	setZeroFlag(true);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nZcv"));
}

MICROMACHINE_TEST_F(b, BHI_Taken, CpuTestFixture) {

	emitInstruction16("1101cccciiiiiiii", COND_HI, 0);
	setCarryFlag(true);
	setZeroFlag(false);
	StepAndExpectThat(apsrFlagsEquals("nzCv"), reg(registers::PC).wasIncrementedBy(4));
}

MICROMACHINE_TEST_F(b, BLS_NotTaken, CpuTestFixture) {
	emitInstruction16("1101cccciiiiiiii", COND_LS, 0);
	setCarryFlag(true);
	setZeroFlag(false);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("nzCv"));
}

MICROMACHINE_TEST_F(b, BLS_Taken, CpuTestFixture) {

	emitInstruction16("1101cccciiiiiiii", COND_LS, 0);
	setCarryFlag(false);
	setZeroFlag(true);
	StepAndExpectThat(apsrFlagsEquals("nZcv"), reg(registers::PC).wasIncrementedBy(4));
}

MICROMACHINE_TEST_F(b, BGE_NotTaken, CpuTestFixture) {
	emitInstruction16("1101cccciiiiiiii", COND_GE, 0);
	setNegativeFlag(true);
	setOverflowFlag(false);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("Nzcv"));
}

MICROMACHINE_TEST_F(b, BGE_Taken1, CpuTestFixture) {

	emitInstruction16("1101cccciiiiiiii", COND_GE, 0);
	setNegativeFlag(true);
	setOverflowFlag(true);
	StepAndExpectThat(apsrFlagsEquals("NzcV"), reg(registers::PC).wasIncrementedBy(4));
}

MICROMACHINE_TEST_F(b, BGE_Taken2, CpuTestFixture) {

	emitInstruction16("1101cccciiiiiiii", COND_GE, 0);
	setNegativeFlag(false);
	setOverflowFlag(false);
	StepAndExpectThat(apsrFlagsEquals("nzcv"), reg(registers::PC).wasIncrementedBy(4));
}

MICROMACHINE_TEST_F(b, BLT_NotTaken, CpuTestFixture) {
	emitInstruction16("1101cccciiiiiiii", COND_LT, 0);
	setNegativeFlag(true);
	setOverflowFlag(true);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("NzcV"));
}

MICROMACHINE_TEST_F(b, BLT_Taken1, CpuTestFixture) {

	emitInstruction16("1101cccciiiiiiii", COND_LT, 0);
	setNegativeFlag(true);
	setOverflowFlag(false);
	StepAndExpectThat(apsrFlagsEquals("Nzcv"), reg(registers::PC).wasIncrementedBy(4));
}

MICROMACHINE_TEST_F(b, BLT_Taken2, CpuTestFixture) {

	emitInstruction16("1101cccciiiiiiii", COND_LT, 0);
	setNegativeFlag(false);
	setOverflowFlag(true);
	StepAndExpectThat(apsrFlagsEquals("nzcV"), reg(registers::PC).wasIncrementedBy(4));
}

MICROMACHINE_TEST_F(b, BGT_NotTaken, CpuTestFixture) {
	emitInstruction16("1101cccciiiiiiii", COND_GT, 0);
	setZeroFlag(true);
	setNegativeFlag(true);
	setOverflowFlag(true);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("NZcV"));
}

MICROMACHINE_TEST_F(b, BGT_Taken1, CpuTestFixture) {
	emitInstruction16("1101cccciiiiiiii", COND_GT, 0);
	setZeroFlag(false);
	setNegativeFlag(false);
	setOverflowFlag(false);
	StepAndExpectThat(reg(registers::PC).wasIncrementedBy(4));
}

MICROMACHINE_TEST_F(b, BGT_Taken2, CpuTestFixture) {

	emitInstruction16("1101cccciiiiiiii", COND_GT, 0);
	setZeroFlag(false);
	setNegativeFlag(true);
	setOverflowFlag(true);
	StepAndExpectThat(apsrFlagsEquals("NzcV"), reg(registers::PC).wasIncrementedBy(4));
}

MICROMACHINE_TEST_F(b, BLE_NotTaken, CpuTestFixture) {

	emitInstruction16("1101cccciiiiiiii", COND_LE, 0);
	setZeroFlag(false);
	setNegativeFlag(true);
	setOverflowFlag(true);
	StepAndExpectThatInstruction16IsExecutedAndThat(apsrFlagsEquals("NzcV"));
}

MICROMACHINE_TEST_F(b, BLE_Taken1, CpuTestFixture) {

	emitInstruction16("1101cccciiiiiiii", COND_LE, 0);
	setZeroFlag(true);
	setNegativeFlag(true);
	setOverflowFlag(false);
	StepAndExpectThat(apsrFlagsEquals("NZcv"), reg(registers::PC).wasIncrementedBy(4));
}

MICROMACHINE_TEST_F(b, BLE_Taken2, CpuTestFixture) {

	emitInstruction16("1101cccciiiiiiii", COND_LE, 0);
	setZeroFlag(true);
	setNegativeFlag(false);
	setOverflowFlag(true);
	StepAndExpectThat(apsrFlagsEquals("nZcV"), reg(registers::PC).wasIncrementedBy(4));
}

MICROMACHINE_TEST_F(b, BEQ_TakenWithLargestPositiveOffset, CpuTestFixture) {
	emitInstruction16("1101cccciiiiiiii", COND_EQ, 127);
	setZeroFlag(true);
	StepAndExpectThat(reg(registers::PC).equals(TestMachine::INITIAL_PC + 4 + 127 * 2), apsrFlagsEquals("nZcv"));
}

MICROMACHINE_TEST_F(b, BEQ_TakenWithLargesNegativeOffset, CpuTestFixture) {

	emitInstruction16("1101cccciiiiiiii", COND_EQ, -128);
	setZeroFlag(true);
	StepAndExpectThat(reg(registers::PC).equals(TestMachine::INITIAL_PC + 4 - 128 * 2), apsrFlagsEquals("nZcv"));
}

/* B - Encoding T2 (Unconditional)
   Encoding: 11100 Imm:11 */
MICROMACHINE_TEST_F(b, BAL_ZeroOffset, CpuTestFixture) {

	emitInstruction16("11100iiiiiiiiiii", 0);
	StepAndExpectThat(reg(registers::PC).wasIncrementedBy(4));
}

MICROMACHINE_TEST_F(b, BAL_LargestPositiveOffset, CpuTestFixture) {

	emitInstruction16("11100iiiiiiiiiii", 1023);
	StepAndExpectThat(reg(registers::PC).equals(TestMachine::INITIAL_PC + 4 + 1023 * 2));
}

MICROMACHINE_TEST_F(b, BAL_LargestNegativeOffset, CpuTestFixture) {

	emitInstruction16("11100iiiiiiiiiii", -1024);
	StepAndExpectThat(reg(registers::PC).equals(TestMachine::INITIAL_PC + 4 - 1024 * 2));
}
