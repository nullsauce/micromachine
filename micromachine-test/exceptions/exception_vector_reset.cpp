//
// Created by fla on 17-5-8.
//

#include "CpuTestFixture.hpp"
#include "exception/exception.hpp"
#include "registers/system_control/config_and_control_reg.hpp"

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


TEST(ExceptionVectorReset, ResetClearsAllActiveStateForFixedAndConfigurableExceptions) {

	mcu mcu = mcuWithAllExceptionsEnabledPendingActiveAtMaxPriority();
	exception_vector& exception_vector = mcu.get_exception_vector();

	exception_vector.reset();

	for(exception::Type exception : {exception::RESET,
									 exception::NMI,
									 exception::HARDFAULT,
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
