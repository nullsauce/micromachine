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


/* ADD - Immediate - Encoding T1
   Encoding: 000 11 1 0 Imm:3 Rn:3 Rd:3 */
MICROMACHINE_TEST_F(addImmediate, T1UseLowestRegisterOnlyAddLargestImmediate, CpuTestFixture) {
	code_gen().emit_add_imm_t1(7, registers::R0, registers::R0);

	Step();

	ExpectThat()
		.APSRFlagsMatches("nzcv")
		.Register(registers::R0).Equals(0U + 7U)
	;
}

MICROMACHINE_TEST_F(addImmediate, T1UseHigestRegisterOnlyAddSmallestImmediate, CpuTestFixture) {
	code_gen().emit_add_imm_t1(0, registers::R7, registers::R7);

	getCpu().regs().set(registers::R7, 0x77777777U);

	Step();

	ExpectThat()
		.APSRFlagsMatches("nzcv")
		.Register(registers::R7).Equals(0x77777777U + 0U)
	;
}

MICROMACHINE_TEST_F(addImmediate, T1UseDifferentRegistersForEachArg, CpuTestFixture) {
	code_gen().emit_add_imm_t1(3, registers::R7, registers::R0);

	getCpu().regs().set(registers::R7, 0x77777777U);

	Step();

	ExpectThat()
		.APSRFlagsMatches("nzcv")
		.Register(registers::R0).Equals(0x77777777U + 3U)
	;

}

MICROMACHINE_TEST_F(addImmediate, T1ForceCarryByAdding1ToLargestInteger, CpuTestFixture) {
	code_gen().emit_add_imm_t1(1, registers::R6, registers::R1);

	getCpu().regs().set(registers::R1, 0x11111111U);
	getCpu().regs().set(registers::R6, 0xFFFFFFFFU);

	Step();

	ExpectThat()
		.APSRFlagsMatches("nZCv")
		.Register(registers::R1).Equals(0)
	;
}

MICROMACHINE_TEST_F(addImmediate, T1ForceOverflowPastLargestPositiveInteger, CpuTestFixture) {
	code_gen().emit_add_imm_t1(1, registers::R2, registers::R5);

	getCpu().regs().set(registers::R2, 0x7FFFFFFFU);

	Step();

	ExpectThat()
		.APSRFlagsMatches("NzcV")
		.Register(registers::R5).Equals(0x7FFFFFFFU + 1U)
	;
}


/* ADD - Immediate - Encoding T2
   Encoding: 001 10 Rdn:3 Imm:8 */
MICROMACHINE_TEST_F(addImmediate, T2UseLowestRegisterAndAddLargestImmediate, CpuTestFixture) {
	code_gen().emit_add_imm_t2(registers::R0, 255);

	Step();

	ExpectThat()
		.APSRFlagsMatches("nzcv")
		.Register(registers::R0).Equals(0U + 255U)
	;
}

MICROMACHINE_TEST_F(addImmediate, T2UseHigestRegisterAndAddSmallestImmediate, CpuTestFixture) {
	code_gen().emit_add_imm_t2(registers::R7, 0);

	getCpu().regs().set(registers::R7, 0x77777777U);

	Step();

	ExpectThat()
		.APSRFlagsMatches("nzcv")
		.Register(registers::R7).Equals(0x77777777U + 0U)
	;

}

MICROMACHINE_TEST_F(addImmediate, T2ForceCarryByAdding1ToLargestInteger, CpuTestFixture) {
	code_gen().emit_add_imm_t2(registers::R3, 1);

	getCpu().regs().set(registers::R3, 0xFFFFFFFFU);

	Step();

	ExpectThat()
		.APSRFlagsMatches("nZCv")
		.Register(registers::R3).Equals(0)
	;

}

MICROMACHINE_TEST_F(addImmediate, T2ForceOverflowPastLargestPositiveInteger, CpuTestFixture) {
	code_gen().emit_add_imm_t2(registers::R3, 1);

	getCpu().regs().set(registers::R3, 0x7FFFFFFFU);

	Step();

	ExpectThat()
		.APSRFlagsMatches("NzcV")
		.Register(registers::R3).Equals(0x7FFFFFFFU + 1)
	;
}
