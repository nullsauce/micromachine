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

#include "CpuTestHarness.hpp"


/* B - Encoding T1 (Conditional)
   Encoding: 1101 Cond:4 Imm:8 */
TEST_F(CpuTestHarness, b_BEQ_NotTaken)
{
	code_gen().emit_ins16("1101cccciiiiiiii", COND_EQ, 0);
	// These tests set the APSR flags to specific value and expect them to be unmodified upon return.
	setExpectedXPSRflags("z");
	clearZero();
	step();
}

TEST_F(CpuTestHarness, b_BEQ_Taken)
{
	code_gen().emit_ins16("1101cccciiiiiiii", COND_EQ, 0);
	setExpectedXPSRflags("Z");
	setZero();
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BNE_NotTaken)
{
	code_gen().emit_ins16("1101cccciiiiiiii", COND_NE, 0);
	setExpectedXPSRflags("Z");
	setZero();
	step();
}

TEST_F(CpuTestHarness, b_BNE_Taken)
{
	code_gen().emit_ins16("1101cccciiiiiiii", COND_NE, 0);
	setExpectedXPSRflags("z");
	clearZero();
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BCS_NotTaken)
{
	code_gen().emit_ins16("1101cccciiiiiiii", COND_CS, 0);
	setExpectedXPSRflags("c");
	clearCarry();
	step();
}

TEST_F(CpuTestHarness, b_BCS_Taken)
{
	code_gen().emit_ins16("1101cccciiiiiiii", COND_CS, 0);
	setExpectedXPSRflags("C");
	setCarry();
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BCC_NotTaken)
{
	code_gen().emit_ins16("1101cccciiiiiiii", COND_CC, 0);
	setExpectedXPSRflags("C");
	setCarry();
	step();
}

TEST_F(CpuTestHarness, b_BCC_Taken)
{
	code_gen().emit_ins16("1101cccciiiiiiii", COND_CC, 0);
	setExpectedXPSRflags("c");
	clearCarry();
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BMI_NotTaken)
{
	code_gen().emit_ins16("1101cccciiiiiiii", COND_MI, 0);
	setExpectedXPSRflags("n");
	clearNegative();
	step();
}

TEST_F(CpuTestHarness, b_BMI_Taken)
{
	code_gen().emit_ins16("1101cccciiiiiiii", COND_MI, 0);
	setExpectedXPSRflags("N");
	setNegative();
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BPL_NotTaken)
{
	code_gen().emit_ins16("1101cccciiiiiiii", COND_PL, 0);
	setExpectedXPSRflags("N");
	setNegative();
	step();
}

TEST_F(CpuTestHarness, b_BPL_Taken)
{
	code_gen().emit_ins16("1101cccciiiiiiii", COND_PL, 0);
	setExpectedXPSRflags("n");
	clearNegative();
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BVS_NotTaken)
{
	code_gen().emit_ins16("1101cccciiiiiiii", COND_VS, 0);
	setExpectedXPSRflags("v");
	clearOverflow();
	step();
}

TEST_F(CpuTestHarness, b_BVS_Taken)
{
	code_gen().emit_ins16("1101cccciiiiiiii", COND_VS, 0);
	setExpectedXPSRflags("V");
	setOverflow();
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BVC_NotTaken)
{
	code_gen().emit_ins16("1101cccciiiiiiii", COND_VC, 0);
	setExpectedXPSRflags("V");
	setOverflow();
	step();
}

TEST_F(CpuTestHarness, b_BVC_Taken)
{
	code_gen().emit_ins16("1101cccciiiiiiii", COND_VC, 0);
	setExpectedXPSRflags("v");
	clearOverflow();
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BHI_NotTaken)
{
	code_gen().emit_ins16("1101cccciiiiiiii", COND_HI, 0);
	setExpectedXPSRflags("cZ");
	clearCarry();
	setZero();
	step();
}

TEST_F(CpuTestHarness, b_BHI_Taken)
{
	code_gen().emit_ins16("1101cccciiiiiiii", COND_HI, 0);
	setExpectedXPSRflags("Cz");
	setCarry();
	clearZero();
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BLS_NotTaken)
{
	code_gen().emit_ins16("1101cccciiiiiiii", COND_LS, 0);
	setExpectedXPSRflags("Cz");
	setCarry();
	clearZero();
	step();
}

TEST_F(CpuTestHarness, b_BLS_Taken)
{
	code_gen().emit_ins16("1101cccciiiiiiii", COND_LS, 0);
	setExpectedXPSRflags("cZ");
	clearCarry();
	setZero();
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BGE_NotTaken)
{
	code_gen().emit_ins16("1101cccciiiiiiii", COND_GE, 0);
	setExpectedXPSRflags("Nv");
	setNegative();
	clearOverflow();
	step();
}

TEST_F(CpuTestHarness, b_BGE_Taken1)
{
	code_gen().emit_ins16("1101cccciiiiiiii", COND_GE, 0);
	setExpectedXPSRflags("NV");
	setNegative();
	setOverflow();
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BGE_Taken2)
{
	code_gen().emit_ins16("1101cccciiiiiiii", COND_GE, 0);
	setExpectedXPSRflags("nv");
	clearNegative();
	clearOverflow();
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BLT_NotTaken)
{
	code_gen().emit_ins16("1101cccciiiiiiii", COND_LT, 0);
	setExpectedXPSRflags("NV");
	setNegative();
	setOverflow();
	step();
}

TEST_F(CpuTestHarness, b_BLT_Taken1)
{
	code_gen().emit_ins16("1101cccciiiiiiii", COND_LT, 0);
	setExpectedXPSRflags("Nv");
	setNegative();
	clearOverflow();
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BLT_Taken2)
{
	code_gen().emit_ins16("1101cccciiiiiiii", COND_LT, 0);
	setExpectedXPSRflags("nV");
	clearNegative();
	setOverflow();
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BGT_NotTaken)
{
	code_gen().emit_ins16("1101cccciiiiiiii", COND_GT, 0);
	setExpectedXPSRflags("ZNV");
	setZero();
	setNegative();
	setOverflow();
	step();
}

TEST_F(CpuTestHarness, b_BGT_Taken1)
{
	code_gen().emit_ins16("1101cccciiiiiiii", COND_GT, 0);
	setExpectedXPSRflags("znv");
	clearZero();
	clearNegative();
	clearOverflow();
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BGT_Taken2)
{
	code_gen().emit_ins16("1101cccciiiiiiii", COND_GT, 0);
	setExpectedXPSRflags("zNV");
	clearZero();
	setNegative();
	setOverflow();
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BLE_NotTaken)
{
	code_gen().emit_ins16("1101cccciiiiiiii", COND_LE, 0);
	setExpectedXPSRflags("zNV");
	clearZero();
	setNegative();
	setOverflow();
	step();
}

TEST_F(CpuTestHarness, b_BLE_Taken1)
{
	code_gen().emit_ins16("1101cccciiiiiiii", COND_LE, 0);
	setExpectedXPSRflags("ZNv");
	setZero();
	setNegative();
	clearOverflow();
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BLE_Taken2)
{
	code_gen().emit_ins16("1101cccciiiiiiii", COND_LE, 0);
	setExpectedXPSRflags("ZnV");
	setZero();
	clearNegative();
	setOverflow();
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BEQ_TakenWithLargestPositiveOffset)
{
	code_gen().emit_ins16("1101cccciiiiiiii", COND_EQ, 127);
	setExpectedXPSRflags("Z");
	setZero();
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4 + 127 * 2);
	step();
}

TEST_F(CpuTestHarness, b_BEQ_TakenWithLargesNegativeOffset)
{
	code_gen().emit_ins16("1101cccciiiiiiii", COND_EQ, -128);
	setExpectedXPSRflags("Z");
	setZero();
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4 - 128 * 2);
	step();
}



/* B - Encoding T2 (Unconditional)
   Encoding: 11100 Imm:11 */
TEST_F(CpuTestHarness, b_BAL_ZeroOffset)
{
	code_gen().emit_ins16("11100iiiiiiiiiii", 0);
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BAL_LargestPositiveOffset)
{
	code_gen().emit_ins16("11100iiiiiiiiiii", 1023);
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4 + 1023 * 2);
	step();
}

TEST_F(CpuTestHarness, b_BAL_LargestNegativeOffset)
{
	code_gen().emit_ins16("11100iiiiiiiiiii", -1024);
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4 - 1024 * 2);
	step();
}
