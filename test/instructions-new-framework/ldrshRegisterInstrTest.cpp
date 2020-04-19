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


/* LDRSH - Register
   Encoding: 0101 111 Rm:3 Rn:3 Rt:3 */
TEST_F(CpuTestHarness, ldrshRegister_UseAMixOfRegistersWordAligned_NegativeValue)
{
	code_gen().emit_ins16("0101111mmmnnnttt", registers::R7, registers::R3, registers::R0);
	setRegisterValue(registers::R3, INITIAL_PC);
	setRegisterValue(registers::R7, 4);
	memory_write_32(INITIAL_PC + 4, 0xBAADFEED);
	setExpectedRegisterValue(registers::R0, 0xFFFFFEED);
	step();
}

TEST_F(CpuTestHarness, ldrshRegister_UseAnotherMixOfRegistersWordAligned_NegativeValue)
{
	code_gen().emit_ins16("0101111mmmnnnttt", registers::R1, registers::R0, registers::R7);
	setRegisterValue(registers::R0, INITIAL_PC);
	setRegisterValue(registers::R1, 4);
	memory_write_32(INITIAL_PC + 4, 0xBAADFEED);
	setExpectedRegisterValue(registers::R7, 0xFFFFFEED);
	step();
}

TEST_F(CpuTestHarness, ldrshRegister_YetAnotherMixOfRegistersNotWordAligned_NegativeValue)
{
	code_gen().emit_ins16("0101111mmmnnnttt", registers::R0, registers::R7, registers::R4);
	setRegisterValue(registers::R7, INITIAL_PC);
	setRegisterValue(registers::R0, 6);
	memory_write_32(INITIAL_PC + 4, 0xBAADFEED);
	setExpectedRegisterValue(registers::R4, 0xFFFFBAAD);
	step();
}

TEST_F(CpuTestHarness, ldrshRegister_LoadPositiveHalfWord)
{
	code_gen().emit_ins16("0101111mmmnnnttt", registers::R0, registers::R7, registers::R4);
	setRegisterValue(registers::R7, INITIAL_PC);
	setRegisterValue(registers::R0, 4);
	memory_write_32(INITIAL_PC + 4, 0xFFFF7FFF);
	setExpectedRegisterValue(registers::R4, 0x7FFF);
	step();
}

TEST_F(CpuTestHarness, ldrshRegister_AttemptUnalignedLoad)
{
	code_gen().emit_ins16("0101111mmmnnnttt", registers::R7, registers::R3, registers::R0);
	setRegisterValue(registers::R3, INITIAL_PC);
	setRegisterValue(registers::R7, 1);
	setExpectedExceptionTaken(CPU_STEP_HARDFAULT);
	step();
}

TEST_F(CpuTestHarness, ldrshRegister_AttemptLoadFromInvalidAddress)
{
	code_gen().emit_ins16("0101111mmmnnnttt", registers::R7, registers::R3, registers::R0);
	setRegisterValue(registers::R3, 0xFFFFFFFC);
	setRegisterValue(registers::R7, 0);
	setExpectedExceptionTaken(CPU_STEP_HARDFAULT);
	step();
}