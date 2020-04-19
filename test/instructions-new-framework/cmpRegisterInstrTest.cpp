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


/* CMP - Register - Encoding T1
   Encoding: 010000 1010 Rm:3 Rn:3 */
TEST_F(CpuTestHarness, cmpRegister_T1UseLowestRegisterForAllArgs)
{
	code_gen().emit_ins16("0100001010mmmnnn", registers::R0, registers::R0);
	setExpectedXPSRflags("nZCv");
	step();
}

TEST_F(CpuTestHarness, cmpRegister_T1UseHigestRegisterForAllArgs)
{
	code_gen().emit_ins16("0100001010mmmnnn", registers::R7, registers::R7);
	setExpectedXPSRflags("nZCv");
	step();
}

TEST_F(CpuTestHarness, cmpRegister_T1RnLargerThanRm)
{
	code_gen().emit_ins16("0100001010mmmnnn", registers::R1, registers::R2);
	setExpectedXPSRflags("nzCv");
	step();
}

TEST_F(CpuTestHarness, cmpRegister_T1RnSmallerThanRm)
{
	code_gen().emit_ins16("0100001010mmmnnn", registers::R1, registers::R0);
	setExpectedXPSRflags("Nzcv");
	setRegisterValue(registers::R1, 1);
	step();
}

TEST_F(CpuTestHarness, cmpRegister_T1ForceNegativeOverflow)
{
	code_gen().emit_ins16("0100001010mmmnnn", registers::R1, registers::R2);
	setExpectedXPSRflags("nzCV");
	setRegisterValue(registers::R2, 0x80000000U);
	setRegisterValue(registers::R1, 1U);
	step();
}

TEST_F(CpuTestHarness, cmpRegister_T1ForcePositiveOverflow)
{
	code_gen().emit_ins16("0100001010mmmnnn", registers::R1, registers::R2);
	setExpectedXPSRflags("NzcV");
	setRegisterValue(registers::R2, 0x7FFFFFFFU);
	setRegisterValue(registers::R1, -1U);
	step();
}



/* CMP - Register - Encoding T2
   Encoding: 010001 01 N:1 Rm:4 Rn:3
   NOTE: At least one register must be high register, registers::R8 - registers::R14. */
TEST_F(CpuTestHarness, cmpRegister_T2CompareLowestRegisterToHighestRegister)
{
	code_gen().emit_ins16("01000101nmmmmnnn", registers::R0, registers::LR);
	setRegisterValue(registers::LR, 0xEEEEEEEE);
	setExpectedXPSRflags("nzcv");
	step();
}

TEST_F(CpuTestHarness, cmpRegister_T2CompareHighestRegisterToLowestRegister)
{
	code_gen().emit_ins16("01000101nmmmmnnn", registers::LR, registers::R0);
	setRegisterValue(registers::LR, 0xEEEEEEEE);
	setExpectedXPSRflags("NzCv");
	step();
}

TEST_F(CpuTestHarness, cmpRegister_T2CompareR8ToItself)
{
	code_gen().emit_ins16("01000101nmmmmnnn", registers::R8, registers::R8);
	setExpectedXPSRflags("nZCv");
	step();
}

TEST_F(CpuTestHarness, cmpRegister_T2ForceNegativeOverflow)
{
	code_gen().emit_ins16("01000101nmmmmnnn", registers::R11, registers::R12);
	setExpectedXPSRflags("nzCV");
	setRegisterValue(registers::R11, 0x80000000U);
	setRegisterValue(registers::R12, 1U);
	step();
}

TEST_F(CpuTestHarness, cmpRegister_T2ForcePositiveOverflow)
{
	code_gen().emit_ins16("01000101nmmmmnnn", registers::R11, registers::R12);
	setExpectedXPSRflags("NzcV");
	setRegisterValue(registers::R11, 0x7FFFFFFFU);
	setRegisterValue(registers::R12, -1U);
	step();
}
/*
TEST_SIM_ONLY(cmpRegister, T2UnpredictableForBothArgsToBeLowRegisters)
{
    code_gen().emit_ins16("01000101nmmmmnnn", registers::R6, registers::R7);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(registers::PC, INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(cmpRegister, T2UnpredictableForRnToBeR15)
{
    code_gen().emit_ins16("01000101nmmmmnnn", registers::PC, registers::R8);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(registers::PC, INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(cmpRegister, T2UnpredictableForRmToBeR15)
{
    code_gen().emit_ins16("01000101nmmmmnnn", registers::R8, registers::PC);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(registers::PC, INITIAL_PC);
    step(&m_context);
}
*/
