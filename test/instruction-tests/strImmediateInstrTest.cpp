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


/* STR - Immediate Encoding T1
   Encoding: 011 0 0 Imm:5 Rn:3 Rt:3 */
MICROMACHINE_TEST_F(strImmediate, T1UseAMixOfRegistersWithSmallestImmediateOffset, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("01100iiiiinnnttt", 0, registers::R7, registers::R0);
	getCpu().regs().set(registers::R7, INITIAL_PC + 4);
	getCpu().mem().write32(INITIAL_PC + 4, 0xBAADFEED);
	Step();
	EXPECT_EQ(0x00000000, getCpu().mem().read32(INITIAL_PC + 4));
}

MICROMACHINE_TEST_F(strImmediate, T1UseAnotherMixOfRegistersWithLargestImmediateOffset, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	getCpu().regs().set(registers::R7, 0x77777777U);
	code_gen().emit_ins16("01100iiiiinnnttt", 31, registers::R0, registers::R7);
	getCpu().regs().set(registers::R0, INITIAL_PC);
	getCpu().mem().write32(INITIAL_PC + 31 * 4, 0xBAADFEED);
	Step();
	EXPECT_EQ(0x77777777, getCpu().mem().read32(INITIAL_PC + 31 * 4));
}

MICROMACHINE_TEST_F(strImmediate, T1AttemptUnalignedStore, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	getCpu().regs().set(registers::R2, 0x22222222U);
	code_gen().emit_ins16("01100iiiiinnnttt", 0, registers::R3, registers::R2);
	getCpu().regs().set(registers::R3, INITIAL_PC + 2);
	Step();
	ExpectThat().HardfaultHandlerReached();
}

MICROMACHINE_TEST_F(strImmediate, T1AttemptStoreToInvalidAddress, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	getCpu().regs().set(registers::R2, 0x22222222U);
	code_gen().emit_ins16("01100iiiiinnnttt", 16, registers::R3, registers::R2);
	getCpu().regs().set(registers::R3, 0xFFFFFFFC - 16 * 4);
	Step();
	ExpectThat().HardfaultHandlerReached();
}



/* STR - Immediate Encoding T2 (registers::SP is base register)
   Encoding: 1001 0 Rt:3 Imm:8 */
MICROMACHINE_TEST_F(strImmediate, T2HighestRegisterWithSmallestImmediateOffset, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	getCpu().regs().set(registers::R7, 0x77777777U);
	code_gen().emit_ins16("10010tttiiiiiiii", registers::R7, 0);
	getCpu().regs().set(registers::SP, INITIAL_PC + 1024);
	getCpu().mem().write32(INITIAL_PC + 1024, 0xBAADFEED);
	Step();
	EXPECT_EQ(0x77777777, getCpu().mem().read32(INITIAL_PC + 1024));
}

MICROMACHINE_TEST_F(strImmediate, T2LowestRegisterWithLargestImmediateOffset, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("10010tttiiiiiiii", registers::R0, 255);
	getCpu().regs().set(registers::SP, INITIAL_PC + 1024);
	getCpu().mem().write32(INITIAL_PC + 1024 + 255 * 4, 0xBAADFEED);
	Step();
	EXPECT_EQ(0x00000000, getCpu().mem().read32(INITIAL_PC + 1024 + 255 * 4));
}

/*
 * This test is disabled because the current software architecture prevents sp
 * from holding an unaligned address. However, the doc clearly states that
 * unaligned STR instructions should raise a hard fault.
 *
 * TODO: Remove the pedantic forced alignment of the value returned by get_sp()
 * and allow the error to happen. This might have side effects on other parts
 * of the code that wrongly expect that sp wont hold an unaligned value.
 *
MICROMACHINE_TEST_F(strImmediate, T2AttemptUnalignedStore, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().set_pc(INITIAL_PC);
	getCpu().regs().set(registers::R2, 0x22222222U);
	code_gen().emit_ins16("10010tttiiiiiiii", registers::R2, 0);
	getCpu().regs().set(registers::SP, INITIAL_PC + 1026);
	getCpu().mem().write32(INITIAL_PC + 1024, 0xBAADFEED);
	Step();
	ExpectThat().HardfaultHandlerReached();
}*/
/*
TEST_SIM_ONLY(strImmediate, T2AttemptStoreToInvalidAddress)
{
    code_gen().emit_ins16("10010tttiiiiiiii", registers::R2, 0);
    getCpu().regs().set(registers::SP, 0xFFFFFFFC);
    setExpectedExceptionHandled(CPU_STEP_HARDFAULT);
    step(&m_context);
}
*/
