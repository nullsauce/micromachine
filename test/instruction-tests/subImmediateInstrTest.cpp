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


/* SUB - Immediate - Encoding T1
   Encoding: 000 11 1 1 Imm:3 Rn:3 Rd:3 */
MICROMACHINE_TEST_F(subImmediate, T1UseLowestRegisterOnly_SmallestImmediate, CpuTestFixture) {
	code_gen().emit_ins16("0001111iiinnnddd", 0, registers::R0, registers::R0);
	Step();
	ExpectThat().APSRFlagsMatches("nZCv");
	ExpectThat().Register(registers::R0).Equals(0U);
}

MICROMACHINE_TEST_F(subImmediate, T1UseHigestRegisterOnly_LargestImmediate, CpuTestFixture) {
	code_gen().emit_ins16("0001111iiinnnddd", 7, registers::R7, registers::R7);
	getCpu().regs().set(registers::R7, 0x77777777U);
	Step();
	ExpectThat().APSRFlagsMatches("nzCv");
	ExpectThat().Register(registers::R7).Equals(0x77777777U - 7U);
}

MICROMACHINE_TEST_F(subImmediate, T1UseDifferentRegistersForEachArg, CpuTestFixture) {
	code_gen().emit_ins16("0001111iiinnnddd", 3, registers::R0, registers::R2);
	Step();
	ExpectThat().APSRFlagsMatches("Nzcv");
	ExpectThat().Register(registers::R2).Equals(0U - 3U);
}

MICROMACHINE_TEST_F(subImmediate, T1ForceOverflowPastLargestNegativeInteger, CpuTestFixture) {
	code_gen().emit_ins16("0001111iiinnnddd", 1, registers::R1, registers::R6);
	getCpu().regs().set(registers::R1, 0x80000000);
	Step();
	ExpectThat().APSRFlagsMatches("nzCV");
	ExpectThat().Register(registers::R6).Equals(0x80000000U - 1U);
}



/* SUB - Immediate - Encoding T2
   Encoding: 001 11 Rdn:3 Imm:8 */
MICROMACHINE_TEST_F(subImmediate, T2LowestRegister_SmallestImmediate, CpuTestFixture) {
	code_gen().emit_ins16("00111dddiiiiiiii", registers::R0, 0);
	Step();
	ExpectThat().APSRFlagsMatches("nZCv");
	ExpectThat().Register(registers::R0).Equals(0U);
}

MICROMACHINE_TEST_F(subImmediate, T2HigestRegister_LargestImmediate, CpuTestFixture) {
	code_gen().emit_ins16("00111dddiiiiiiii", registers::R7, 255);
	getCpu().regs().set(registers::R7, 0x77777777U);
	Step();
	ExpectThat().APSRFlagsMatches("nzCv");
	ExpectThat().Register(registers::R7).Equals(0x77777777U - 255U);
}

MICROMACHINE_TEST_F(subImmediate, T2Subtract127FromR0CausesNoCarryToIndicateBorrowAndNegativeResult, CpuTestFixture) {
	code_gen().emit_ins16("00111dddiiiiiiii", registers::R0, 127);
	Step();
	ExpectThat().APSRFlagsMatches("Nzcv");
	ExpectThat().Register(registers::R0).Equals(0U - 127U);
}

MICROMACHINE_TEST_F(subImmediate, T2ForceOverflowPastLargestNegativeInteger, CpuTestFixture) {
	code_gen().emit_ins16("00111dddiiiiiiii", registers::R3, 1);
	getCpu().regs().set(registers::R3, 0x80000000);
	Step();
	ExpectThat().APSRFlagsMatches("nzCV");
	ExpectThat().Register(registers::R3).Equals(0x80000000U - 1U);
}
