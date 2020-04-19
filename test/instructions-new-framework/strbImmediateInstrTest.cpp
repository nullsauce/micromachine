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

/* STRB - Immediate
   Encoding: 011 1 0 Imm:5 Rn:3 Rt:3 */
TEST_F(CpuTestHarness, strbImmediate_UseAMixOfRegistersWordAlignedWithSmallestOffset)
{
	code_gen().emit_ins16("01110iiiiinnnttt", 0, registers::R7, registers::R0);
	setRegisterValue(registers::R7, INITIAL_PC + 4);
	memory_write_32(INITIAL_PC + 4, 0xBAADFEED);
	step();
	EXPECT_EQ(0xBAADFE00, memory_read_32(INITIAL_PC + 4));
}

TEST_F(CpuTestHarness, strbImmediate_UseAnotherMixOfRegistersSecondByteInWord)
{
	code_gen().emit_ins16("01110iiiiinnnttt", 1, registers::R0, registers::R7);
	setRegisterValue(registers::R0, INITIAL_PC + 4);
	memory_write_32(INITIAL_PC + 4, 0xBAADFEED);
	step();
	EXPECT_EQ(0xBAAD77ED, memory_read_32(INITIAL_PC + 4));
}

TEST_F(CpuTestHarness, strbImmediate_YetAnotherMixOfRegistersThirdByteInWord)
{
	code_gen().emit_ins16("01110iiiiinnnttt", 2, registers::R3, registers::R4);
	setRegisterValue(registers::R3, INITIAL_PC + 4);
	memory_write_32(INITIAL_PC + 4, 0xBAADFEED);
	step();
	EXPECT_EQ(0xBA44FEED, memory_read_32(INITIAL_PC + 4));
}

TEST_F(CpuTestHarness, strbImmediate_YetAnotherMixOfRegistersFourthByteInWord)
{
	code_gen().emit_ins16("01110iiiiinnnttt", 3, registers::R1, registers::R5);
	setRegisterValue(registers::R1, INITIAL_PC + 4);
	memory_write_32(INITIAL_PC + 4, 0xBAADFEED);
	step();
	EXPECT_EQ(0x55ADFEED, memory_read_32(INITIAL_PC + 4));
}

TEST_F(CpuTestHarness, strbImmediate_LargestOffset)
{
	code_gen().emit_ins16("01110iiiiinnnttt", 31, registers::R2, registers::R4);
	setRegisterValue(registers::R2, INITIAL_PC);
	memory_write_32(INITIAL_PC + 28, 0xBAADFEED);
	step();
	EXPECT_EQ(0x44ADFEED, memory_read_32(INITIAL_PC + 28));
}

TEST_F(CpuTestHarness, strbImmediate_AttemptStoreToInvalidAddress)
{
	code_gen().emit_ins16("01110iiiiinnnttt", 0, registers::R3, registers::R0);
	setRegisterValue(registers::R3, 0xFFFFFFFC);
	setExpectedExceptionTaken(CPU_STEP_HARDFAULT);
	step();
}