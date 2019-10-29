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

#include "framework/CpuTestHarness.hpp"


/* LDR - Literal
   Encoding: 01001 Rt:3 Imm:8 */
TEST_F(CpuTestHarness, ldrLiteral_LoadOffset0IntoHighestRegister)
{
	emitInstruction16("01001tttiiiiiiii", R7, 0);
	memory_write_32(INITIAL_PC + 4, 0xBAADFEED);
	setExpectedRegisterValue(R7, 0xBAADFEED);
	step();
}

TEST_F(CpuTestHarness, ldrLiteral_LoadOffset0IntoHighestRegisterNot4ByteAligned)
{
	// Emit UNDEFINED 16-bit instruction.
	emitInstruction16("1101111000000000");
	// Emit actual test instruction at a 2-byte aligned address which isn't 4-byte aligned.
	emitInstruction16("01001tttiiiiiiii", R7, 0);
	memory_write_32(INITIAL_PC + 4, 0xBAADFEED);
	// Move PC to point to second instruction.
	setRegisterValue(PC, _cpu.regs().get_pc() + 2);
	setExpectedRegisterValue(R7, 0xBAADFEED);
	step();
}

TEST_F(CpuTestHarness, ldrLiteral_LoadMaximumOffsetIntoLowestRegister)
{
	emitInstruction16("01001tttiiiiiiii", R0, 255);
	memory_write_32(INITIAL_PC + 4 + 255 * 4, 0xBAADFEED);
	setExpectedRegisterValue(R0, 0xBAADFEED);
	step();
}

TEST_F(CpuTestHarness, ldrLiteral_AttemptToLoadFromInvalidAddress)
{
	m_emitAddress = INITIAL_SP - 128;
	setRegisterValue(PC, INITIAL_SP - 128);
	setExpectedRegisterValue(PC, INITIAL_SP - 128);
	memory_write_32(INITIAL_SP - 128, 0);
	emitInstruction16("01001tttiiiiiiii", R0, 255);
	setExpectedExceptionTaken(PINKYSIM_STEP_HARDFAULT);
	step();
}