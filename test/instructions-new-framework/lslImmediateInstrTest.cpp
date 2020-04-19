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
#define IMM_0  0
#define IMM_1  1
#define IMM_3  3
#define IMM_4  4
#define IMM_31 31


/* LSL - Immediate (Logical Shift Left)
   Encoding: 000 00 imm:5 Rm:3 Rd:3 */
TEST_F(CpuTestHarness, lslImmediate_MovR7toR0_CarryUnmodified)
{
	code_gen().emit_ins16("00000iiiiimmmddd", IMM_0, registers::R7, registers::R0);
	setExpectedXPSRflags("nzc");
	clearCarry();
	setExpectedRegisterValue(registers::R0, 0x77777777U);
	step();
}

TEST_F(CpuTestHarness, lslImmediate_MovR0toR7_ZeroResultAndCarryUnmodified)
{
	code_gen().emit_ins16("00000iiiiimmmddd", IMM_0, registers::R0, registers::R7);
	setExpectedXPSRflags("nZC");
	setCarry();
	setExpectedRegisterValue(registers::R7, 0x0);
	step();
}

TEST_F(CpuTestHarness, lslImmediate_ShiftR1by3_ResultInNegativeValue)
{
	code_gen().emit_ins16("00000iiiiimmmddd", IMM_3, registers::R1, registers::R0);
	setExpectedXPSRflags("Nzc");
	setExpectedRegisterValue(registers::R0, 0x11111111U << 3);
	step();
}

TEST_F(CpuTestHarness, lslImmediate_ShiftR1by4_HasCarryOut)
{
	code_gen().emit_ins16("00000iiiiimmmddd", IMM_4, registers::R1, registers::R0);
	setExpectedXPSRflags("nzC");
	setExpectedRegisterValue(registers::R0, 0x11111111U << 4);
	step();
}

TEST_F(CpuTestHarness, lslImmediate_ShiftR0by31_PushesLowestbitIntoSignBit)
{
	code_gen().emit_ins16("00000iiiiimmmddd", IMM_31, registers::R0, registers::R0);
	setExpectedXPSRflags("Nzc");
	setRegisterValue(registers::R0, 1U);
	setExpectedRegisterValue(registers::R0, 1U << 31);
	step();
}

TEST_F(CpuTestHarness, lslImmediate_CarryOutFromHighestBit)
{
	code_gen().emit_ins16("00000iiiiimmmddd", IMM_1, registers::R0, registers::R0);
	setExpectedXPSRflags("nzC");
	setRegisterValue(registers::R0, 0xA0000000U);
	setExpectedRegisterValue(registers::R0, 0xA0000000U << 1);
	step();
}

TEST_F(CpuTestHarness, lslImmediate_CarryOutFromLowestBit)
{
	code_gen().emit_ins16("00000iiiiimmmddd", IMM_31, registers::R0, registers::R0);
	setExpectedXPSRflags("nZC");
	setRegisterValue(registers::R0, 0x2U);
	setExpectedRegisterValue(registers::R0, 0x2U << 31);
	step();
}
