/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#pragma once

#include <gtest/gtest.h>

#include "MachineTestHarness.hpp"


namespace micromachine::testing {

class CpuTestFixture : public MachineTestHarness {
public:
	static constexpr uint32_t INITIAL_R0 = 0x00000000U;
	static constexpr uint32_t INITIAL_R1 = 0x11111111U;
	static constexpr uint32_t INITIAL_R2 = 0x22222222U;
	static constexpr uint32_t INITIAL_R3 = 0x33333333U;
	static constexpr uint32_t INITIAL_R4 = 0x44444444U;
	static constexpr uint32_t INITIAL_R5 = 0x55555555U;
	static constexpr uint32_t INITIAL_R6 = 0x66666666U;
	static constexpr uint32_t INITIAL_R7 = 0x77777777U;
	static constexpr uint32_t INITIAL_R8 = 0x88888888U;
	static constexpr uint32_t INITIAL_R9 = 0x99999999U;
	static constexpr uint32_t INITIAL_R10 = 0xAAAAAAAAU;
	static constexpr uint32_t INITIAL_R11 = 0xBBBBBBBBU;
	static constexpr uint32_t INITIAL_R12 = 0xCCCCCCCCU;
	static constexpr uint32_t INITIAL_LR = 0xFFFFFFFFU;

protected:
	virtual void SetUp() override {
		// Fill general purpose registers with placeholder values
		// PC & SP values are set by the machine
		setReg(registers::R0, INITIAL_R0);
		setReg(registers::R1, INITIAL_R1);
		setReg(registers::R2, INITIAL_R2);
		setReg(registers::R3, INITIAL_R3);
		setReg(registers::R4, INITIAL_R4);
		setReg(registers::R5, INITIAL_R5);
		setReg(registers::R6, INITIAL_R6);
		setReg(registers::R7, INITIAL_R7);
		setReg(registers::R8, INITIAL_R8);
		setReg(registers::R9, INITIAL_R9);
		setReg(registers::R10, INITIAL_R10);
		setReg(registers::R11, INITIAL_R11);
		setReg(registers::R12, INITIAL_R12);
		setReg(registers::LR, INITIAL_LR);
		MachineTestHarness::SetUp();
	}
};

}
