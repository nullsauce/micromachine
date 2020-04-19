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


/* BLX (Branch with Link and Exchange)
   Encoding: 010001 11 1 Rm:4 (0)(0)(0) */
MICROMACHINE_TEST_F(blx, UseLowestRegisterToBranchToEvenAddressWhichClearsThumbModeAndHardFaultOnNextInstr, CpuTestFixture) {
	constexpr uint32_t INITIAL_PC = 0x00001000;
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("010001111mmmm000", registers::R0);
	getCpu().regs().set(registers::R0, INITIAL_PC + 16);
	Step();
	ExpectThat()
		.XPSRFlagsEquals("t")
		.Register(registers::PC).Equals(INITIAL_PC + 16)
		.Register(registers::LR).Equals((INITIAL_PC + 2) | 1);

	Step();
	ExpectThat().HardfaultHandlerReached();
}


MICROMACHINE_TEST_F(blx, UseRegisterToBranchToOddAddressAsRequiredForThumb, CpuTestFixture) {
	constexpr uint32_t INITIAL_PC = 0x00001000;
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("010001111mmmm000", registers::R2);
	getCpu().regs().set(registers::R2, (INITIAL_PC + 16) | 1);
	Step();
	ExpectThat().Register(registers::PC).Equals(INITIAL_PC + 16);
	ExpectThat().Register(registers::LR).Equals((INITIAL_PC + 2) | 1);
}

MICROMACHINE_TEST_F(blx, UseHighestRegisterToBranchToOddAddressAsRequiredForThumb, CpuTestFixture) {
	constexpr uint32_t INITIAL_PC = 0x00001000;
	getCpu().regs().set_pc(INITIAL_PC);
	code_gen().emit_ins16("010001111mmmm000", registers::LR);
	getCpu().regs().set(registers::LR, (INITIAL_PC + 16) | 1);
	Step();
	ExpectThat().Register(registers::PC).Equals(INITIAL_PC + 16);
	ExpectThat().Register(registers::LR).Equals((INITIAL_PC + 2) | 1);
}
/*
TEST_SIM_ONLY(blx, UnpredictableToUseR15)
{
    code_gen().emit_ins16("010001111mmmm000", registers::PC);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    ExpectThat().Register(registers::PC).Equals(INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(blx, UnpredictableForBit0ToBeHigh)
{
    code_gen().emit_ins16("010001111mmmm001", registers::R0);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    ExpectThat().Register(registers::PC).Equals(INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(blx, UnpredictableForBit1ToBeHigh)
{
    code_gen().emit_ins16("010001111mmmm010", registers::R0);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    ExpectThat().Register(registers::PC).Equals(INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(blx, UnpredictableForBit2ToBeHigh)
{
    code_gen().emit_ins16("010001111mmmm100", registers::R0);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    ExpectThat().Register(registers::PC).Equals(INITIAL_PC);
    step(&m_context);
}
*/
