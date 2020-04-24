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


// IPSR value can't be changed from within a debug monitor.
#ifdef THUNK2REAL
#define IPSR_VAL 0x0
#else
#define IPSR_VAL 0x20
#endif


/* MRS
   Encoding: 11110 0 1111 1 (0) (1)(1)(1)(1)
             10 (0) 0 Rd:4 SYSm:8 */
TEST_F(CpuTestHarness, mrs_FromAPSR)
{
	code_gen().emit32bit("1111001111101111", "1000ddddssssssss", R12, SYS_APSR);

	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	setExpectedXPSRflags("NzCv");

	setNegative();
	clearZero();
	setCarry();
	clearOverflow();

	setIPSR(IPSR_VAL);
	setExpectedIPSR(IPSR_VAL);


	setRegisterValue(R12, 0xFFFFFFFF);
	setExpectedRegisterValue(R12, APSR_N | APSR_C);
	step();
}

TEST_F(CpuTestHarness, mrs_FromIAPSR)
{
	code_gen().emit32bit("1111001111101111", "1000ddddssssssss", R0, SYS_IAPSR);
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	setExpectedXPSRflags("NzCv");
	setNegative();
	clearZero();
	setCarry();
	clearOverflow();
	setIPSR(IPSR_VAL);
	setExpectedIPSR(IPSR_VAL);
	setRegisterValue(R0, 0xFFFFFFFF);
	setExpectedRegisterValue(R0, APSR_N | APSR_C | IPSR_VAL);
	step();
}

TEST_F(CpuTestHarness, mrs_FromEAPSR)
{
	code_gen().emit32bit("1111001111101111", "1000ddddssssssss", R12, SYS_EAPSR);
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	setExpectedXPSRflags("NzCv");
	setNegative();
	clearZero();
	setCarry();
	clearOverflow();
	setIPSR(IPSR_VAL);
	setExpectedIPSR(IPSR_VAL);
	setRegisterValue(R12, 0xFFFFFFFF);
	setExpectedRegisterValue(R12, APSR_N | APSR_C);
	step();
}

TEST_F(CpuTestHarness, mrs_FromXPSR)
{
	code_gen().emit32bit("1111001111101111", "1000ddddssssssss", R12, SYS_XPSR);
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	setExpectedXPSRflags("NzCv");
	setNegative();
	clearZero();
	setCarry();
	clearOverflow();
	setIPSR(IPSR_VAL);
	setExpectedIPSR(IPSR_VAL);
	setRegisterValue(R12, 0xFFFFFFFF);
	setExpectedRegisterValue(R12, APSR_N | APSR_C | IPSR_VAL);
	step();
}

TEST_F(CpuTestHarness, mrs_FromIPSR)
{
	code_gen().emit32bit("1111001111101111", "1000ddddssssssss", R12, SYS_IPSR);
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	setExpectedXPSRflags("NzCv");
	setNegative();
	clearZero();
	setCarry();
	clearOverflow();
	setIPSR(IPSR_VAL);
	setExpectedIPSR(IPSR_VAL);
	setRegisterValue(R12, 0xFFFFFFFF);
	setExpectedRegisterValue(R12, IPSR_VAL);
	step();
}

TEST_F(CpuTestHarness, mrs_FromEPSR)
{
	code_gen().emit32bit("1111001111101111", "1000ddddssssssss", R12, SYS_EPSR);
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	setExpectedXPSRflags("NzCv");
	setNegative();
	clearZero();
	setCarry();
	clearOverflow();
	setIPSR(IPSR_VAL);
	setExpectedIPSR(IPSR_VAL);
	setRegisterValue(R12, 0xFFFFFFFF);
	setExpectedRegisterValue(R12, 0);
	step();
}

TEST_F(CpuTestHarness, mrs_FromIEPSR)
{
	code_gen().emit32bit("1111001111101111", "1000ddddssssssss", R12, SYS_IEPSR);
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	setExpectedXPSRflags("NzCv");
	setNegative();
	clearZero();
	setCarry();
	clearOverflow();
	setIPSR(IPSR_VAL);
	setExpectedIPSR(IPSR_VAL);
	setRegisterValue(R12, 0xFFFFFFFF);
	setExpectedRegisterValue(R12, IPSR_VAL);
	step();
}

TEST_F(CpuTestHarness, mrs_FromMSP)
{
	code_gen().emit32bit("1111001111101111", "1000ddddssssssss", R12, SYS_MSP);
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	setRegisterValue(R12, 0xFFFFFFFF);
	setExpectedRegisterValue(R12, INITIAL_SP);
	step();
}

TEST_F(CpuTestHarness, mrs_FromPSP)
{
	code_gen().emit32bit("1111001111101111", "1000ddddssssssss", R12, SYS_PSP);
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	setRegisterValue(R12, 0xFFFFFFFF);
	setExpectedRegisterValue(R12, 0x0);
	step();
}

TEST_F(CpuTestHarness, mrs_FromPRIMASKsetTo1)
{
	code_gen().emit32bit("1111001111101111", "1000ddddssssssss", R12, SYS_PRIMASK);
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	setRegisterValue(R12, 0xFFFFFFFF);
	PRIMASK = 1;
	setExpectedRegisterValue(R12, 1);
	step();
}

TEST_F(CpuTestHarness, mrs_PRIMASKto0)
{
	code_gen().emit32bit("1111001111101111", "1000ddddssssssss", R12, SYS_PRIMASK);
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	setRegisterValue(R12, 0xFFFFFFFF);
	PRIMASK = 0;
	setExpectedRegisterValue(R12, 0);
	step();
}

TEST_F(CpuTestHarness, mrs_CONTROLIgnored)
{
	code_gen().emit32bit("1111001111101111", "1000ddddssssssss", R12, SYS_CONTROL);
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	setRegisterValue(R12, 0xFFFFFFFF);
	setExpectedRegisterValue(R12, 0);
	step();
}
/*
TEST_F(CpuTestHarness, mrs_R13IsUnpredictable)
{
    code_gen().emit_ins32("1111001111101111", "1000ddddssssssss", registers::SP, SYS_XPSR);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(registers::PC, INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_F(CpuTestHarness, mrs_R15IsUnpredictable)
{
    code_gen().emit_ins32("1111001111101111", "1000ddddssssssss", registers::PC, SYS_XPSR);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(registers::PC, INITIAL_PC);
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
            code_gen().emit_ins32("1111001111101111", "1000ddddssssssss", R0, i);
            setRegisterValue(R0, 0xFFFFFFFF);
            setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
            setExpectedRegisterValue(registers::PC, INITIAL_PC);
            pinkySimStep(&m_context);
        }
    }
}

TEST_SIM_ONLY(mrs, UnpredictableBecauseOfBit2_13)
{
    code_gen().emit_ins32("1111001111101111", "1010ddddssssssss", R12, SYS_XPSR);
    setRegisterValue(R0, 0xFFFFFFFF);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(registers::PC, INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(mrs, UnpredictableBecauseOfBit1_0)
{
    code_gen().emit_ins32("1111001111101110", "1000ddddssssssss", R12, SYS_XPSR);
    setRegisterValue(R0, 0xFFFFFFFF);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(registers::PC, INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(mrs, UnpredictableBecauseOfBit1_1)
{
    code_gen().emit_ins32("1111001111101101", "1000ddddssssssss", R12, SYS_XPSR);
    setRegisterValue(R0, 0xFFFFFFFF);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(registers::PC, INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(mrs, UnpredictableBecauseOfBit1_2)
{
    code_gen().emit_ins32("1111001111101011", "1000ddddssssssss", R12, SYS_XPSR);
    setRegisterValue(R0, 0xFFFFFFFF);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(registers::PC, INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(mrs, UnpredictableBecauseOfBit1_3)
{
    code_gen().emit_ins32("1111001111100111", "1000ddddssssssss", R12, SYS_XPSR);
    setRegisterValue(R0, 0xFFFFFFFF);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(registers::PC, INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(mrs, UnpredictableBecauseOfBit1_4)
{
    code_gen().emit_ins32("1111001111111111", "1000ddddssssssss", R12, SYS_XPSR);
    setRegisterValue(R0, 0xFFFFFFFF);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(registers::PC, INITIAL_PC);
    step(&m_context);
}
*/
