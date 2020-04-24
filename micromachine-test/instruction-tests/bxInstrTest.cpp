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

/* BX (Branch and Exchange)
   Encoding: 010001 11 0 Rm:4 (0)(0)(0) */
MICROMACHINE_TEST_F(bx,
					UseLowestRegisterToBranchToEvenAddressWhichClearsThumbModeAndHardFaultOnNextInstr,
					CpuTestFixture) {

	emitInstruction16("010001110mmmm000", registers::R0);
	setReg(registers::R0, TestMachine::INITIAL_PC + 16);
	StepAndExpectThat(thumbFlagIsNotSet(),
					  reg(registers::PC).equals(TestMachine::INITIAL_PC + 16));
	StepAndExpectThat(hardfaultHandlerReached(), thumbFlagIsSet());
}

MICROMACHINE_TEST_F(bx, UseHighestRegisterToBranchToOddAddressWhichIsRequiredForThumb, CpuTestFixture) {

	emitInstruction16("010001110mmmm000", registers::LR);
	setReg(registers::LR, (TestMachine::INITIAL_PC + 16) | 1);
	StepAndExpectThat(reg(registers::PC).equals(TestMachine::INITIAL_PC + 16));
}

/*
TEST_SIM_ONLY(bx, UnpredictableToUseR15)
{
	code_gen().emit_ins16("010001110mmmm000", registers::PC));
	setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
	reg(registers::PC).equals(TestMachine::INITIAL_PC),
	pinkySimStep(&m_context);
}

TEST_SIM_ONLY(bx, UnpredictableForBit0ToBeHigh)
{
	code_gen().emit_ins16("010001110mmmm001", registers::R0);
	setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
	reg(registers::PC).equals(TestMachine::INITIAL_PC),
	pinkySimStep(&m_context);
}

TEST_SIM_ONLY(bx, UnpredictableForBit1ToBeHigh)
{
	code_gen().emit_ins16("010001110mmmm010", registers::R0);
	setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
	reg(registers::PC).equals(TestMachine::INITIAL_PC),
	pinkySimStep(&m_context);
}

TEST_SIM_ONLY(bx, UnpredictableForBit2ToBeHigh)
{
	code_gen().emit_ins16("010001110mmmm100", registers::R0);
	setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
	reg(registers::PC).equals(TestMachine::INITIAL_PC),
	step(&m_context);
}
*/
