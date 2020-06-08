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

#include "CpuTestFixture.hpp"
using namespace micromachine::testing;

/* CPS
   Encoding: 1011 0110 011 im:1 (0)(0)(1)(0) */

MICROMACHINE_TEST_F(cps, InterruptEnable, CpuTestFixture) {
	emitInstruction16("10110110011i0010", 0);
	setPrimaskFlag(true);
	StepAndExpectThatInstruction16IsExecutedAndThat(primaskIsNotSet());
}

MICROMACHINE_TEST_F(cps, InterruptDisable, CpuTestFixture) {
	emitInstruction16("10110110011i0010", 1);
	setPrimaskFlag(false);
	StepAndExpectThatInstruction16IsExecutedAndThat(primaskIsSet());
}

/*
TEST_SIM_ONLY(cps, UnpredictableBecauseOfBit0)
{
	code_gen().emit_ins16("10110110011i0011", 0);
	setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
	reg(registers::PC).equals(TestMachine::TestMachine::INITIAL_PC),
	step(&m_context);
}
*//*
TEST_SIM_ONLY(cps, UnpredictableBecauseOfBit1)
{
    code_gen().emit_ins16("10110110011i0000", 0);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    reg(registers::PC).equals(TestMachine::TestMachine::INITIAL_PC),
    step(&m_context);
}
*//*
TEST_SIM_ONLY(cps, UnpredictableBecauseOfBit2)
{
    code_gen().emit_ins16("10110110011i0110", 0);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    reg(registers::PC).equals(TestMachine::TestMachine::INITIAL_PC),
    step(&m_context);
}
*//*
TEST_SIM_ONLY(cps, UnpredictableBecauseOfBit3)
{
    code_gen().emit_ins16("10110110011i1010", 0);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    reg(registers::PC).equals(TestMachine::TestMachine::INITIAL_PC),
    step(&m_context);
}
*/
