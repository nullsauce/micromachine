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


/* DMB
   Encoding: 11110 0 111 01 1 (1)(1)(1)(1)
             10 (0) 0 (1)(1)(1)(1) 0101 option:4 */
TEST_F(CpuTestHarness, dmb_OptionSetTo15)
{
	code_gen().emit_ins32("1111001110111111", "100011110101oooo", 15);
	setExpectedRegisterValue(PC, INITIAL_PC + 4);
	step();
}

TEST_F(CpuTestHarness, dmb_OptionSetTo0)
{
	code_gen().emit_ins32("1111001110111111", "100011110101oooo", 0);
	setExpectedRegisterValue(PC, INITIAL_PC + 4);
	step();
}
/*
TEST_SIM_ONLY(dmb, UnpredictableBecauseOfBit1_0)
{
    code_gen().emit_ins32("1111001110111110", "100011110101oooo", 15);
    setExpectedStepReturn(PINKYSIM_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(PC, INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(dmb, UnpredictableBecauseOfBit1_1)
{
    code_gen().emit_ins32("1111001110111101", "100011110101oooo", 15);
    setExpectedStepReturn(PINKYSIM_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(PC, INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(dmb, UnpredictableBecauseOfBit1_2)
{
    code_gen().emit_ins32("1111001110111011", "100011110101oooo", 15);
    setExpectedStepReturn(PINKYSIM_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(PC, INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(dmb, UnpredictableBecauseOfBit1_3)
{
    code_gen().emit_ins32("1111001110110111", "100011110101oooo", 15);
    setExpectedStepReturn(PINKYSIM_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(PC, INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(dmb, UnpredictableBecauseOfBit2_8)
{
    code_gen().emit_ins32("1111001110111111", "100011100101oooo", 15);
    setExpectedStepReturn(PINKYSIM_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(PC, INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(dmb, UnpredictableBecauseOfBit2_9)
{
    code_gen().emit_ins32("1111001110111111", "100011010101oooo", 15);
    setExpectedStepReturn(PINKYSIM_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(PC, INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(dmb, UnpredictableBecauseOfBit2_10)
{
    code_gen().emit_ins32("1111001110111111", "100010110101oooo", 15);
    setExpectedStepReturn(PINKYSIM_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(PC, INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(dmb, UnpredictableBecauseOfBit2_11)
{
    code_gen().emit_ins32("1111001110111111", "100001110101oooo", 15);
    setExpectedStepReturn(PINKYSIM_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(PC, INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(dmb, UnpredictableBecauseOfBit2_13)
{
    code_gen().emit_ins32("1111001110111111", "101011110101oooo", 15);
    setExpectedStepReturn(PINKYSIM_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(PC, INITIAL_PC);
    pinkySimStep(&m_context);
}
*/