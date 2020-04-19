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


/* LDR - Immediate Encoding T1
   Encoding: 011 0 1 Imm:5 Rn:3 Rt:3 */
TEST_F(CpuTestHarness, ldrImmediate_T1UseAMixOfRegistersWithSmallestOffset)
{
	code_gen().emit_ins16("01101iiiiinnnttt", 0, registers::R7, registers::R0);
	setRegisterValue(registers::R7, INITIAL_PC + 4);
	memory_write_32(INITIAL_PC + 4, 0xBAADFEED);
	setExpectedRegisterValue(registers::R0, 0xBAADFEED);
	step();
}

TEST_F(CpuTestHarness, ldrImmediate_T1UseAnotherMixOfRegistersWithLargestOffset)
{
	code_gen().emit_ins16("01101iiiiinnnttt", 31, registers::R0, registers::R7);
	setRegisterValue(registers::R0, INITIAL_PC);
	memory_write_32(INITIAL_PC + 31 * 4, 0xBAADFEED);
	setExpectedRegisterValue(registers::R7, 0xBAADFEED);
	step();
}

TEST_F(CpuTestHarness, ldrImmediate_T1AttemptUnalignedLoad)
{
	code_gen().emit_ins16("01101iiiiinnnttt", 0, registers::R3, registers::R2);
	setRegisterValue(registers::R3, INITIAL_PC + 2);
	setExpectedExceptionTaken(CPU_STEP_HARDFAULT);
	step();
}

TEST_F(CpuTestHarness, ldrImmediate_T1AttemptLoadFromInvalidAddress)
{
	code_gen().emit_ins16("01101iiiiinnnttt", 16, registers::R3, registers::R2);
	setRegisterValue(registers::R3, 0xFFFFFFFC - 16 * 4);
	setExpectedExceptionTaken(CPU_STEP_HARDFAULT);
	step();
}

/* LDR - Immediate Encoding T2 (SP is base register)
   Encoding: 1001 1 Rt:3 Imm:8 */
TEST_F(CpuTestHarness, ldrImmediate_T2UseHighestRegisterWithSmallestOffset)
{
	code_gen().emit_ins16("10011tttiiiiiiii", registers::R7, 0);
	setRegisterValue(registers::SP, INITIAL_PC + 1024);
	memory_write_32(INITIAL_PC + 1024, 0xBAADFEED);
	setExpectedRegisterValue(registers::R7, 0xBAADFEED);
	step();
}

TEST_F(CpuTestHarness, ldrImmediate_T2UseLowestRegisterWithLargestOffset)
{
	code_gen().emit_ins16("10011tttiiiiiiii", registers::R0, 255);
	setRegisterValue(registers::SP, INITIAL_PC + 1024);
	memory_write_32(INITIAL_PC + 1024 + 255 * 4, 0xBAADFEED);
	setExpectedRegisterValue(registers::R0, 0xBAADFEED);
	step();
}
/*
TEST_SIM_ONLY(ldrImmediate, T2AttemptUnalignedLoad)
{
    code_gen().emit_ins16("10011tttiiiiiiii", registers::R2, 0);
    setRegisterValue(registers::SP, INITIAL_PC + 1026);
    setExpectedExceptionHandled(CPU_STEP_HARDFAULT);
    setExpectedRegisterValue(registers::PC, INITIAL_PC);
    memory_write_32(m_context.pMemory, INITIAL_PC + 1024, 0xBAADFEED, READ_ONLY);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(ldrImmediate, T2AttemptLoadFromInvalidAddress)
{
    code_gen().emit_ins16("10011tttiiiiiiii", registers::R2, 0);
    setRegisterValue(registers::R3, 0xFFFFFFFC);
    setExpectedExceptionHandled(CPU_STEP_HARDFAULT);
    step(&m_context);
}
*/
