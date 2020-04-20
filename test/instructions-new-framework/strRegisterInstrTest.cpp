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


/* STR - Register
   Encoding: 0101 000 Rm:3 Rn:3 Rt:3 */
MICROMACHINE_TEST_F(strRegister, UseAMixOfRegisters, CpuTestFixture) {
	code_gen().emit_ins16("0101000mmmnnnttt", registers::R7, registers::R3, registers::R0);
	getCpu().regs().set(registers::R3, INITIAL_PC);
	getCpu().regs().set(registers::R7, 4);
	getCpu().mem().write32(INITIAL_PC + 4, 0xBAADFEED);
	Step();
	EXPECT_EQ(0x00000000, getCpu().mem().read32(INITIAL_PC + 4));
}

MICROMACHINE_TEST_F(strRegister, UseAnotherMixOfRegisters, CpuTestFixture) {
	code_gen().emit_ins16("0101000mmmnnnttt", registers::R1, registers::R0, registers::R7);
	getCpu().regs().set(registers::R0, INITIAL_PC);
	getCpu().regs().set(registers::R1, 4);
	getCpu().mem().write32(INITIAL_PC + 4, 0xBAADFEED);
	Step();
	EXPECT_EQ(0x77777777, getCpu().mem().read32(INITIAL_PC + 4));
}

MICROMACHINE_TEST_F(strRegister, YetAnotherMixOfRegisters, CpuTestFixture) {
	code_gen().emit_ins16("0101000mmmnnnttt", registers::R0, registers::R7, registers::R4);
	getCpu().regs().set(registers::R7, INITIAL_PC);
	getCpu().regs().set(registers::R0, 4);
	getCpu().mem().write32(INITIAL_PC + 4, 0xBAADFEED);
	Step();
	EXPECT_EQ(0x44444444, getCpu().mem().read32(INITIAL_PC + 4));
}

MICROMACHINE_TEST_F(strRegister, AttemptUnalignedStore, CpuTestFixture) {
	code_gen().emit_ins16("0101000mmmnnnttt", registers::R7, registers::R3, registers::R0);
	getCpu().regs().set(registers::R3, INITIAL_PC);
	getCpu().regs().set(registers::R7, 2);
	ExpectThat().HardfaultHandlerReached();
	Step();
}

MICROMACHINE_TEST_F(strRegister, AttemptStoreToInvalidAddress, CpuTestFixture) {
	code_gen().emit_ins16("0101000mmmnnnttt", registers::R7, registers::R3, registers::R0);
	getCpu().regs().set(registers::R3, 0xFFFFFFFC);
	getCpu().regs().set(registers::R7, 0);
	ExpectThat().HardfaultHandlerReached();
	Step();
}
