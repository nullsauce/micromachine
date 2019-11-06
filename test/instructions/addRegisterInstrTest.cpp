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


/* ADD - Register - Encoding T1
   Encoding: 000 11 0 0 Rm:3 Rn:3 Rd:3 */
TEST_F(CpuTestHarness, addRegister_T1UseLowestRegisterForAllArgs)
{
	code_gen().emit_ins16("0001100mmmnnnddd", R0, R0, R0);
	setExpectedXPSRflags("nZcv");
	setExpectedRegisterValue(R0, 0U);
	step();
}

TEST_F(CpuTestHarness, addRegister_T1UseHigestRegisterForAllArgs)
{
	code_gen().emit_ins16("0001100mmmnnnddd", R7, R7, R7);
	setExpectedXPSRflags("NzcV");
	setExpectedRegisterValue(R7, 0x77777777U + 0x77777777U);
	step();
}

TEST_F(CpuTestHarness, addRegister_T1UseDifferentRegistersForEachArg)
{
	code_gen().emit_ins16("0001100mmmnnnddd", R1, R2, R3);
	setExpectedXPSRflags("nzcv");
	setExpectedRegisterValue(R3, 0x11111111U + 0x22222222U);
	step();
}

// Force APSR flags to be set which haven't already been covered above.
TEST_F(CpuTestHarness, addRegister_T1ForceCarryWithNoOverflow)
{
	code_gen().emit_ins16("0001100mmmnnnddd", R1, R2, R0);
	setExpectedXPSRflags("nZCv");
	setRegisterValue(R1, -1);
	setRegisterValue(R2, 1);
	setExpectedRegisterValue(R0, -1 + 1);
	step();
}

TEST_F(CpuTestHarness, addRegister_T1ForceCarryAndOverflow)
{
	code_gen().emit_ins16("0001100mmmnnnddd", R1, R2, R0);
	setExpectedXPSRflags("nzCV");
	setRegisterValue(R1, -1);
	setRegisterValue(R2, 0x80000000U);
	setExpectedRegisterValue(R0, 0x7FFFFFFFU);
	step();
}



/* ADD - Register - Encoding T2
   Encoding: 010001 00 DN:1 Rm:4 Rdn:3
   NOTE: Shouldn't modify any of the APSR flags.*/
TEST_F(CpuTestHarness, addRegister_T2UseR1ForAllArgs)
{
	code_gen().emit_ins16("01000100dmmmmddd", R1, R1);
	setExpectedRegisterValue(R1, 0x11111111U + 0x11111111U);
	step();
}

TEST_F(CpuTestHarness, addRegister_T2UseLowestRegisterForAllArgs)
{
	code_gen().emit_ins16("01000100dmmmmddd", R0, R0);
	setExpectedRegisterValue(R0, 0U);
	step();
}

TEST_F(CpuTestHarness, addRegister_T2UseR12ForAllArgs)
{
	code_gen().emit_ins16("01000100dmmmmddd", R12, R12);
	setExpectedRegisterValue(R12, 0xCCCCCCCCU + 0xCCCCCCCCU);
	step();
}

TEST_F(CpuTestHarness, addRegister_T2UseDifferentRegistersForEachArg)
{
	code_gen().emit_ins16("01000100dmmmmddd", R2, R1);
	setExpectedRegisterValue(R2, 0x11111111U + 0x22222222U);
	step();
}

TEST_F(CpuTestHarness, addRegister_T2WrapAroundTo0)
{
	code_gen().emit_ins16("01000100dmmmmddd", R2, R1);
	setRegisterValue(R1, -1);
	setRegisterValue(R2, 1);
	setExpectedRegisterValue(R2, -1 + 1);
	step();
}

TEST_F(CpuTestHarness, addRegister_T2OverflowFromLowestNegativeValue)
{
	code_gen().emit_ins16("01000100dmmmmddd", R11, R10);
	setRegisterValue(R10, -1);
	setRegisterValue(R11, 0x80000000U);
	setExpectedRegisterValue(R11, 0x7FFFFFFF);
	step();
}

TEST_F(CpuTestHarness, addRegister_T2Add4ToSP)
{
	code_gen().emit_ins16("01000100dmmmmddd", SP, R1);
	setRegisterValue(SP, INITIAL_SP - 4);
	setRegisterValue(R1, 4);
	setExpectedRegisterValue(SP, INITIAL_SP - 4 + 4);
	step();
}

TEST_F(CpuTestHarness, addRegister_T2Subtract4FromSP)
{
	code_gen().emit_ins16("01000100dmmmmddd", SP, R1);
	setRegisterValue(R1, -4);
	setExpectedRegisterValue(SP, INITIAL_SP - 4);
	step();
}

TEST_F(CpuTestHarness, addRegister_T2Add1ToLR)
{
	code_gen().emit_ins16("01000100dmmmmddd", LR, R1);
	setRegisterValue(R1, 1);
	setExpectedRegisterValue(LR, INITIAL_LR + 1);
	step();
}

TEST_F(CpuTestHarness, addRegister_T2Add1ToPCWhichWillBeOddAndRoundedDown)
{
	code_gen().emit_ins16("01000100dmmmmddd", PC, R1);
	setRegisterValue(R1, 1);
	setExpectedRegisterValue(PC, (INITIAL_PC + 4 + 1) & 0xFFFFFFFE);
	step();
}

TEST_F(CpuTestHarness, addRegister_T2Add2ToPC)
{
	code_gen().emit_ins16("01000100dmmmmddd", PC, R1);
	setRegisterValue(R1, 2);
	setExpectedRegisterValue(PC, (INITIAL_PC + 4 + 2) & 0xFFFFFFFE);
	step();
}
/*
TEST_SIM_ONLY(addRegister, T2ItIsUnpredictableToHaveBothArgsBePC)
{
    code_gen().emit_ins16("01000100dmmmmddd", PC, PC);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(PC, INITIAL_PC);
    step(&m_context);
}
*/