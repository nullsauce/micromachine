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

/* CPS
   Encoding: 1011 0110 011 im:1 (0)(0)(1)(0) */

TEST_F(CpuTestHarness, cps_InterruptEnable)
{
	code_gen().emit_ins16("10110110011i0010", 0);
	PRIMASK |= PRIMASK_PM;
	step();
	EXPECT_FALSE(PRIMASK & PRIMASK_PM);
}

TEST_F(CpuTestHarness, cps_InterruptDisable)
{
	code_gen().emit_ins16("10110110011i0010", 1);
	PRIMASK &= ~PRIMASK_PM;
	step();
	EXPECT_TRUE(PRIMASK & PRIMASK_PM);
}

/*
TEST_SIM_ONLY(cps, UnpredictableBecauseOfBit0)
{
    code_gen().emit_ins16("10110110011i0011", 0);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(registers::PC, INITIAL_PC);
    step(&m_context);
}
*//*
TEST_SIM_ONLY(cps, UnpredictableBecauseOfBit1)
{
    code_gen().emit_ins16("10110110011i0000", 0);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(registers::PC, INITIAL_PC);
    step(&m_context);
}
*//*
TEST_SIM_ONLY(cps, UnpredictableBecauseOfBit2)
{
    code_gen().emit_ins16("10110110011i0110", 0);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(registers::PC, INITIAL_PC);
    step(&m_context);
}
*//*
TEST_SIM_ONLY(cps, UnpredictableBecauseOfBit3)
{
    code_gen().emit_ins16("10110110011i1010", 0);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(registers::PC, INITIAL_PC);
    step(&m_context);
}
*/
