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


/* SBC - Register (SUBtract with Carry)
   Encoding: 010000 0110 Rm:3 Rdn:3 */
MICROMACHINE_TEST_F(sbcRegister, UseLowestRegisterForAllArgsAndShouldBeZeroWithCarrySetForNoBorrow, CpuTestFixture) {
	code_gen().emit_ins16("0100000110mmmddd", registers::R0, registers::R0);
	getCpu().regs().app_status_register().write_carry_flag(true);
	Step();
	ExpectThat().APSRFlagsMatches("nZCv");
	ExpectThat().Register(registers::R0).Equals(0);
}

MICROMACHINE_TEST_F(sbcRegister, UseHigestRegisterForAllArgsAndShouldBeZeroWithCarrySetForNoBorrow, CpuTestFixture) {
	getCpu().regs().set(registers::R7, 0x77777777U);
	code_gen().emit_ins16("0100000110mmmddd", registers::R7, registers::R7);
	getCpu().regs().app_status_register().write_carry_flag(true);
	Step();
	ExpectThat().APSRFlagsMatches("nZCv");
	ExpectThat().Register(registers::R7).Equals(0);
}

MICROMACHINE_TEST_F(sbcRegister, UseDifferentRegistersForEachArgAndOnlyCarryShouldBeSetToIndicateNoBorrow, CpuTestFixture) {
	getCpu().regs().set(registers::R1, 0x11111111U);
	getCpu().regs().set(registers::R2, 0x22222222U);
	code_gen().emit_ins16("0100000110mmmddd", registers::R1, registers::R2);
	getCpu().regs().app_status_register().write_carry_flag(true);
	Step();
	ExpectThat().APSRFlagsMatches("nzCv");
	ExpectThat().Register(registers::R2).Equals(0x22222222U - 0x11111111U);
}

MICROMACHINE_TEST_F(sbcRegister, ForceCarryClearToIndicateBorrowAndResultWillBeNegative, CpuTestFixture) {
	getCpu().regs().set(registers::R1, 0x11111111U);
	code_gen().emit_ins16("0100000110mmmddd", registers::R1, registers::R0);
	getCpu().regs().set(registers::R1, 1);
	getCpu().regs().app_status_register().write_carry_flag(true);
	Step();
	ExpectThat().APSRFlagsMatches("Nzcv");
	ExpectThat().Register(registers::R0).Equals(0U - 1U);
}

MICROMACHINE_TEST_F(sbcRegister, ForceNegativeOverflow, CpuTestFixture) {
	getCpu().regs().set(registers::R1, 0x11111111U);
	getCpu().regs().set(registers::R2, 0x22222222U);
	code_gen().emit_ins16("0100000110mmmddd", registers::R1, registers::R2);
	getCpu().regs().set(registers::R2, 0x80000000U);
	getCpu().regs().set(registers::R1, 1U);
	getCpu().regs().app_status_register().write_carry_flag(true);
	Step();
	ExpectThat().APSRFlagsMatches("nzCV");
	ExpectThat().Register(registers::R2).Equals((int32_t)0x80000000U - 1U);
}

MICROMACHINE_TEST_F(sbcRegister, ForcePositiveOverflow, CpuTestFixture) {
	code_gen().emit_ins16("0100000110mmmddd", registers::R1, registers::R2);
	getCpu().regs().set(registers::R2, 0x7FFFFFFFU);
	getCpu().regs().set(registers::R1, -1U);
	getCpu().regs().app_status_register().write_carry_flag(true);
	Step();
	ExpectThat().APSRFlagsMatches("NzcV");
	ExpectThat().Register(registers::R2).Equals(0x7FFFFFFF + 1U);
}

MICROMACHINE_TEST_F(sbcRegister, ClearCarryToCauseABorrowToOccur, CpuTestFixture) {
	getCpu().regs().set(registers::R1, 0x11111111U);
	getCpu().regs().set(registers::R2, 0x22222222U);
	code_gen().emit_ins16("0100000110mmmddd", registers::R1, registers::R2);
	getCpu().regs().app_status_register().write_carry_flag(false); // Causes borrow.
	Step();
	ExpectThat().APSRFlagsMatches("nzCv");
	ExpectThat().Register(registers::R2).Equals(0x22222222U - 1U - 0x11111111U);
}
