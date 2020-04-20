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


// Immediate values used for shift amount in tests.
#define IMM_1  1
#define IMM_32 0


/* ASR - Immediate (Arithmetic Shift Right)
   Encoding: 000 10 imm:5 Rm:3 Rd:3 */
MICROMACHINE_TEST_F(asrImmediate, ShiftNegativeNumberBy1_Shift0OutFromLowestBit, CpuTestFixture) {
	getCpu().regs().set(registers::R7, 0x77777777);
	code_gen().emit_ins16("00010iiiiimmmddd", IMM_1, registers::R0, registers::R7);
	getCpu().regs().set(registers::R0, 0x80000000U);
	Step();
	ExpectThat().APSRFlagsMatches("Nzc");
	ExpectThat().Register(registers::R7).Equals((int32_t)0x80000000U >> 1);
}

MICROMACHINE_TEST_F(asrImmediate, ShiftPositiveNumberBy1_Shift1OutFromLowestBit, CpuTestFixture) {
	getCpu().regs().set(registers::R7, 0x77777777);

	code_gen().emit_ins16("00010iiiiimmmddd", IMM_1, registers::R7, registers::R0);
	getCpu().regs().set(registers::R7, 0x7FFFFFFFU);
	Step();
	ExpectThat().APSRFlagsMatches("nzC");
	ExpectThat().Register(registers::R0).Equals((int32_t)0x7FFFFFFFU >> 1);
}

MICROMACHINE_TEST_F(asrImmediate, NegativeNumberBy32_Shift1OutFromHighestBit, CpuTestFixture) {
	code_gen().emit_ins16("00010iiiiimmmddd", IMM_32, registers::R0, registers::R0);
	getCpu().regs().set(registers::R0, 0x80000000U);
	Step();
	ExpectThat().APSRFlagsMatches("NzC");
	ExpectThat().Register(registers::R0).Equals(0xFFFFFFFFU);
}

MICROMACHINE_TEST_F(asrImmediate, PositiveNumberBy32_Shift0FromHighestBit, CpuTestFixture) {
	getCpu().regs().set(registers::R6, 0x66666666);
	getCpu().regs().set(registers::R1, 0x11111111);
	code_gen().emit_ins16("00010iiiiimmmddd", IMM_32, registers::R1, registers::R6);
	getCpu().regs().set(registers::R1, 0x7FFFFFFFU);
	Step();
	ExpectThat().APSRFlagsMatches("nZc");
	ExpectThat().Register(registers::R6).Equals(0x0U);
}

MICROMACHINE_TEST_F(asrImmediate, R1by1ToR7, CpuTestFixture) {
	getCpu().regs().set(registers::R7, 0x77777777);
	getCpu().regs().set(registers::R1, 0x11111111);
	code_gen().emit_ins16("00010iiiiimmmddd", IMM_1, registers::R1, registers::R7);
	Step();
	ExpectThat().APSRFlagsMatches("nzC");
	ExpectThat().Register(registers::R7).Equals((int32_t)0x11111111U >> 1);
}

MICROMACHINE_TEST_F(asrImmediate, R7by1ToR2, CpuTestFixture) {
	getCpu().regs().set(registers::R7, 0x77777777);
	getCpu().regs().set(registers::R2, 0x22222222);
	code_gen().emit_ins16("00010iiiiimmmddd", IMM_1, registers::R7, registers::R2);
	Step();
	ExpectThat().APSRFlagsMatches("nzC");
	ExpectThat().Register(registers::R2).Equals((int32_t)0x77777777U >> 1);
}

MICROMACHINE_TEST_F(asrImmediate, R0by1, CpuTestFixture) {
	code_gen().emit_ins16("00010iiiiimmmddd", IMM_1, registers::R0, registers::R0);
	Step();
	ExpectThat().APSRFlagsMatches("nZc");
	ExpectThat().Register(registers::R0).Equals((int32_t)0x00000000U >> 1);
}
