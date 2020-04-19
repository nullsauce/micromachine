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


/* EOR - Register
   Encoding: 010000 0001 Rm:3 Rdn:3 */
/* NOTE: APSR_C state is maintained by this instruction. */
MICROMACHINE_TEST_F(eorRegister, UseLowestRegisterForBothArgs, CpuTestFixture) {
	code_gen().emit_ins16("0100000001mmmddd", registers::R0, registers::R0);
	// Use a couple of tests to explicitly set/clear carry to verify both states are maintained.
	getCpu().regs().app_status_register().write_carry_flag(false);
	Step();
	ExpectThat().Register(registers::R0).Equals(0);
	ExpectThat().XPSRFlagsEquals("nZc");
}

MICROMACHINE_TEST_F(eorRegister, UseHighestRegisterForBothArgs, CpuTestFixture) {
	getCpu().regs().set(registers::R7, 0x77777777);
	code_gen().emit_ins16("0100000001mmmddd", registers::R7, registers::R7);
	getCpu().regs().app_status_register().write_carry_flag(true);
	Step();
	ExpectThat().Register(registers::R7).Equals(0);
	ExpectThat().XPSRFlagsEquals("nZC");
}

MICROMACHINE_TEST_F(eorRegister, XorR3andR7, CpuTestFixture) {
	getCpu().regs().set(registers::R3, 0x33333333);
	getCpu().regs().set(registers::R7, 0x77777777);
	code_gen().emit_ins16("0100000001mmmddd", registers::R3, registers::R7);
	getCpu().regs().app_status_register().write_carry_flag(false);
	Step();
	ExpectThat().XPSRFlagsEquals("nzc");
	ExpectThat().Register(registers::R7).Equals(0x33333333 ^ 0x77777777);
}

MICROMACHINE_TEST_F(eorRegister, UseXorToJustFlipNegativeSignBitOn, CpuTestFixture) {
	code_gen().emit_ins16("0100000001mmmddd", registers::R6, registers::R3);
	getCpu().regs().set(registers::R3, 0x33333333);
	getCpu().regs().set(registers::R6, 0x80000000);
	getCpu().regs().app_status_register().write_carry_flag(true);
	Step();
	ExpectThat().XPSRFlagsEquals("NzC");
	ExpectThat().Register(registers::R3).Equals(0x33333333 ^ 0x80000000);
}
