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
	code_gen().emit_ins16("1101cccciiiiiiii", COND_EQ, 0);
	// These tests set the APSR flags to specific value and expect them to be unmodified upon return.
	getCpu().regs().app_status_register().write_zero_flag(false);
	Step();
	ExpectThat().XPSRFlagsEquals("z");
}

MICROMACHINE_TEST_F(b, BEQ_Taken, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("1101cccciiiiiiii", COND_EQ, 0);
	getCpu().regs().app_status_register().write_zero_flag(true);
	Step();
	ExpectThat().XPSRFlagsEquals("Z");
	ExpectThat().Register(registers::PC).WasIncrementedBy(4)
}

MICROMACHINE_TEST_F(b, BNE_NotTaken, CpuTestFixture) {
	code_gen().emit_ins16("1101cccciiiiiiii", COND_NE, 0);
	getCpu().regs().app_status_register().write_zero_flag(true);
	Step();
	ExpectThat().XPSRFlagsEquals("Z");
}

MICROMACHINE_TEST_F(b, BNE_Taken, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("1101cccciiiiiiii", COND_NE, 0);
	getCpu().regs().app_status_register().write_zero_flag(false);
	Step();
	ExpectThat().XPSRFlagsEquals("z");
	ExpectThat().Register(registers::PC).WasIncrementedBy(4)
}

MICROMACHINE_TEST_F(b, BCS_NotTaken, CpuTestFixture) {
	code_gen().emit_ins16("1101cccciiiiiiii", COND_CS, 0);
	getCpu().regs().app_status_register().write_carry_flag(false);
	Step();
	ExpectThat().XPSRFlagsEquals("c");
}

MICROMACHINE_TEST_F(b, BCS_Taken, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("1101cccciiiiiiii", COND_CS, 0);
	getCpu().regs().app_status_register().write_carry_flag(true);
	Step();
	ExpectThat().XPSRFlagsEquals("C");
	ExpectThat().Register(registers::PC).WasIncrementedBy(4)
}

MICROMACHINE_TEST_F(b, BCC_NotTaken, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("1101cccciiiiiiii", COND_CC, 0);
	getCpu().regs().app_status_register().write_carry_flag(true);
	Step();
	ExpectThat().XPSRFlagsEquals("C");
}

MICROMACHINE_TEST_F(b, BCC_Taken, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("1101cccciiiiiiii", COND_CC, 0);
	getCpu().regs().app_status_register().write_carry_flag(false);
	Step();
	ExpectThat().XPSRFlagsEquals("c");
	ExpectThat().Register(registers::PC).WasIncrementedBy(4)
}

MICROMACHINE_TEST_F(b, BMI_NotTaken, CpuTestFixture) {
	code_gen().emit_ins16("1101cccciiiiiiii", COND_MI, 0);
	getCpu().regs().app_status_register().write_neg_flag(false);
	Step();
	ExpectThat().XPSRFlagsEquals("n");
}

MICROMACHINE_TEST_F(b, BMI_Taken, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("1101cccciiiiiiii", COND_MI, 0);
	getCpu().regs().app_status_register().write_neg_flag(true);
	Step();
	ExpectThat().Register(registers::PC).WasIncrementedBy(4)
	ExpectThat().XPSRFlagsEquals("N");
}

MICROMACHINE_TEST_F(b, BPL_NotTaken, CpuTestFixture) {
	code_gen().emit_ins16("1101cccciiiiiiii", COND_PL, 0);
	getCpu().regs().app_status_register().write_neg_flag(true);
	Step();
	ExpectThat().XPSRFlagsEquals("N");
}

MICROMACHINE_TEST_F(b, BPL_Taken, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("1101cccciiiiiiii", COND_PL, 0);
	getCpu().regs().app_status_register().write_neg_flag(false);
	Step();
	ExpectThat().Register(registers::PC).WasIncrementedBy(4)
	ExpectThat().XPSRFlagsEquals("n");
}

MICROMACHINE_TEST_F(b, BVS_NotTaken, CpuTestFixture) {
	code_gen().emit_ins16("1101cccciiiiiiii", COND_VS, 0);
	getCpu().regs().app_status_register().write_overflow_flag(false);
	Step();
	ExpectThat().XPSRFlagsEquals("v");
}

MICROMACHINE_TEST_F(b, BVS_Taken, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("1101cccciiiiiiii", COND_VS, 0);
	getCpu().regs().app_status_register().write_overflow_flag(true);
	Step();
	ExpectThat().Register(registers::PC).WasIncrementedBy(4)
	ExpectThat().XPSRFlagsEquals("V");
}

MICROMACHINE_TEST_F(b, BVC_NotTaken, CpuTestFixture) {
	code_gen().emit_ins16("1101cccciiiiiiii", COND_VC, 0);
	getCpu().regs().app_status_register().write_overflow_flag(true);
	Step();
	ExpectThat().XPSRFlagsEquals("V");
}

MICROMACHINE_TEST_F(b, BVC_Taken, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("1101cccciiiiiiii", COND_VC, 0);
	getCpu().regs().app_status_register().write_overflow_flag(false);
	Step();
	ExpectThat().XPSRFlagsEquals("v");
	ExpectThat().Register(registers::PC).WasIncrementedBy(4)
}

MICROMACHINE_TEST_F(b, BHI_NotTaken, CpuTestFixture) {
	code_gen().emit_ins16("1101cccciiiiiiii", COND_HI, 0);
	getCpu().regs().app_status_register().write_carry_flag(false);
	getCpu().regs().app_status_register().write_zero_flag(true);
	Step();
	ExpectThat().XPSRFlagsEquals("cZ");
}

MICROMACHINE_TEST_F(b, BHI_Taken, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("1101cccciiiiiiii", COND_HI, 0);
	getCpu().regs().app_status_register().write_carry_flag(true);
	getCpu().regs().app_status_register().write_zero_flag(false);
	Step();
	ExpectThat().Register(registers::PC).WasIncrementedBy(4)
	ExpectThat().XPSRFlagsEquals("Cz");
}

MICROMACHINE_TEST_F(b, BLS_NotTaken, CpuTestFixture) {
	code_gen().emit_ins16("1101cccciiiiiiii", COND_LS, 0);
	getCpu().regs().app_status_register().write_carry_flag(true);
	getCpu().regs().app_status_register().write_zero_flag(false);
	Step();
	ExpectThat().XPSRFlagsEquals("Cz");
}

MICROMACHINE_TEST_F(b, BLS_Taken, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("1101cccciiiiiiii", COND_LS, 0);
	getCpu().regs().app_status_register().write_carry_flag(false);
	getCpu().regs().app_status_register().write_zero_flag(true);
	Step();
	ExpectThat().Register(registers::PC).WasIncrementedBy(4)
	ExpectThat().XPSRFlagsEquals("cZ");
}

MICROMACHINE_TEST_F(b, BGE_NotTaken, CpuTestFixture) {
	code_gen().emit_ins16("1101cccciiiiiiii", COND_GE, 0);
	getCpu().regs().app_status_register().write_neg_flag(true);
	getCpu().regs().app_status_register().write_overflow_flag(false);
	Step();
	ExpectThat().XPSRFlagsEquals("Nv");
}

MICROMACHINE_TEST_F(b, BGE_Taken1, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("1101cccciiiiiiii", COND_GE, 0);
	getCpu().regs().app_status_register().write_neg_flag(true);
	getCpu().regs().app_status_register().write_overflow_flag(true);
	Step();
	ExpectThat().Register(registers::PC).WasIncrementedBy(4)
	ExpectThat().XPSRFlagsEquals("NV");
}

MICROMACHINE_TEST_F(b, BGE_Taken2, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("1101cccciiiiiiii", COND_GE, 0);
	getCpu().regs().app_status_register().write_neg_flag(false);
	getCpu().regs().app_status_register().write_overflow_flag(false);
	Step();
	ExpectThat().XPSRFlagsEquals("nv");
	ExpectThat().Register(registers::PC).WasIncrementedBy(4)
}

MICROMACHINE_TEST_F(b, BLT_NotTaken, CpuTestFixture) {
	code_gen().emit_ins16("1101cccciiiiiiii", COND_LT, 0);
	getCpu().regs().app_status_register().write_neg_flag(true);
	getCpu().regs().app_status_register().write_overflow_flag(true);
	Step();
	ExpectThat().XPSRFlagsEquals("NV");
}

MICROMACHINE_TEST_F(b, BLT_Taken1, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("1101cccciiiiiiii", COND_LT, 0);
	getCpu().regs().app_status_register().write_neg_flag(true);
	getCpu().regs().app_status_register().write_overflow_flag(false);
	Step();
	ExpectThat().XPSRFlagsEquals("Nv");
	ExpectThat().Register(registers::PC).WasIncrementedBy(4)
}

MICROMACHINE_TEST_F(b, BLT_Taken2, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("1101cccciiiiiiii", COND_LT, 0);
	getCpu().regs().app_status_register().write_neg_flag(false);
	getCpu().regs().app_status_register().write_overflow_flag(true);
	Step();
	ExpectThat().XPSRFlagsEquals("nV");
	ExpectThat().Register(registers::PC).WasIncrementedBy(4)
}

MICROMACHINE_TEST_F(b, BGT_NotTaken, CpuTestFixture) {
	code_gen().emit_ins16("1101cccciiiiiiii", COND_GT, 0);
	getCpu().regs().app_status_register().write_zero_flag(true);
	getCpu().regs().app_status_register().write_neg_flag(true);
	getCpu().regs().app_status_register().write_overflow_flag(true);
	Step();
	ExpectThat().XPSRFlagsEquals("ZNV");
}

MICROMACHINE_TEST_F(b, BGT_Taken1, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("1101cccciiiiiiii", COND_GT, 0);
	getCpu().regs().app_status_register().write_zero_flag(false);
	getCpu().regs().app_status_register().write_neg_flag(false);
	getCpu().regs().app_status_register().write_overflow_flag(false);
	Step();
	ExpectThat().Register(registers::PC).WasIncrementedBy(4)
	ExpectThat().XPSRFlagsEquals("znv");
}

MICROMACHINE_TEST_F(b, BGT_Taken2, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("1101cccciiiiiiii", COND_GT, 0);
	getCpu().regs().app_status_register().write_zero_flag(false);
	getCpu().regs().app_status_register().write_neg_flag(true);
	getCpu().regs().app_status_register().write_overflow_flag(true);
	Step();
	ExpectThat().Register(registers::PC).WasIncrementedBy(4)
	ExpectThat().XPSRFlagsEquals("zNV");
}

MICROMACHINE_TEST_F(b, BLE_NotTaken, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("1101cccciiiiiiii", COND_LE, 0);
	getCpu().regs().app_status_register().write_zero_flag(false);
	getCpu().regs().app_status_register().write_neg_flag(true);
	getCpu().regs().app_status_register().write_overflow_flag(true);
	Step();
	ExpectThat().XPSRFlagsEquals("zNV");
}

MICROMACHINE_TEST_F(b, BLE_Taken1, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("1101cccciiiiiiii", COND_LE, 0);
	getCpu().regs().app_status_register().write_zero_flag(true);
	getCpu().regs().app_status_register().write_neg_flag(true);
	getCpu().regs().app_status_register().write_overflow_flag(false);
	Step();
	ExpectThat().Register(registers::PC).WasIncrementedBy(4)
	ExpectThat().XPSRFlagsEquals("ZNv");
}

MICROMACHINE_TEST_F(b, BLE_Taken2, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("1101cccciiiiiiii", COND_LE, 0);
	getCpu().regs().app_status_register().write_zero_flag(true);
	getCpu().regs().app_status_register().write_neg_flag(false);
	getCpu().regs().app_status_register().write_overflow_flag(true);
	Step();
	ExpectThat().Register(registers::PC).WasIncrementedBy(4)
	ExpectThat().XPSRFlagsEquals("ZnV");
}

MICROMACHINE_TEST_F(b, BEQ_TakenWithLargestPositiveOffset, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("1101cccciiiiiiii", COND_EQ, 127);
	getCpu().regs().app_status_register().write_zero_flag(true);
	Step();
	ExpectThat().Register(registers::PC).Equals(INITIAL_PC + 4 + 127 * 2);
	ExpectThat().XPSRFlagsEquals("Z");
}

MICROMACHINE_TEST_F(b, BEQ_TakenWithLargesNegativeOffset, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("1101cccciiiiiiii", COND_EQ, -128);
	getCpu().regs().app_status_register().write_zero_flag(true);
	Step();
	ExpectThat().Register(registers::PC).Equals(INITIAL_PC + 4 - 128 * 2);
	ExpectThat().XPSRFlagsEquals("Z");
}



/* B - Encoding T2 (Unconditional)
   Encoding: 11100 Imm:11 */
MICROMACHINE_TEST_F(b, BAL_ZeroOffset, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("11100iiiiiiiiiii", 0);
	Step();
	ExpectThat().Register(registers::PC).WasIncrementedBy(4)
}

MICROMACHINE_TEST_F(b, BAL_LargestPositiveOffset, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("11100iiiiiiiiiii", 1023);
	Step();
	ExpectThat().Register(registers::PC).Equals(INITIAL_PC + 4 + 1023 * 2);
}

MICROMACHINE_TEST_F(b, BAL_LargestNegativeOffset, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("11100iiiiiiiiiii", -1024);
	Step();
	ExpectThat().Register(registers::PC).Equals(INITIAL_PC + 4 - 1024 * 2);
}
