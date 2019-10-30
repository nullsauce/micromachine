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
	emitInstruction16("1101cccciiiiiiii", COND_EQ, 0);
	// These tests set the APSR flags to specific value and expect them to be unmodified upon return.
	setExpectedXPSRflags("z");
	clearZero();
	step();
}

TEST_F(CpuTestHarness, b_BEQ_Taken)
{
	emitInstruction16("1101cccciiiiiiii", COND_EQ, 0);
	setExpectedXPSRflags("Z");
	setZero();
	setExpectedRegisterValue(PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BNE_NotTaken)
{
	emitInstruction16("1101cccciiiiiiii", COND_NE, 0);
	setExpectedXPSRflags("Z");
	setZero();
	step();
}

TEST_F(CpuTestHarness, b_BNE_Taken)
{
	emitInstruction16("1101cccciiiiiiii", COND_NE, 0);
	setExpectedXPSRflags("z");
	clearZero();
	setExpectedRegisterValue(PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BCS_NotTaken)
{
	emitInstruction16("1101cccciiiiiiii", COND_CS, 0);
	setExpectedXPSRflags("c");
	clearCarry();
	step();
}

TEST_F(CpuTestHarness, b_BCS_Taken)
{
	emitInstruction16("1101cccciiiiiiii", COND_CS, 0);
	setExpectedXPSRflags("C");
	setCarry();
	setExpectedRegisterValue(PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BCC_NotTaken)
{
	emitInstruction16("1101cccciiiiiiii", COND_CC, 0);
	setExpectedXPSRflags("C");
	setCarry();
	step();
}

TEST_F(CpuTestHarness, b_BCC_Taken)
{
	emitInstruction16("1101cccciiiiiiii", COND_CC, 0);
	setExpectedXPSRflags("c");
	clearCarry();
	setExpectedRegisterValue(PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BMI_NotTaken)
{
	emitInstruction16("1101cccciiiiiiii", COND_MI, 0);
	setExpectedXPSRflags("n");
	clearNegative();
	step();
}

TEST_F(CpuTestHarness, b_BMI_Taken)
{
	emitInstruction16("1101cccciiiiiiii", COND_MI, 0);
	setExpectedXPSRflags("N");
	setNegative();
	setExpectedRegisterValue(PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BPL_NotTaken)
{
	emitInstruction16("1101cccciiiiiiii", COND_PL, 0);
	setExpectedXPSRflags("N");
	setNegative();
	step();
}

TEST_F(CpuTestHarness, b_BPL_Taken)
{
	emitInstruction16("1101cccciiiiiiii", COND_PL, 0);
	setExpectedXPSRflags("n");
	clearNegative();
	setExpectedRegisterValue(PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BVS_NotTaken)
{
	emitInstruction16("1101cccciiiiiiii", COND_VS, 0);
	setExpectedXPSRflags("v");
	clearOverflow();
	step();
}

TEST_F(CpuTestHarness, b_BVS_Taken)
{
	emitInstruction16("1101cccciiiiiiii", COND_VS, 0);
	setExpectedXPSRflags("V");
	setOverflow();
	setExpectedRegisterValue(PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BVC_NotTaken)
{
	emitInstruction16("1101cccciiiiiiii", COND_VC, 0);
	setExpectedXPSRflags("V");
	setOverflow();
	step();
}

TEST_F(CpuTestHarness, b_BVC_Taken)
{
	emitInstruction16("1101cccciiiiiiii", COND_VC, 0);
	setExpectedXPSRflags("v");
	clearOverflow();
	setExpectedRegisterValue(PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BHI_NotTaken)
{
	emitInstruction16("1101cccciiiiiiii", COND_HI, 0);
	setExpectedXPSRflags("cZ");
	clearCarry();
	setZero();
	step();
}

TEST_F(CpuTestHarness, b_BHI_Taken)
{
	emitInstruction16("1101cccciiiiiiii", COND_HI, 0);
	setExpectedXPSRflags("Cz");
	setCarry();
	clearZero();
	setExpectedRegisterValue(PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BLS_NotTaken)
{
	emitInstruction16("1101cccciiiiiiii", COND_LS, 0);
	setExpectedXPSRflags("Cz");
	setCarry();
	clearZero();
	step();
}

TEST_F(CpuTestHarness, b_BLS_Taken)
{
	emitInstruction16("1101cccciiiiiiii", COND_LS, 0);
	setExpectedXPSRflags("cZ");
	clearCarry();
	setZero();
	setExpectedRegisterValue(PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BGE_NotTaken)
{
	emitInstruction16("1101cccciiiiiiii", COND_GE, 0);
	setExpectedXPSRflags("Nv");
	setNegative();
	clearOverflow();
	step();
}

TEST_F(CpuTestHarness, b_BGE_Taken1)
{
	emitInstruction16("1101cccciiiiiiii", COND_GE, 0);
	setExpectedXPSRflags("NV");
	setNegative();
	setOverflow();
	setExpectedRegisterValue(PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BGE_Taken2)
{
	emitInstruction16("1101cccciiiiiiii", COND_GE, 0);
	setExpectedXPSRflags("nv");
	clearNegative();
	clearOverflow();
	setExpectedRegisterValue(PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BLT_NotTaken)
{
	emitInstruction16("1101cccciiiiiiii", COND_LT, 0);
	setExpectedXPSRflags("NV");
	setNegative();
	setOverflow();
	step();
}

TEST_F(CpuTestHarness, b_BLT_Taken1)
{
	emitInstruction16("1101cccciiiiiiii", COND_LT, 0);
	setExpectedXPSRflags("Nv");
	setNegative();
	clearOverflow();
	setExpectedRegisterValue(PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BLT_Taken2)
{
	emitInstruction16("1101cccciiiiiiii", COND_LT, 0);
	setExpectedXPSRflags("nV");
	clearNegative();
	setOverflow();
	setExpectedRegisterValue(PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BGT_NotTaken)
{
	emitInstruction16("1101cccciiiiiiii", COND_GT, 0);
	setExpectedXPSRflags("ZNV");
	setZero();
	setNegative();
	setOverflow();
	step();
}

TEST_F(CpuTestHarness, b_BGT_Taken1)
{
	emitInstruction16("1101cccciiiiiiii", COND_GT, 0);
	setExpectedXPSRflags("znv");
	clearZero();
	clearNegative();
	clearOverflow();
	setExpectedRegisterValue(PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BGT_Taken2)
{
	emitInstruction16("1101cccciiiiiiii", COND_GT, 0);
	setExpectedXPSRflags("zNV");
	clearZero();
	setNegative();
	setOverflow();
	setExpectedRegisterValue(PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BLE_NotTaken)
{
	emitInstruction16("1101cccciiiiiiii", COND_LE, 0);
	setExpectedXPSRflags("zNV");
	clearZero();
	setNegative();
	setOverflow();
	step();
}

TEST_F(CpuTestHarness, b_BLE_Taken1)
{
	emitInstruction16("1101cccciiiiiiii", COND_LE, 0);
	setExpectedXPSRflags("ZNv");
	setZero();
	setNegative();
	clearOverflow();
	setExpectedRegisterValue(PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BLE_Taken2)
{
	emitInstruction16("1101cccciiiiiiii", COND_LE, 0);
	setExpectedXPSRflags("ZnV");
	setZero();
	clearNegative();
	setOverflow();
	setExpectedRegisterValue(PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BEQ_TakenWithLargestPositiveOffset)
{
	emitInstruction16("1101cccciiiiiiii", COND_EQ, 127);
	setExpectedXPSRflags("Z");
	setZero();
	setExpectedRegisterValue(PC, INITIAL_PC + 4 + 127 * 2);
	step();
}

TEST_F(CpuTestHarness, b_BEQ_TakenWithLargesNegativeOffset)
{
	emitInstruction16("1101cccciiiiiiii", COND_EQ, -128);
	setExpectedXPSRflags("Z");
	setZero();
	setExpectedRegisterValue(PC, INITIAL_PC + 4 - 128 * 2);
	step();
}



/* B - Encoding T2 (Unconditional)
   Encoding: 11100 Imm:11 */
TEST_F(CpuTestHarness, b_BAL_ZeroOffset)
{
	emitInstruction16("11100iiiiiiiiiii", 0);
	setExpectedRegisterValue(PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, b_BAL_LargestPositiveOffset)
{
	emitInstruction16("11100iiiiiiiiiii", 1023);
	setExpectedRegisterValue(PC, INITIAL_PC + 4 + 1023 * 2);
	step();
}

TEST_F(CpuTestHarness, b_BAL_LargestNegativeOffset)
{
	emitInstruction16("11100iiiiiiiiiii", -1024);
	setExpectedRegisterValue(PC, INITIAL_PC + 4 - 1024 * 2);
	step();
}
