//
// Created by fla on 17-5-8.
//

#include <gtest/gtest.h>
#include "timer.hpp"

TEST(SystickOperation, WhenDisabledTimerDoeNotCountsDown) {
	systick syst;
	syst.control_register().set_enabled(false);
	syst.reload_value_register() = 1000;
	syst.current_value_register().set_internal(10);
	syst.tick();
	EXPECT_EQ(10, syst.current_value_register());
}

TEST(SystickOperation, WhenEnabledTimerCountsDownFromValueInSYST_CVR) {
	systick syst;
	syst.control_register().set_enabled(true);
	syst.reload_value_register() = 1000;
	syst.current_value_register().set_internal(10);
	syst.tick();
	EXPECT_EQ(9, syst.current_value_register());
}

TEST(SystickOperation, WhenTheCounterReachesZeroItReloadsTheValueInSYST_RVRonTheNextClockAndDecrementsAfter) {
	systick syst;
	syst.control_register().set_enabled(true);
	syst.reload_value_register() = 1000U;
	syst.current_value_register().set_internal(1);
	syst.tick();
	// When The Counter Reaches Zero
	EXPECT_EQ(0, syst.current_value_register());
	syst.tick();
	// It Reloads The Value In SYST_RVR on The Next Clock
	EXPECT_EQ(1000, syst.current_value_register());
	syst.tick();
	// And Decrements on subsequent clocks
	EXPECT_EQ(999, syst.current_value_register());
}

TEST(SystickOperation, WhenTheCounterTransitionsToZeroItSetsTheCOUNTFLAGstatusBitToOne) {
	systick syst;
	syst.control_register().set_enabled(true);
	syst.reload_value_register() = 1000;
	syst.current_value_register().set_internal(1);
	EXPECT_FALSE(syst.control_register().count_flag());
	syst.tick();
	EXPECT_TRUE(syst.control_register().count_flag());
}

TEST(SystickOperation, ReadingTheCOUNTFLAGstatusBitClearsItToZero) {
	systick syst;
	syst.control_register().set_enabled(true);
	syst.reload_value_register() = 1000;
	syst.current_value_register().set_internal(1);
	syst.tick();
	EXPECT_TRUE(syst.control_register().count_flag());
	EXPECT_FALSE(syst.control_register().count_flag());
}

TEST(SystickOperation, WritingToSYST_CVRclearsBothTheRegisterAndTheCOUNTFLAGstatusBit) {
	systick syst;
	syst.control_register().set_enabled(true);
	syst.reload_value_register() = 1000;
	syst.current_value_register().set_internal(1);
	EXPECT_EQ(1, syst.current_value_register());
	syst.tick();
	EXPECT_EQ(0, syst.current_value_register());
	syst.current_value_register() = 123;
	EXPECT_EQ(0, syst.current_value_register());
	EXPECT_EQ(0, syst.control_register().count_flag());
}

TEST(SystickOperation, WritingToSYST_CVRcausesAReloadOfSYST_CVRfromSYST_RVRonNextTick) {
	systick syst;
	syst.control_register().set_enabled(true);
	syst.reload_value_register() = 1000;
	syst.current_value_register().set_internal(1);
	EXPECT_EQ(1, syst.current_value_register());
	syst.tick();
	EXPECT_EQ(0, syst.current_value_register());
	syst.current_value_register() = 123;
	EXPECT_EQ(0, syst.current_value_register());
	EXPECT_EQ(0, syst.control_register().count_flag());
	syst.tick();
	EXPECT_EQ(1000, syst.current_value_register());
}

TEST(SystickOperation, WritingAvalueOfZeroToSYST_RVRdisablesTheCounterOnTheNextWrap) {
	systick syst;
	syst.control_register().set_enabled(true);
	syst.reload_value_register() = 0;
	syst.current_value_register().set_internal(1);
	syst.tick();
	EXPECT_TRUE(syst.control_register().enabled());
	syst.tick();
	EXPECT_FALSE(syst.control_register().enabled());
}

/* //TODO: this must be tested withing CPU
TEST(SystickOperation, AwriteToSYST_CVRdoesNotTriggerTheSysTickExceptionLogic) {
	systick syst;
	syst.control_register().set_enabled(true);
	syst.reload_value_register() = 0;
	syst.current_value_register().set_internal(100U);
	syst.tick();
	syst.current_value_register() = 4;
	EXPECT_FALSE(syst.control_register().enabled());
}*/
