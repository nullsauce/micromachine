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


/* BX (Branch and Exchange)
   Encoding: 010001 11 0 Rm:4 (0)(0)(0) */
MICROMACHINE_TEST_F(bx, UseLowestRegisterToBranchToEvenAddressWhichClearsThumbModeAndHardFaultOnNextInstr,
					CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().pc() = INITIAL_PC;
	code_gen().emit_ins16("010001110mmmm000", registers::R0);
	getCpu().regs().set(registers::R0, INITIAL_PC + 16);
	Step();
	ExpectThat().ThumbBitIsNotSet();
	ExpectThat().Register(registers::PC).Equals(INITIAL_PC + 16);

	Step();
	ExpectThat().HardfaultHandlerReached();
}

MICROMACHINE_TEST_F(bx, UseHighestRegisterToBranchToOddAddressWhichIsRequiredForThumb, CpuTestFixture) {
	const uint32_t INITIAL_PC = code_gen().write_address();
	getCpu().regs().pc() = INITIAL_PC;

	code_gen().emit_ins16("010001110mmmm000", registers::LR);
	getCpu().regs().set(registers::LR, (INITIAL_PC + 16) | 1);
	Step();
	ExpectThat().Register(registers::PC).Equals(INITIAL_PC + 16);
}
/*
TEST_SIM_ONLY(bx, UnpredictableToUseR15)
{
    code_gen().emit_ins16("010001110mmmm000", registers::PC);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    ExpectThat().Register(registers::PC).Equals(INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(bx, UnpredictableForBit0ToBeHigh)
{
    code_gen().emit_ins16("010001110mmmm001", registers::R0);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    ExpectThat().Register(registers::PC).Equals(INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(bx, UnpredictableForBit1ToBeHigh)
{
    code_gen().emit_ins16("010001110mmmm010", registers::R0);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    ExpectThat().Register(registers::PC).Equals(INITIAL_PC);
    pinkySimStep(&m_context);
}

TEST_SIM_ONLY(bx, UnpredictableForBit2ToBeHigh)
{
    code_gen().emit_ins16("010001110mmmm100", registers::R0);
    setExpectedStepReturn(CPU_STEP_UNPREDICTABLE);
    ExpectThat().Register(registers::PC).Equals(INITIAL_PC);
    step(&m_context);
}
*/
