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


TEST_F(CpuTestHarness, undefined_Undedfined16BitWithAllZeroesForImmedaite)
{
	code_gen().emit16bit("11011110iiiiiiii", 0);
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	setExpectedExceptionTaken(CPU_STEP_HARDFAULT);
	step();
}

TEST_F(CpuTestHarness, undefined_Undedfined16BitWithAllOnesForImmedaite)
{
	code_gen().emit16bit("11011110iiiiiiii", -1);
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	setExpectedExceptionTaken(CPU_STEP_HARDFAULT);
	step();
}

TEST_F(CpuTestHarness, undefined_Undefined32BitWithAllZeroesForImmediate)
{
	code_gen().emit32bit("111101111111iiii", "1010iiiiiiiiiiii", 0, 0);
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	setExpectedExceptionTaken(CPU_STEP_HARDFAULT);
	step();
}

TEST_F(CpuTestHarness, undefined_Undefined32BitWithAllOnesForImmediate)
{
	code_gen().emit32bit("111101111111iiii", "1010iiiiiiiiiiii", -1, -1);
	setExpectedRegisterValue(registers::PC, INITIAL_PC + 4);
	setExpectedExceptionTaken(CPU_STEP_HARDFAULT);
	step();
}
