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


/* LDRH - Immediate
   Encoding: 1000 1 Imm:5 Rn:3 Rt:3 */
TEST_F(CpuTestHarness, ldrhImmediate_UseAMixOfRegistersWordAligned)
{
	code_gen().emit_ins16("10001iiiiinnnttt", 0, registers::R7, registers::R0);
	setRegisterValue(registers::R7, INITIAL_PC + 4);
	memory_write_32(INITIAL_PC + 4, 0xBAADFEED);
	setExpectedRegisterValue(registers::R0, 0xFEED);
	step();
}

TEST_F(CpuTestHarness, ldrhImmediate_UseAnotherMixOfRegistersNotWordAligned)
{
	code_gen().emit_ins16("10001iiiiinnnttt", 1, registers::R0, registers::R7);
	setRegisterValue(registers::R0, INITIAL_PC + 4);
	memory_write_32(INITIAL_PC + 4, 0xBAADFEED);
	setExpectedRegisterValue(registers::R7, 0xBAAD);
	step();
}

TEST_F(CpuTestHarness, ldrhImmediate_LargestOffset)
{
	code_gen().emit_ins16("10001iiiiinnnttt", 31, registers::R1, registers::R6);
	setRegisterValue(registers::R1, INITIAL_PC);
	memory_write_32(INITIAL_PC + 60, 0xBAADFEED);
	setExpectedRegisterValue(registers::R6, 0xBAAD);
	step();
}

TEST_F(CpuTestHarness, ldrhImmediate_AttemptLoadFromInvalidAddress)
{
	code_gen().emit_ins16("10001iiiiinnnttt", 0, registers::R3, registers::R0);
	setRegisterValue(registers::R3, 0xFFFFFFFC);
	setExpectedExceptionTaken(CPU_STEP_HARDFAULT);
	step();
}
