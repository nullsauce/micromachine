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


/* LDRB - Immediate
   Encoding: 011 1 1 Imm:5 Rn:3 Rt:3 */
TEST_F(CpuTestHarness, ldrbImmediate_UseAMixOfRegistersWordAligned)
{
	code_gen().emit_ins16("01111iiiiinnnttt", 0, registers::R7, registers::R0);
	setRegisterValue(registers::R7, INITIAL_PC + 4);
	memory_write_32(INITIAL_PC + 4, 0xBAADFEED);
	setExpectedRegisterValue(registers::R0, 0xED);
	step();
}

TEST_F(CpuTestHarness, ldrbImmediate_UseAnotherMixOfRegistersSecondByteInWord)
{
	code_gen().emit_ins16("01111iiiiinnnttt", 1, registers::R0, registers::R7);
	setRegisterValue(registers::R0, INITIAL_PC + 4);
	memory_write_32(INITIAL_PC + 4, 0xBAADFEED);
	setExpectedRegisterValue(registers::R7, 0xFE);
	step();
}

TEST_F(CpuTestHarness, ldrbImmediate_YetAnotherMixOfRegistersThirdByteInWord)
{
	code_gen().emit_ins16("01111iiiiinnnttt", 2, registers::R1, registers::R4);
	setRegisterValue(registers::R1, INITIAL_PC + 4);
	memory_write_32(INITIAL_PC + 4, 0xBAADFEED);
	setExpectedRegisterValue(registers::R4, 0xAD);
	step();
}

TEST_F(CpuTestHarness, ldrbImmediate_YetAnotherMixOfRegistersFourthByteInWord)
{
	code_gen().emit_ins16("01111iiiiinnnttt", 3, registers::R2, registers::R5);
	setRegisterValue(registers::R2, INITIAL_PC + 4);
	memory_write_32(INITIAL_PC + 4, 0xBAADFEED);
	setExpectedRegisterValue(registers::R5, 0xBA);
	step();
}

TEST_F(CpuTestHarness, ldrbImmediate_UseLargestOffset)
{
	code_gen().emit_ins16("01111iiiiinnnttt", 31, registers::R3, registers::R0);
	setRegisterValue(registers::R3, INITIAL_PC);
	memory_write_32(INITIAL_PC + 28, 0x12345678);
	setExpectedRegisterValue(registers::R0, 0x12);
	step();
}

TEST_F(CpuTestHarness, ldrbImmediate_LoadAPositiveValue)
{
	code_gen().emit_ins16("01111iiiiinnnttt", 0, registers::R3, registers::R0);
	setRegisterValue(registers::R3, INITIAL_PC + 4);
	memory_write_32(INITIAL_PC + 4, 0xFFFFFF7F);
	setExpectedRegisterValue(registers::R0, 0x7F);
	step();
}

TEST_F(CpuTestHarness, ldrbImmediate_AttemptLoadInvalidAddress)
{
	code_gen().emit_ins16("01111iiiiinnnttt", 0, registers::R3, registers::R0);
	setRegisterValue(registers::R3, 0xFFFFFFFC);
	setExpectedExceptionTaken(CPU_STEP_HARDFAULT);
	step();
}
