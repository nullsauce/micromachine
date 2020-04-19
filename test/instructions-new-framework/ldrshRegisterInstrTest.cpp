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

#include "CpuTestFixture.hpp"


/* LDRSH - Register
   Encoding: 0101 111 Rm:3 Rn:3 Rt:3 */
MICROMACHINE_TEST_F(ldrshRegister, UseAMixOfRegistersWordAligned_NegativeValue, CpuTestFixture)
{
	code_gen().emit_ins16("0101111mmmnnnttt", registers::R7, registers::R3, registers::R0);
	getCpu().regs().set(registers::R3, INITIAL_PC);
	getCpu().regs().set(registers::R7, 4);
	getCpu().mem().write32(INITIAL_PC + 4, 0xBAADFEED);
	ExpectThat().Register(registers::R0).Equals(0xFFFFFEED);
	Step();
}

MICROMACHINE_TEST_F(ldrshRegister, UseAnotherMixOfRegistersWordAligned_NegativeValue, CpuTestFixture)
{
	code_gen().emit_ins16("0101111mmmnnnttt", registers::R1, registers::R0, registers::R7);
	getCpu().regs().set(registers::R0, INITIAL_PC);
	getCpu().regs().set(registers::R1, 4);
	getCpu().mem().write32(INITIAL_PC + 4, 0xBAADFEED);
	ExpectThat().Register(registers::R7).Equals(0xFFFFFEED);
	Step();
}

MICROMACHINE_TEST_F(ldrshRegister, YetAnotherMixOfRegistersNotWordAligned_NegativeValue, CpuTestFixture)
{
	code_gen().emit_ins16("0101111mmmnnnttt", registers::R0, registers::R7, registers::R4);
	getCpu().regs().set(registers::R7, INITIAL_PC);
	getCpu().regs().set(registers::R0, 6);
	getCpu().mem().write32(INITIAL_PC + 4, 0xBAADFEED);
	ExpectThat().Register(registers::R4).Equals(0xFFFFBAAD);
	Step();
}

MICROMACHINE_TEST_F(ldrshRegister, LoadPositiveHalfWord, CpuTestFixture)
{
	code_gen().emit_ins16("0101111mmmnnnttt", registers::R0, registers::R7, registers::R4);
	getCpu().regs().set(registers::R7, INITIAL_PC);
	getCpu().regs().set(registers::R0, 4);
	getCpu().mem().write32(INITIAL_PC + 4, 0xFFFF7FFF);
	ExpectThat().Register(registers::R4).Equals(0x7FFF);
	Step();
}

MICROMACHINE_TEST_F(ldrshRegister, AttemptUnalignedLoad, CpuTestFixture)
{
	code_gen().emit_ins16("0101111mmmnnnttt", registers::R7, registers::R3, registers::R0);
	getCpu().regs().set(registers::R3, INITIAL_PC);
	getCpu().regs().set(registers::R7, 1);
	ExpectThat().HardfaultHandlerReached();
	Step();
}

MICROMACHINE_TEST_F(ldrshRegister, AttemptLoadFromInvalidAddress, CpuTestFixture)
{
	code_gen().emit_ins16("0101111mmmnnnttt", registers::R7, registers::R3, registers::R0);
	getCpu().regs().set(registers::R3, 0xFFFFFFFC);
	getCpu().regs().set(registers::R7, 0);
	ExpectThat().HardfaultHandlerReached();
	Step();
}
