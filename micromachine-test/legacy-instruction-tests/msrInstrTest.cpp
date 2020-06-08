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

#include "CpuTestHarness.hpp"


/* MSR
   Encoding: 11110 0 1110 0 (0) Rn:4
             10 (0) 0 (1) (0) (0) (0) SYSm:8 */
TEST_F(CpuTestHarness, msr_ToAPSR)
{
	code_gen().emit32bit("111100111000nnnn", "10001000ssssssss", R12, SYS_APSR);
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
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
	code_gen().emit32bit("111100111000nnnn", "10001000ssssssss", R12, SYS_IAPSR);
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
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
	code_gen().emit32bit("111100111000nnnn", "10001000ssssssss", R12, SYS_EAPSR);
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
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
	code_gen().emit32bit("111100111000nnnn", "10001000ssssssss", R0, SYS_XPSR);
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
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
	code_gen().emit32bit("111100111000nnnn", "10001000ssssssss", R12, SYS_IPSR);
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	setRegisterValue(R12, 0xFFFFFFFF);
	step();
}

TEST_F(CpuTestHarness, msr_ToEPSR)
{
	code_gen().emit32bit("111100111000nnnn", "10001000ssssssss", R12, SYS_EPSR);
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	setRegisterValue(R12, 0xFFFFFFFF);
	step();
}

TEST_F(CpuTestHarness, msr_ToIEPSR)
{
	code_gen().emit32bit("111100111000nnnn", "10001000ssssssss", R12, SYS_IEPSR);
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	setRegisterValue(R12, 0xFFFFFFFF);
	step();
}

TEST_F(CpuTestHarness, msr_ToMSP)
{
	code_gen().emit32bit("111100111000nnnn", "10001000ssssssss", R12, SYS_MSP);
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	setRegisterValue(R12, INITIAL_PC + 1024 + 2);
	setExpectedRegisterValue(registers::SP, INITIAL_PC + 1024);
	step();
}

TEST_F(CpuTestHarness, msr_ToPSP)
{
	code_gen().emit32bit("111100111000nnnn", "10001000ssssssss", R12, SYS_PSP);
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	setRegisterValue(R12, 0xFFFFFFFF);
	step();
}

TEST_F(CpuTestHarness, msr_PRIMASKto1)
{
	code_gen().emit32bit("111100111000nnnn", "10001000ssssssss", R12, SYS_PRIMASK);
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	setRegisterValue(R12, 0xFFFFFFFF);
	step();
	EXPECT_EQ(1, PRIMASK);
}

TEST_F(CpuTestHarness, msr_PRIMASKto0)
{
	code_gen().emit32bit("111100111000nnnn", "10001000ssssssss", R12, SYS_PRIMASK);
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	setRegisterValue(R12, 0xFFFFFFFE);
	step();
	EXPECT_EQ(0, PRIMASK);
}
/*
TEST_F(CpuTestHarness, msr_CONTROLIgnored)
{
    code_gen().emit_ins32("111100111000nnnn", "10001000ssssssss", R12, SYS_CONTROL);
    setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
    setRegisterValue(R12, 0xFFFFFFFF);
    step(&m_context);
    EXPECT_EQ(0, CONTROL);
}
*/
/*
TEST_SIM_ONLY(msr, R13IsUnpredictable)
{
    code_gen().emit_ins32("111100111000nnnn", "10001000ssssssss", registers::SP, SYS_XPSR);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(registers::PC, INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(msr, R15IsUnpredictable)
{
    code_gen().emit_ins32("111100111000nnnn", "10001000ssssssss", registers::PC, SYS_XPSR);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(registers::PC, INITIAL_PC);
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
            setExpectedRegisterValue(registers::PC, INITIAL_PC);
            pinkySimStep(&m_context);
        }
    }
}

TEST_SIM_ONLY(msr, UnpredictableBecauseOfBit2_8)
{
    code_gen().emit_ins32("111100111000nnnn", "10001001ssssssss", R0, SYS_XPSR);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(registers::PC, INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(msr, UnpredictableBecauseOfBit2_9)
{
    code_gen().emit_ins32("111100111000nnnn", "10001010ssssssss", R0, SYS_XPSR);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(registers::PC, INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(msr, UnpredictableBecauseOfBit2_10)
{
    code_gen().emit_ins32("111100111000nnnn", "10001100ssssssss", R0, SYS_XPSR);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(registers::PC, INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(msr, UnpredictableBecauseOfBit2_11)
{
    code_gen().emit_ins32("111100111000nnnn", "10000000ssssssss", R0, SYS_XPSR);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(registers::PC, INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(msr, UnpredictableBecauseOfBit2_13)
{
    code_gen().emit_ins32("111100111000nnnn", "10101000ssssssss", R0, SYS_XPSR);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(registers::PC, INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(msr, UnpredictableBecauseOfBit1_4)
{
    code_gen().emit_ins32("111100111001nnnn", "10001000ssssssss", R0, SYS_XPSR);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(registers::PC, INITIAL_PC);
    step(&m_context);
}
*/
