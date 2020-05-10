//
// Created by fla on 17-5-8.
//

#include "CpuTestFixture.hpp"
#include "exception/exception.hpp"
#include "registers/system_control/ccr_reg.hpp"

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

TEST(ExceptionStateReset, ResetClearsAllActiveStateForFixedAndConfigurableExceptions) {

	mcu mcu = mcuWithAllExceptionsEnabledPendingActiveAtMaxPriority();
	exception_controller& exception_controller = mcu.exceptions();

	mcu.reset();

	for(exception exception : {exception::RESET,
									 exception::NMI,
									 exception::HARDFAULT,
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
