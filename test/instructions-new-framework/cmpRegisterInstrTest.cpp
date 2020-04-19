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


/* CMP - Register - Encoding T1
   Encoding: 010000 1010 Rm:3 Rn:3 */
MICROMACHINE_TEST_F(cmpRegister, T1UseLowestRegisterForAllArgs, CpuTestFixture) {
	code_gen().emit_ins16("0100001010mmmnnn", registers::R0, registers::R0);
	Step();
	ExpectThat().XPSRFlagsEquals("nZCv");
}

MICROMACHINE_TEST_F(cmpRegister, T1UseHigestRegisterForAllArgs, CpuTestFixture) {
	getCpu().regs().set(registers::R7, 0x77777777);
	code_gen().emit_ins16("0100001010mmmnnn", registers::R7, registers::R7);
	Step();
	ExpectThat().XPSRFlagsEquals("nZCv");
}

MICROMACHINE_TEST_F(cmpRegister, T1RnLargerThanRm, CpuTestFixture) {
	getCpu().regs().set(registers::R1, 0x11111111);
	getCpu().regs().set(registers::R2, 0x22222222);
	code_gen().emit_ins16("0100001010mmmnnn", registers::R1, registers::R2);
	Step();
	ExpectThat().XPSRFlagsEquals("nzCv");
}

MICROMACHINE_TEST_F(cmpRegister, T1RnSmallerThanRm, CpuTestFixture) {
	getCpu().regs().set(registers::R1, 0x11111111);

	code_gen().emit_ins16("0100001010mmmnnn", registers::R1, registers::R0);
	getCpu().regs().set(registers::R1, 1);
	Step();
	ExpectThat().XPSRFlagsEquals("Nzcv");
}

MICROMACHINE_TEST_F(cmpRegister, T1ForceNegativeOverflow, CpuTestFixture) {
	code_gen().emit_ins16("0100001010mmmnnn", registers::R1, registers::R2);
	getCpu().regs().set(registers::R2, 0x80000000U);
	getCpu().regs().set(registers::R1, 1U);
	Step();
	ExpectThat().XPSRFlagsEquals("nzCV");
}

MICROMACHINE_TEST_F(cmpRegister, T1ForcePositiveOverflow, CpuTestFixture) {
	getCpu().regs().set(registers::R1, 0x11111111);
	getCpu().regs().set(registers::R2, 0x22222222);
	code_gen().emit_ins16("0100001010mmmnnn", registers::R1, registers::R2);
	getCpu().regs().set(registers::R2, 0x7FFFFFFFU);
	getCpu().regs().set(registers::R1, -1U);
	Step();
	ExpectThat().XPSRFlagsEquals("NzcV");
}



/* CMP - Register - Encoding T2
   Encoding: 010001 01 N:1 Rm:4 Rn:3
   NOTE: At least one register must be high register, registers::R8 - registers::R14. */
MICROMACHINE_TEST_F(cmpRegister, T2CompareLowestRegisterToHighestRegister, CpuTestFixture) {
	code_gen().emit_ins16("01000101nmmmmnnn", registers::R0, registers::LR);
	getCpu().regs().set(registers::LR, 0xEEEEEEEE);
	Step();
	ExpectThat().XPSRFlagsEquals("nzcv");
}

MICROMACHINE_TEST_F(cmpRegister, T2CompareHighestRegisterToLowestRegister, CpuTestFixture) {
	code_gen().emit_ins16("01000101nmmmmnnn", registers::LR, registers::R0);
	getCpu().regs().set(registers::LR, 0xEEEEEEEE);
	Step();
	ExpectThat().XPSRFlagsEquals("NzCv");
}

MICROMACHINE_TEST_F(cmpRegister, T2CompareR8ToItself, CpuTestFixture) {
	getCpu().regs().set(registers::R8, 0x88888888);
	code_gen().emit_ins16("01000101nmmmmnnn", registers::R8, registers::R8);
	Step();
	ExpectThat().XPSRFlagsEquals("nZCv");
}

MICROMACHINE_TEST_F(cmpRegister, T2ForceNegativeOverflow, CpuTestFixture) {
	code_gen().emit_ins16("01000101nmmmmnnn", registers::R11, registers::R12);
	getCpu().regs().set(registers::R11, 0x80000000U);
	getCpu().regs().set(registers::R12, 1U);
	Step();
	ExpectThat().XPSRFlagsEquals("nzCV");
}

MICROMACHINE_TEST_F(cmpRegister, T2ForcePositiveOverflow, CpuTestFixture) {
	code_gen().emit_ins16("01000101nmmmmnnn", registers::R11, registers::R12);
	getCpu().regs().set(registers::R11, 0x7FFFFFFFU);
	getCpu().regs().set(registers::R12, -1U);
	Step();
	ExpectThat().XPSRFlagsEquals("NzcV");
}
/*
TEST_SIM_ONLY(cmpRegister, T2UnpredictableForBothArgsToBeLowRegisters)
{
    code_gen().emit_ins16("01000101nmmmmnnn", registers::R6, registers::R7);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    ExpectThat().Register(registers::PC).Equals(INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(cmpRegister, T2UnpredictableForRnToBeR15)
{
    code_gen().emit_ins16("01000101nmmmmnnn", registers::PC, registers::R8);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    ExpectThat().Register(registers::PC).Equals(INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(cmpRegister, T2UnpredictableForRmToBeR15)
{
    code_gen().emit_ins16("01000101nmmmmnnn", registers::R8, registers::PC);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    ExpectThat().Register(registers::PC).Equals(INITIAL_PC);
    step(&m_context);
}
*/
