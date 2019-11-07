//
// Created by fla on 17-5-8.
//

#include <gtest/gtest.h>
#include "timer.hpp"


class SystickTestBench : public ::testing::Test {
protected:
	nvic _nvic;
	shpr2_reg _sph2;
	shpr3_reg _sph3;
	ExceptionStateVector _evec;
	interrupter _interrupter;
	systick syst;
	SystickTestBench()
		: _evec(_nvic, _sph2, _sph3)
		, _interrupter(_evec)
		, syst(_interrupter)
	{}

	void SetUp() override {
		_nvic.reset();
		_sph2.reset();
		_sph3.reset();
		_evec.reset();
	}

	void TearDown() override {

	}

};

TEST_F(SystickTestBench, WhenDisabledTimerDoeNotCountsDown) {
	syst.control_register().set_enabled(false);
	syst.reload_value_register() = 1000;
	syst.current_value_register().set_internal(10);
	syst.tick();
	EXPECT_EQ(10, syst.current_value_register());
}

TEST_F(SystickTestBench, WhenEnabledTimerCountsDownFromValueInSYST_CVR) {
	syst.control_register().set_enabled(true);
	syst.reload_value_register() = 1000;
	syst.current_value_register().set_internal(10);
	syst.tick();
	EXPECT_EQ(9, syst.current_value_register());
}

TEST_F(SystickTestBench,
	 WhenTheCounterReachesZeroItReloadsTheValueInSYST_RVRonTheNextClockAndDecrementsAfter) {
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

TEST_F(SystickTestBench, WhenTheCounterTransitionsToZeroItSetsTheCOUNTFLAGstatusBitToOne) {
	syst.control_register().set_enabled(true);
	syst.reload_value_register() = 1000;
	syst.current_value_register().set_internal(1);
	EXPECT_FALSE(syst.control_register().count_flag());
	syst.tick();
	EXPECT_TRUE(syst.control_register().count_flag());
}

TEST_F(SystickTestBench, ReadingTheCOUNTFLAGstatusBitClearsItToZero) {
	syst.control_register().set_enabled(true);
	syst.reload_value_register() = 1000;
	syst.current_value_register().set_internal(1);
	syst.tick();
	EXPECT_TRUE(syst.control_register().count_flag());
	EXPECT_FALSE(syst.control_register().count_flag());
}

TEST_F(SystickTestBench, WritingToSYST_CVRclearsBothTheRegisterAndTheCOUNTFLAGstatusBit) {
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

TEST_F(SystickTestBench, WritingToSYST_CVRcausesAReloadOfSYST_CVRfromSYST_RVRonNextTick) {
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

TEST_F(SystickTestBench, WritingAvalueOfZeroToSYST_RVRdisablesTheCounterOnTheNextWrap) {
	syst.control_register().set_enabled(true);
	syst.reload_value_register() = 0;
	syst.current_value_register().set_internal(1);
	syst.tick();
	EXPECT_TRUE(syst.control_register().enabled());
	syst.tick();
	EXPECT_FALSE(syst.control_register().enabled());
}

TEST_F(SystickTestBench, SYSTICKIsPendingAfterCounterReachesZero) {
	syst.control_register().set_enabled(true);
	syst.reload_value_register() = 1;
	syst.current_value_register().set_internal(1);
	syst.tick();
	EXPECT_TRUE(_evec.interrupt_state<Exception::Type::SYSTICK>().is_pending());
}

TEST_F(SystickTestBench, AwriteToSYST_CVRdoesNotTriggerTheSysTickExceptionLogic) {
	syst.control_register().set_enabled(true);
	syst.reload_value_register() = 1;
	syst.current_value_register() = 100U;
	syst.tick();
	EXPECT_FALSE(_evec.interrupt_state<Exception::Type::SYSTICK>().is_pending());
}
