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
	static constexpr uint32_t INITIAL_LR = 0xFFFFFFFF;
protected:
	virtual void SetUp() override {
		// Fill all general purpose registers with placeholder values
		for(reg_idx regIdx = 0; regIdx < core_registers::NUM_GP_REGS; regIdx++) {
			setReg(regIdx, 0x11111111U * regIdx);
		}
		setReg(registers::LR, INITIAL_LR);
		MachineTestHarness::SetUp();
	}
};

}
