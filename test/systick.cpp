//
// Created by fla on 17-5-8.
//

#include <gtest/gtest.h>
#include "registers/system_control/systick.hpp"

TEST(SystickOperation, WhenDisabledTimerDoeNotCountsDown) {
	systick syst;
	syst.control_register().set_enabled(false);
	syst.reload_value_register() = 1000;
	syst.current_value_register().set_internal(10);
	syst.tick();
	EXPECT_EQ(10U, (word)syst.current_value_register());
}

TEST(SystickOperation, WhenEnabledTimerCountsDownFromValueInSYST_CVR) {
	systick syst;
	syst.control_register().set_enabled(true);
	syst.reload_value_register() = 1000;
	syst.current_value_register().set_internal(10);
	syst.tick();
	EXPECT_EQ(9U, (word)syst.current_value_register());
}

TEST(SystickOperation, WhenTheCounterReachesZeroItReloadsTheValueInSYST_RVRonTheNextClockAndDecrementsAfter) {
	systick syst;
	syst.control_register().set_enabled(true);
	syst.reload_value_register() = 1000;
	syst.current_value_register().set_internal(1);
	syst.tick();
	// When The Counter Reaches Zero
	EXPECT_EQ(0U, (word)syst.current_value_register());
	syst.tick();
	// It Reloads The Value In SYST_RVR on The Next Clock
	EXPECT_EQ(1000U, (word)syst.current_value_register());
	syst.tick();
	// And Decrements on subsequent clocks
	EXPECT_EQ(999U, (word)syst.current_value_register());
}