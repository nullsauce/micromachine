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


/* LDRSB - Register
   Encoding: 0101 011 Rm:3 Rn:3 Rt:3 */
TEST_F(CpuTestHarness, ldrsbRegister_UseAMixOfRegistersWordAligned_NegativeValue)
{
	code_gen().emit_ins16("0101011mmmnnnttt", registers::R7, registers::R3, registers::R0);
	setRegisterValue(registers::R3, INITIAL_PC);
	setRegisterValue(registers::R7, 4);
	memory_write_32(INITIAL_PC + 4, 0xBAADFEED);
	setExpectedRegisterValue(registers::R0, 0xFFFFFFED);
	step();
}

TEST_F(CpuTestHarness, ldrsbRegister_UseAnotherMixOfRegistersSecondByteInWord_NegativeValue)
{
	code_gen().emit_ins16("0101011mmmnnnttt", registers::R1, registers::R0, registers::R7);
	setRegisterValue(registers::R0, INITIAL_PC);
	setRegisterValue(registers::R1, 5);
	memory_write_32(INITIAL_PC + 4, 0xBAADFEED);
	setExpectedRegisterValue(registers::R7, 0xFFFFFFFE);
	step();
}

TEST_F(CpuTestHarness, ldrsbRegister_YetAnotherMixOfRegistersThirdByteInWord_NegativeValue)
{
	code_gen().emit_ins16("0101011mmmnnnttt", registers::R0, registers::R7, registers::R4);
	setRegisterValue(registers::R7, INITIAL_PC);
	setRegisterValue(registers::R0, 6);
	memory_write_32(INITIAL_PC + 4, 0xBAADFEED);
	setExpectedRegisterValue(registers::R4, 0xFFFFFFAD);
	step();
}

TEST_F(CpuTestHarness, ldrsbRegister_YetAnotherMixOfRegistersFourthByteInWord_NegativeValue)
{
	code_gen().emit_ins16("0101011mmmnnnttt", registers::R0, registers::R7, registers::R5);
	setRegisterValue(registers::R7, INITIAL_PC);
	setRegisterValue(registers::R0, 7);
	memory_write_32(INITIAL_PC + 4, 0xBAADFEED);
	setExpectedRegisterValue(registers::R5, 0xFFFFFFBA);
	step();
}

TEST_F(CpuTestHarness, ldrsbRegister_LoadAPositiveValue)
{
	code_gen().emit_ins16("0101011mmmnnnttt", registers::R7, registers::R3, registers::R0);
	setRegisterValue(registers::R3, INITIAL_PC);
	setRegisterValue(registers::R7, 4);
	memory_write_32(INITIAL_PC + 4, 0xFFFFFF7F);
	setExpectedRegisterValue(registers::R0, 0x7F);
	step();
}

TEST_F(CpuTestHarness, ldrsbRegister_AttemptLoadInvalidAddress)
{
	code_gen().emit_ins16("0101011mmmnnnttt", registers::R7, registers::R3, registers::R0);
	setRegisterValue(registers::R3, 0xFFFFFFFC);
	setRegisterValue(registers::R7, 0);
	setExpectedExceptionTaken(CPU_STEP_HARDFAULT);
	step();
}
