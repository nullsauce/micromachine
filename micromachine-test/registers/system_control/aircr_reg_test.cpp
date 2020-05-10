//
// Created by fla on 17-5-8.
//

#include "CpuTestFixture.hpp"
#include "exception/exception.hpp"
#include "registers/system_control/aircr_reg.hpp"

#include "CpuTestFixture.hpp"
#include <gtest/gtest.h>

using namespace micromachine::testing;

static mcu mcuWithAllExceptionsEnabledPendingActiveAtMaxPriority() {
	mcu mcu;
	exception_controller& exception_controller = mcu.exceptions();

	for(exception exception : {exception::RESET,
									 exception::NMI,
									 exception::HARDFAULT,
									 exception::SVCALL,
									 exception::PENDSV,
									 exception::SYSTICK}) {
		exception_controller.set_enable(exception, true);
		exception_controller.set_pending(exception, true);
		exception_controller.set_active(exception, true);
	}

	exception_controller.set_priority(exception::SVCALL, 3);
	exception_controller.set_priority(exception::PENDSV, 3);
	exception_controller.set_priority(exception::SYSTICK, 3);
	return mcu;
}

TEST(AIRCRReg, ClearAllActiveStateForFixedAndConfigurableExceptions) {

	mcu mcu = mcuWithAllExceptionsEnabledPendingActiveAtMaxPriority();
	exception_controller& exception_controller = mcu.exceptions();

	mcu.get_memory().write32(interrupt_and_reset_control_reg::AIRCR, 1 << 1);
	mcu.step();

	// we expect a HARDFAULT after the step
	EXPECT_TRUE(exception_controller.is_enabled(exception::HARDFAULT));
	EXPECT_TRUE(exception_controller.is_pending(exception::HARDFAULT));
	EXPECT_TRUE(exception_controller.is_active(exception::HARDFAULT));

	// but the other exceptions should be reset
	for(exception exception : {exception::RESET,
									 exception::NMI,
									 exception::SVCALL,
									 exception::PENDSV,
									 exception::SYSTICK}) {
		EXPECT_TRUE(exception_controller.is_enabled(exception));
		EXPECT_FALSE(exception_controller.is_pending(exception));
		EXPECT_FALSE(exception_controller.is_active(exception));
	}

	EXPECT_EQ(0U, exception_controller.priority(exception::SVCALL));
	EXPECT_EQ(0U, exception_controller.priority(exception::PENDSV));
	EXPECT_EQ(0U, exception_controller.priority(exception::SYSTICK));
}

TEST(AIRCRReg, ReadsCorrectValue) {
	mcu mcu;
	EXPECT_EQ(0x05FA0000, mcu.get_memory().read32(interrupt_and_reset_control_reg::AIRCR));
}

TEST(AIRCRReg, ResetsSystem) {

	mcu mcu = mcuWithAllExceptionsEnabledPendingActiveAtMaxPriority();
	uint32_t pc = mcu.get_cpu().regs().get(registers::PC);

	for(reg_idx i = 0; i < 15; ++i) {
		mcu.get_cpu().regs().set(i, 0x11111111U * (i+1));
	}

	mcu.get_memory().write32(interrupt_and_reset_control_reg::AIRCR, 1 << 2);
	mcu.step();

	for(reg_idx i = 0; i < 15; ++i) {
		EXPECT_EQ(0U, mcu.get_cpu().regs().get(i));
	}
	EXPECT_EQ(pc, mcu.get_cpu().regs().get(registers::PC));

}
