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


/* MSR
   Encoding: 11110 0 1110 0 (0) Rn:4
             10 (0) 0 (1) (0) (0) (0) SYSm:8 */
TEST_F(CpuTestHarness, msr_ToAPSR)
{
	code_gen().emit_ins32("111100111000nnnn", "10001000ssssssss", R12, SYS_APSR);
	setExpectedRegisterValue(PC, INITIAL_PC + 4);
	setExpectedXPSRflags("NZCV");
	clearNegative();
	clearZero();
	clearCarry();
	clearOverflow();
	setRegisterValue(R12, 0xFFFFFFFF);
	step();
}

TEST_F(CpuTestHarness, msr_ToIAPSR)
{
	code_gen().emit_ins32("111100111000nnnn", "10001000ssssssss", R12, SYS_IAPSR);
	setExpectedRegisterValue(PC, INITIAL_PC + 4);
	setExpectedXPSRflags("NZCV");
	clearNegative();
	clearZero();
	clearCarry();
	clearOverflow();
	setRegisterValue(R12, 0xFFFFFFFF);
	step();
}

TEST_F(CpuTestHarness, msr_ToEAPSR)
{
	code_gen().emit_ins32("111100111000nnnn", "10001000ssssssss", R12, SYS_EAPSR);
	setExpectedRegisterValue(PC, INITIAL_PC + 4);
	setExpectedXPSRflags("NZCV");
	clearNegative();
	clearZero();
	clearCarry();
	clearOverflow();
	setRegisterValue(R12, 0xFFFFFFFF);
	step();
}

TEST_F(CpuTestHarness, msr_ToXPSR)
{
	code_gen().emit_ins32("111100111000nnnn", "10001000ssssssss", R0, SYS_XPSR);
	setExpectedRegisterValue(PC, INITIAL_PC + 4);
	setExpectedXPSRflags("NZCV");
	clearNegative();
	clearZero();
	clearCarry();
	clearOverflow();
	setRegisterValue(R0, 0xFFFFFFFF);
	step();
}

TEST_F(CpuTestHarness, msr_ToIPSR)
{
	code_gen().emit_ins32("111100111000nnnn", "10001000ssssssss", R12, SYS_IPSR);
	setExpectedRegisterValue(PC, INITIAL_PC + 4);
	setRegisterValue(R12, 0xFFFFFFFF);
	step();
}

TEST_F(CpuTestHarness, msr_ToEPSR)
{
	code_gen().emit_ins32("111100111000nnnn", "10001000ssssssss", R12, SYS_EPSR);
	setExpectedRegisterValue(PC, INITIAL_PC + 4);
	setRegisterValue(R12, 0xFFFFFFFF);
	step();
}

TEST_F(CpuTestHarness, msr_ToIEPSR)
{
	code_gen().emit_ins32("111100111000nnnn", "10001000ssssssss", R12, SYS_IEPSR);
	setExpectedRegisterValue(PC, INITIAL_PC + 4);
	setRegisterValue(R12, 0xFFFFFFFF);
	step();
}

TEST_F(CpuTestHarness, msr_ToMSP)
{
	code_gen().emit_ins32("111100111000nnnn", "10001000ssssssss", R12, SYS_MSP);
	setExpectedRegisterValue(PC, INITIAL_PC + 4);
	setRegisterValue(R12, INITIAL_PC + 1024 + 2);
	setExpectedRegisterValue(SP, INITIAL_PC + 1024);
	step();
}

TEST_F(CpuTestHarness, msr_ToPSP)
{
	code_gen().emit_ins32("111100111000nnnn", "10001000ssssssss", R12, SYS_PSP);
	setExpectedRegisterValue(PC, INITIAL_PC + 4);
	setRegisterValue(R12, 0xFFFFFFFF);
	step();
}

TEST_F(CpuTestHarness, msr_PRIMASKto1)
{
	code_gen().emit_ins32("111100111000nnnn", "10001000ssssssss", R12, SYS_PRIMASK);
	setExpectedRegisterValue(PC, INITIAL_PC + 4);
	setRegisterValue(R12, 0xFFFFFFFF);
	step();
	EXPECT_EQ(1, PRIMASK);
}

TEST_F(CpuTestHarness, msr_PRIMASKto0)
{
	code_gen().emit_ins32("111100111000nnnn", "10001000ssssssss", R12, SYS_PRIMASK);
	setExpectedRegisterValue(PC, INITIAL_PC + 4);
	setRegisterValue(R12, 0xFFFFFFFE);
	step();
	EXPECT_EQ(0, PRIMASK);
}
/*
TEST_F(CpuTestHarness, msr_CONTROLIgnored)
{
    code_gen().emit_ins32("111100111000nnnn", "10001000ssssssss", R12, SYS_CONTROL);
    setExpectedRegisterValue(PC, INITIAL_PC + 4);
    setRegisterValue(R12, 0xFFFFFFFF);
    step(&m_context);
    EXPECT_EQ(0, CONTROL);
}
*/
/*
TEST_SIM_ONLY(msr, R13IsUnpredictable)
{
    code_gen().emit_ins32("111100111000nnnn", "10001000ssssssss", SP, SYS_XPSR);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(PC, INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(msr, R15IsUnpredictable)
{
    code_gen().emit_ins32("111100111000nnnn", "10001000ssssssss", PC, SYS_XPSR);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(PC, INITIAL_PC);
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
            code_gen().emit_ins32("111100111000nnnn", "10001000ssssssss", R0, i);
            setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
            setExpectedRegisterValue(PC, INITIAL_PC);
            pinkySimStep(&m_context);
        }
    }
}

TEST_SIM_ONLY(msr, UnpredictableBecauseOfBit2_8)
{
    code_gen().emit_ins32("111100111000nnnn", "10001001ssssssss", R0, SYS_XPSR);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(PC, INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(msr, UnpredictableBecauseOfBit2_9)
{
    code_gen().emit_ins32("111100111000nnnn", "10001010ssssssss", R0, SYS_XPSR);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(PC, INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(msr, UnpredictableBecauseOfBit2_10)
{
    code_gen().emit_ins32("111100111000nnnn", "10001100ssssssss", R0, SYS_XPSR);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(PC, INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(msr, UnpredictableBecauseOfBit2_11)
{
    code_gen().emit_ins32("111100111000nnnn", "10000000ssssssss", R0, SYS_XPSR);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(PC, INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(msr, UnpredictableBecauseOfBit2_13)
{
    code_gen().emit_ins32("111100111000nnnn", "10101000ssssssss", R0, SYS_XPSR);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(PC, INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(msr, UnpredictableBecauseOfBit1_4)
{
    code_gen().emit_ins32("111100111001nnnn", "10001000ssssssss", R0, SYS_XPSR);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(PC, INITIAL_PC);
    step(&m_context);
}
*/