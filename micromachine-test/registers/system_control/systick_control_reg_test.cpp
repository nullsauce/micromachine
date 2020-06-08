
#include "exception/exception.hpp"
#include "exception/exception_vector_controller.hpp"
#include "exception/exception_vector.hpp"
#include "nvic.hpp"
#include "registers/system_control/shpr2_reg.hpp"
#include "registers/system_control/shpr3_reg.hpp"
#include "systick.hpp"

#include <gtest/gtest.h>

using namespace micromachine::system;

class SystickTestBench : public ::testing::Test {
protected:
	nvic _nvic;
	shpr2_reg _sph2;
	shpr3_reg _sph3;
	exception_vector_controller _exception_controller;
	uint32_t _xpsr;
	ipsr_reg _ipsr_reg;
	interrupt_control_state_reg _icsr;
	systick syst;
	SystickTestBench()
		: _exception_controller(_nvic, _sph2, _sph3, _icsr)
		, _xpsr(0)
		, _ipsr_reg(_xpsr)
		, _icsr(_exception_controller, _ipsr_reg)
		, syst(_exception_controller)
	{}

	void SetUp() override {
		_nvic.reset();
		_sph2.reset();
		_sph3.reset();
		_exception_controller.reset();
	}

	void TearDown() override {

	}
};

TEST_F(SystickTestBench, WhenDisabledTimerDoeNotCountsDown) {
	syst.control_register().enabled() = false;
	syst.reload_value_register() = 1000;
	syst.current_value_register().set_internal(10);
	syst.step();
	EXPECT_EQ(10, syst.current_value_register());
}

TEST_F(SystickTestBench, WhenEnabledTimerCountsDownFromValueInSYST_CVR) {
	syst.control_register().enabled() = true;
	syst.reload_value_register() = 1000;
	syst.current_value_register().set_internal(10);
	syst.step();
	EXPECT_EQ(9, syst.current_value_register());
}

TEST_F(SystickTestBench,
	 WhenTheCounterReachesZeroItReloadsTheValueInSYST_RVRonTheNextClockAndDecrementsAfter) {
	syst.control_register().enabled() = true;
	syst.reload_value_register() = 1000U;
	syst.current_value_register().set_internal(1);
	syst.step();
	// When The Counter Reaches Zero
	EXPECT_EQ(0, syst.current_value_register());
	syst.step();
	// It Reloads The Value In SYST_RVR on The Next Clock
	EXPECT_EQ(1000, syst.current_value_register());
	syst.step();
	// And Decrements on subsequent clocks
	EXPECT_EQ(999, syst.current_value_register());
}

TEST_F(SystickTestBench, WhenTheCounterTransitionsToZeroItSetsTheCOUNTFLAGstatusBitToOne) {
	syst.control_register().enabled() = true;
	syst.reload_value_register() = 1000;
	syst.current_value_register().set_internal(1);
	EXPECT_FALSE(syst.control_register().count_flag());
	syst.step();
	EXPECT_TRUE(syst.control_register().count_flag());
}

TEST_F(SystickTestBench, ReadingTheCOUNTFLAGstatusBitClearsItToZero) {
	syst.control_register().enabled() = true;
	syst.reload_value_register() = 1000;
	syst.current_value_register().set_internal(1);
	syst.step();
	EXPECT_TRUE(syst.control_register().count_flag());
	EXPECT_FALSE(syst.control_register().count_flag());
}

TEST_F(SystickTestBench, WritingToSYST_CVRclearsBothTheRegisterAndTheCOUNTFLAGstatusBit) {
	syst.control_register().enabled() = true;
	syst.reload_value_register() = 1000;
	syst.current_value_register().set_internal(1);
	EXPECT_EQ(1, syst.current_value_register());
	syst.step();
	EXPECT_EQ(0, syst.current_value_register());
	syst.current_value_register() = 123;
	EXPECT_EQ(0, syst.current_value_register());
	EXPECT_EQ(0, syst.control_register().count_flag());
}

TEST_F(SystickTestBench, WritingToSYST_CVRcausesAReloadOfSYST_CVRfromSYST_RVRonNextTick) {
	syst.control_register().enabled() = true;
	syst.reload_value_register() = 1000;
	syst.current_value_register().set_internal(1);
	EXPECT_EQ(1, syst.current_value_register());
	syst.step();
	EXPECT_EQ(0, syst.current_value_register());
	syst.current_value_register() = 123;
	EXPECT_EQ(0, syst.current_value_register());
	EXPECT_EQ(0, syst.control_register().count_flag());
	syst.step();
	EXPECT_EQ(1000, syst.current_value_register());
}

TEST_F(SystickTestBench, WritingAvalueOfZeroToSYST_RVRdisablesTheCounterOnTheNextWrap) {
	syst.control_register().enabled() = true;
	syst.reload_value_register() = 0;
	syst.current_value_register().set_internal(1);
	syst.step();
	EXPECT_TRUE(syst.control_register().enabled());
	syst.step();
	EXPECT_FALSE(syst.control_register().enabled());
}

TEST_F(SystickTestBench, SYSTICKIsPendingAfterCounterReachesZero) {
	syst.control_register().enabled() = true;
	syst.reload_value_register() = 1;
	syst.current_value_register().set_internal(1);
	syst.step();
	EXPECT_TRUE(_exception_controller.is_pending(exception::SYSTICK));
}

TEST_F(SystickTestBench, AwriteToSYST_CVRdoesNotTriggerTheSysTickExceptionLogic) {
	syst.control_register().enabled() = true;
	syst.reload_value_register() = 1;
	syst.current_value_register() = 100U;
	syst.step();
	EXPECT_FALSE(_exception_controller.is_pending(exception::SYSTICK));
}
