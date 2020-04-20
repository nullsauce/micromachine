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
#define IMM_0  0
#define IMM_1  1
#define IMM_3  3
#define IMM_4  4
#define IMM_31 31


/* LSL - Immediate (Logical Shift Left)
   Encoding: 000 00 imm:5 Rm:3 Rd:3 */
MICROMACHINE_TEST_F(lslImmediate, MovR7toR0_CarryUnmodified, CpuTestFixture) {
	code_gen().emit_ins16("00000iiiiimmmddd", IMM_0, registers::R7, registers::R0);
	getCpu().regs().set(registers::R7, 0x77777777U);
	getCpu().regs().app_status_register().write_carry_flag(false);
	Step();
	ExpectThat().XPSRFlagsEquals("nzc");
	ExpectThat().Register(registers::R0).Equals(0x77777777U);
}

MICROMACHINE_TEST_F(lslImmediate, MovR0toR7_ZeroResultAndCarryUnmodified, CpuTestFixture) {
	code_gen().emit_ins16("00000iiiiimmmddd", IMM_0, registers::R0, registers::R7);
	getCpu().regs().app_status_register().write_carry_flag(true);
	Step();
	ExpectThat().XPSRFlagsEquals("nZC");
	ExpectThat().Register(registers::R7).Equals(0x0);
}

MICROMACHINE_TEST_F(lslImmediate, ShiftR1by3_ResultInNegativeValue, CpuTestFixture) {
	getCpu().regs().set(registers::R1, 0x11111111U);
	code_gen().emit_ins16("00000iiiiimmmddd", IMM_3, registers::R1, registers::R0);
	Step();
	ExpectThat().XPSRFlagsEquals("Nzc");
	ExpectThat().Register(registers::R0).Equals(0x11111111U << 3);
}

MICROMACHINE_TEST_F(lslImmediate, ShiftR1by4_HasCarryOut, CpuTestFixture) {
	getCpu().regs().set(registers::R1, 0x11111111U);
	code_gen().emit_ins16("00000iiiiimmmddd", IMM_4, registers::R1, registers::R0);
	Step();
	ExpectThat().Register(registers::R0).Equals(0x11111111U << 4);
	ExpectThat().XPSRFlagsEquals("nzC");
}

MICROMACHINE_TEST_F(lslImmediate, ShiftR0by31_PushesLowestbitIntoSignBit, CpuTestFixture) {
	code_gen().emit_ins16("00000iiiiimmmddd", IMM_31, registers::R0, registers::R0);
	getCpu().regs().set(registers::R0, 1U);
	Step();
	ExpectThat().XPSRFlagsEquals("Nzc");
	ExpectThat().Register(registers::R0).Equals(1U << 31);
}

MICROMACHINE_TEST_F(lslImmediate, CarryOutFromHighestBit, CpuTestFixture) {
	code_gen().emit_ins16("00000iiiiimmmddd", IMM_1, registers::R0, registers::R0);
	getCpu().regs().set(registers::R0, 0xA0000000U);
	Step();
	ExpectThat().XPSRFlagsEquals("nzC");
	ExpectThat().Register(registers::R0).Equals(0xA0000000U << 1);
}

MICROMACHINE_TEST_F(lslImmediate, CarryOutFromLowestBit, CpuTestFixture) {
	code_gen().emit_ins16("00000iiiiimmmddd", IMM_31, registers::R0, registers::R0);
	getCpu().regs().set(registers::R0, 0x2U);
	Step();
	ExpectThat().XPSRFlagsEquals("nZC");
	ExpectThat().Register(registers::R0).Equals(0x2U << 31);
}
