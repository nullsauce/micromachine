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


/* LDM
   Encoding: 1100 1 Rn:3 RegisterList:8 */
TEST_F(CpuTestHarness, ldm_JustPopR0WithR7AsAddress_WritebackNewAddressToR7)
{
	code_gen().emit_ins16("11001nnnrrrrrrrr", R7, (1 << 0));
	setRegisterValue(R7, INITIAL_PC + 16);
	setExpectedRegisterValue(R7, INITIAL_PC + 16 + 1 * 4);
	setExpectedRegisterValue(R0, 0xFFFFFFFF);
	memory_write_32(INITIAL_PC + 16, 0xFFFFFFFF);
	step();
}

TEST_F(CpuTestHarness, ldm_JustPopR7WithR0AsAddress_WritebackNewAddressToR0)
{
	code_gen().emit_ins16("11001nnnrrrrrrrr", R0, (1 << 7));
	setRegisterValue(R0, INITIAL_PC + 16);
	setExpectedRegisterValue(R0, INITIAL_PC + 16 + 1 * 4);
	setExpectedRegisterValue(R7, 0xFFFFFFFF);
	memory_write_32(INITIAL_PC + 16, 0xFFFFFFFF);
	step();
}

TEST_F(CpuTestHarness, ldm_PopAllNoWriteback)
{
	code_gen().emit_ins16("11001nnnrrrrrrrr", R0, 0xFF);
	setRegisterValue(R0, INITIAL_PC + 16);
	setExpectedRegisterValue(R0, 0);
	setExpectedRegisterValue(R1, 1);
	setExpectedRegisterValue(R2, 2);
	setExpectedRegisterValue(R3, 3);
	setExpectedRegisterValue(R4, 4);
	setExpectedRegisterValue(R5, 5);
	setExpectedRegisterValue(R6, 6);
	setExpectedRegisterValue(R7, 7);
	for (int i = 0; i < 8; i++)
		memory_write_32(INITIAL_PC + 16 + 4 * i, i);
	step();
}

TEST_F(CpuTestHarness, ldm_PopAllButAddressRegister_WritebackNewAddress)
{
	code_gen().emit_ins16("11001nnnrrrrrrrr", R7, 0x7F);
	setRegisterValue(R7, INITIAL_PC + 16);
	setExpectedRegisterValue(R0, 0);
	setExpectedRegisterValue(R1, 1);
	setExpectedRegisterValue(R2, 2);
	setExpectedRegisterValue(R3, 3);
	setExpectedRegisterValue(R4, 4);
	setExpectedRegisterValue(R5, 5);
	setExpectedRegisterValue(R6, 6);
	setExpectedRegisterValue(R7, INITIAL_PC + 16 + 7 * 4);
	for (int i = 0; i < 7; i++)
		memory_write_32(INITIAL_PC + 16 + 4 * i, i);
	step();
}

TEST_F(CpuTestHarness, ldm_HardFaultFromInvalidMemoryRead)
{
	code_gen().emit_ins16("11001nnnrrrrrrrr", 0, (1 << 0));
	setRegisterValue(R0, 0xFFFFFFFC);
	setExpectedExceptionTaken(CPU_STEP_HARDFAULT);
	step();
}
/*
TEST_SIM_ONLY(ldm, UnpredictableToPopNoRegisters)
{
    code_gen().emit_ins16("11001nnnrrrrrrrr", 0, 0);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    setExpectedRegisterValue(registers::PC, INITIAL_PC);
    step(&m_context);
}
*/
