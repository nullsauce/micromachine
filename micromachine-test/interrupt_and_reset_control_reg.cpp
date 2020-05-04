//
// Created by fla on 17-5-8.
//

#include "CpuTestFixture.hpp"
#include "exception/exception.hpp"
#include "registers/system_control/interrupt_and_reset_control_reg.hpp"

#include "CpuTestFixture.hpp"
#include <gtest/gtest.h>

using namespace micromachine::testing;

static mcu mcuWithAllExceptionsEnabledPendingActiveAtMaxPriority() {
	mcu mcu;
	exception_vector& exception_vector = mcu.get_exception_vector();

	for(exception::Type exception : {exception::RESET,
									 exception::NMI,
									 exception::HARDFAULT,
									 exception::SVCALL,
									 exception::PENDSV,
									 exception::SYSTICK}) {
		exception_vector.interrupt_state(exception).set_enable(true);
		exception_vector.interrupt_state(exception).set_pending(true);
		exception_vector.interrupt_state(exception).set_active(true);
	}

	exception_vector.interrupt_state(exception::SVCALL).set_priority(3);
	exception_vector.interrupt_state(exception::PENDSV).set_priority(3);
	exception_vector.interrupt_state(exception::SYSTICK).set_priority(3);
	return mcu;
}

TEST(AIRCRReg, ClearAllActiveStateForFixedAndConfigurableExceptions) {

	mcu mcu = mcuWithAllExceptionsEnabledPendingActiveAtMaxPriority();
	exception_vector& exception_vector = mcu.get_exception_vector();

	mcu.get_memory().write32(interrupt_and_reset_control_reg::AIRCR, 1 << 1);
	mcu.step();

	// we expect a HARDFAULT after the step
	EXPECT_TRUE(exception_vector.interrupt_state(exception::HARDFAULT).is_enabled());
	EXPECT_TRUE(exception_vector.interrupt_state(exception::HARDFAULT).is_pending());
	EXPECT_TRUE(exception_vector.interrupt_state(exception::HARDFAULT).is_active());

	// but the other exceptions should be reset
	for(exception::Type exception : {exception::RESET,
									 exception::NMI,
									 exception::SVCALL,
									 exception::PENDSV,
									 exception::SYSTICK}) {
		EXPECT_TRUE(exception_vector.interrupt_state(exception).is_enabled());
		EXPECT_FALSE(exception_vector.interrupt_state(exception).is_pending());
		EXPECT_FALSE(exception_vector.interrupt_state(exception).is_active());
	}

	EXPECT_EQ(0U, exception_vector.interrupt_state(exception::SVCALL).priority());
	EXPECT_EQ(0U, exception_vector.interrupt_state(exception::PENDSV).priority());
	EXPECT_EQ(0U, exception_vector.interrupt_state(exception::SYSTICK).priority());
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
