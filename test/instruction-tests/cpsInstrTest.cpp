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

/* CPS
   Encoding: 1011 0110 011 im:1 (0)(0)(1)(0) */

MICROMACHINE_TEST_F(cps, InterruptEnable, CpuTestFixture) {
	code_gen().emit_ins16("10110110011i0010", 0);
	getCpu().special_regs().primask_register().set_pm(true);
	Step();
	ExpectThat().PrimaskStatusIs(false);
}

MICROMACHINE_TEST_F(cps, InterruptDisable, CpuTestFixture) {
	code_gen().emit_ins16("10110110011i0010", 1);
	getCpu().special_regs().primask_register().set_pm(false);
	Step();
	ExpectThat().PrimaskStatusIs(true);
}

/*
TEST_SIM_ONLY(cps, UnpredictableBecauseOfBit0)
{
    code_gen().emit_ins16("10110110011i0011", 0);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    ExpectThat().Register(registers::PC).Equals(INITIAL_PC);
    step(&m_context);
}
*//*
TEST_SIM_ONLY(cps, UnpredictableBecauseOfBit1)
{
    code_gen().emit_ins16("10110110011i0000", 0);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    ExpectThat().Register(registers::PC).Equals(INITIAL_PC);
    step(&m_context);
}
*//*
TEST_SIM_ONLY(cps, UnpredictableBecauseOfBit2)
{
    code_gen().emit_ins16("10110110011i0110", 0);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    ExpectThat().Register(registers::PC).Equals(INITIAL_PC);
    step(&m_context);
}
*//*
TEST_SIM_ONLY(cps, UnpredictableBecauseOfBit3)
{
    code_gen().emit_ins16("10110110011i1010", 0);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    ExpectThat().Register(registers::PC).Equals(INITIAL_PC);
    step(&m_context);
}
*/
