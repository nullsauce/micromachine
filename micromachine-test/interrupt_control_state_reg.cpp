//
// Created by fla on 17-5-8.
//


#include "registers/system_control/interrupt_control_state_reg.hpp"
#include "CpuTestFixture.hpp"

#include <gtest/gtest.h>

using namespace micromachine::testing;

static constexpr uint32_t ICSR_ADDR = 0xE000ED04;

MICROMACHINE_TEST_F(InterruptControlStateReg, SetNMIPendingFlag, CpuTestFixture) {
	memWrite32(ICSR_ADDR, 1 << 31);
	StepAndExpectThat(
		memoryAt(ICSR_ADDR).equals32(1 << 31)
	);
}
