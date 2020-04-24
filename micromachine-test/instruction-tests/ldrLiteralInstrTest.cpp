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
using namespace micromachine::testing;

/* LDR - Literal
   Encoding: 01001 Rt:3 Imm:8 */
MICROMACHINE_TEST_F(ldrLiteral, LoadOffset0IntoHighestRegister, CpuTestFixture) {

	emitInstruction16("01001tttiiiiiiii", registers::R7, 0);
	memWrite32(TestMachine::INITIAL_PC + 4, 0xBAADFEED);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R7).equals(0xBAADFEED));
}

MICROMACHINE_TEST_F(ldrLiteral, LoadOffset0IntoHighestRegisterNot4ByteAligned, CpuTestFixture) {

	// Emit UNDEFINED 16-bit instruction.
	emitInstruction16("1101111000000000");
	// Emit actual test instruction at a 2-byte aligned address which isn't 4-byte aligned.
	emitInstruction16("01001tttiiiiiiii", registers::R7, 0);
	memWrite32(TestMachine::INITIAL_PC + 4, 0xBAADFEED);
	// Move PC to point to second instruction.
	setReg(registers::PC, getReg(registers::PC) + 2);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R7).equals(0xBAADFEED));
}

MICROMACHINE_TEST_F(ldrLiteral, LoadMaximumOffsetIntoLowestRegister, CpuTestFixture) {

	emitInstruction16("01001tttiiiiiiii", registers::R0, 255);
	memWrite32(TestMachine::INITIAL_PC + 4 + 255 * 4, 0xBAADFEED);
	StepAndExpectThatInstruction16IsExecutedAndThat(reg(registers::R0).equals(0xBAADFEED));
}

MICROMACHINE_TEST_F(ldrLiteral, AttemptToLoadFromInvalidAddress, CpuTestFixture) {
	// load a pc-relative address at end of memory - 128, with an offset of 255
	// this should overflow memory by 127 and hard fault
	setReg(registers::PC, TestMachine::MEMORY_SIZE - 128);
	setNextEmitInstructionAddress(TestMachine::MEMORY_SIZE - 128);
	emitInstruction16("01001tttiiiiiiii", registers::R0, 255);

	StepAndExpectThat(hardfaultHandlerReached());
}
