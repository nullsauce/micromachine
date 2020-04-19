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


// Immediate values used for shift amount in tests.
#define IMM_1  1
#define IMM_32 0


/* ASR - Immediate (Arithmetic Shift Right)
   Encoding: 000 10 imm:5 Rm:3 Rd:3 */
TEST_F(CpuTestHarness, asrImmediate_ShiftNegativeNumberBy1_Shift0OutFromLowestBit)
{
	code_gen().emit_ins16("00010iiiiimmmddd", IMM_1, registers::R0, registers::R7);
	setRegisterValue(registers::R0, 0x80000000U);
	setExpectedXPSRflags("Nzc");
	setExpectedRegisterValue(registers::R7, (int32_t) 0x80000000U >> 1);
	step();
}

TEST_F(CpuTestHarness, asrImmediate_ShiftPositiveNumberBy1_Shift1OutFromLowestBit)
{
	code_gen().emit_ins16("00010iiiiimmmddd", IMM_1, registers::R7, registers::R0);
	setRegisterValue(registers::R7, 0x7FFFFFFFU);
	setExpectedXPSRflags("nzC");
	setExpectedRegisterValue(registers::R0, (int32_t) 0x7FFFFFFFU >> 1);
	step();
}

TEST_F(CpuTestHarness, asrImmediate_NegativeNumberBy32_Shift1OutFromHighestBit)
{
	code_gen().emit_ins16("00010iiiiimmmddd", IMM_32, registers::R0, registers::R0);
	setRegisterValue(registers::R0, 0x80000000U);
	setExpectedXPSRflags("NzC");
	setExpectedRegisterValue(registers::R0, 0xFFFFFFFFU);
	step();
}

TEST_F(CpuTestHarness, asrImmediate_PositiveNumberBy32_Shift0FromHighestBit)
{
	code_gen().emit_ins16("00010iiiiimmmddd", IMM_32, registers::R1, registers::R6);
	setRegisterValue(registers::R1, 0x7FFFFFFFU);
	setExpectedXPSRflags("nZc");
	setExpectedRegisterValue(registers::R6, 0x0U);
	step();
}

TEST_F(CpuTestHarness, asrImmediate_registers::R1by1ToR7)
{
	code_gen().emit_ins16("00010iiiiimmmddd", IMM_1, registers::R1, registers::R7);
	setExpectedXPSRflags("nzC");
	setExpectedRegisterValue(registers::R7, (int32_t) 0x11111111U >> 1);
	step();
}

TEST_F(CpuTestHarness, asrImmediate_registers::R7by1ToR2)
{
	code_gen().emit_ins16("00010iiiiimmmddd", IMM_1, registers::R7, registers::R2);
	setExpectedXPSRflags("nzC");
	setExpectedRegisterValue(registers::R2, (int32_t) 0x77777777U >> 1);
	step();
}

TEST_F(CpuTestHarness, asrImmediate_registers::R0by1)
{
	code_gen().emit_ins16("00010iiiiimmmddd", IMM_1, registers::R0, registers::R0);
	setExpectedXPSRflags("nZc");
	setExpectedRegisterValue(registers::R0, (int32_t) 0x00000000U >> 1);
	step();
}
