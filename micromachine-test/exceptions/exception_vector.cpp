//
// Created by fla on 17-5-8.
//

#include "exception/exception_vector.hpp"
#include "exception/exception.hpp"
#include "exception/exception_vector_controller.hpp"
#include "nvic.hpp"
#include <gtest/gtest.h>

using namespace micromachine::system;

class ExceptionVectorTestBench : public ::testing::Test {
protected:
	nvic _nvic;
	shpr2_reg _sph2;
	shpr3_reg _sph3;
	exception_vector_controller _exception_controller;
	uint32_t _xpsr;
	ipsr_reg _ipsr_reg;
	interrupt_control_state_reg _icsr;
	ExceptionVectorTestBench()
		: _exception_controller(_nvic, _sph2, _sph3, _icsr)
		, _xpsr(0)
		, _ipsr_reg(_xpsr)
		, _icsr(_exception_controller, _ipsr_reg)
	{}

	void SetUp() override {
		_nvic.reset();
		_sph2.reset();
		_sph3.reset();
		_exception_controller.reset();
	}

	void TearDown() override {

	}

	void SimulateExceptionHandling(micromachine::system::exception::Type e) {
		_exception_controller.set_active(e, true);
		_exception_controller.set_pending(e, false);
		_exception_controller.set_active(e, false);
	}

};

TEST_F(ExceptionVectorTestBench, NotPendingWhenEmpty)
{
	EXPECT_FALSE(_exception_controller.any_active());
}

TEST_F(ExceptionVectorTestBench, ActivatingClearsPendingFlag)
{
	_exception_controller.raise_hardfault();
	EXPECT_TRUE(_exception_controller.any_pending());
	EXPECT_EQ(exception::HARDFAULT, _exception_controller.top_pending()->number());
	_exception_controller.set_active(exception::HARDFAULT, true);
	_exception_controller.set_pending(exception::HARDFAULT, false);
	EXPECT_FALSE(_exception_controller.is_pending(exception::Type::HARDFAULT));
	EXPECT_FALSE(_exception_controller.any_pending());
}

TEST_F(ExceptionVectorTestBench, RasingOnePendingShouldBePending)
{
	_exception_controller.raise_hardfault();
	EXPECT_TRUE(_exception_controller.any_pending());
}

TEST_F(ExceptionVectorTestBench, RasingOnePendingShouldBeTheOneSeenAsTopPending)
{
	_exception_controller.raise_hardfault();
	EXPECT_TRUE(_exception_controller.any_pending());
	ASSERT_NE(nullptr, _exception_controller.top_pending());
	EXPECT_EQ(exception::HARDFAULT, _exception_controller.top_pending()->number());
}


TEST_F(ExceptionVectorTestBench, TopPendingShouldBeHighestPriority)
{
	// Raise a HARDFAULT exception
	_exception_controller.raise_hardfault();
	ASSERT_NE(nullptr, _exception_controller.top_pending());
	EXPECT_EQ(exception::HARDFAULT, _exception_controller.top_pending()->number());

	// Raise a NMI exception
	// Reset is higher priority and should take precedence as top pending
	_exception_controller.raise_nmi();
	ASSERT_NE(nullptr, _exception_controller.top_pending());
	EXPECT_EQ(exception::NMI, _exception_controller.top_pending()->number());

	// Simulate handling of the NMI exception
	SimulateExceptionHandling(exception::NMI);

	// The HARDFAULT exception should still be pending
	ASSERT_NE(nullptr, _exception_controller.top_pending());
	EXPECT_EQ(exception::HARDFAULT, _exception_controller.top_pending()->number());

	// Simulate the handling of the HARDFAULT exception
	SimulateExceptionHandling(exception::HARDFAULT);

	// There should be no more pending exceptions
	EXPECT_EQ(nullptr, _exception_controller.top_pending());
}


TEST_F(ExceptionVectorTestBench, RaiseExternalInterrupt)
{
	_exception_controller.set_enable(exception::EXTI_12, true);
	_exception_controller.raise_external_interrupt(exception::EXTI_12);
	ASSERT_NE(nullptr, _exception_controller.top_pending());
	EXPECT_EQ(exception::EXTI_12, _exception_controller.top_pending()->number());
}

TEST_F(ExceptionVectorTestBench, RaiseDisabledExternalInterrupt)
{
	_exception_controller.set_enable(exception::EXTI_12, false);
	_exception_controller.raise_external_interrupt(exception::EXTI_12);
	ASSERT_EQ(nullptr, _exception_controller.top_pending());
}

TEST_F(ExceptionVectorTestBench, ExceptionWithLowerNumberTakesPrecedenceOnExceptionWithSamePriority)
{
	_exception_controller.set_enable(exception::EXTI_13, true);
	_exception_controller.set_enable(exception::EXTI_12, true);
	_exception_controller.raise_external_interrupt(exception::EXTI_13);
	_exception_controller.raise_external_interrupt(exception::EXTI_12);
	ASSERT_NE(nullptr, _exception_controller.top_pending());
	EXPECT_EQ(exception::EXTI_12, _exception_controller.top_pending()->number());
}

TEST_F(ExceptionVectorTestBench, ExceptionWithHigherNumberTakesPrecedenceOnDisabledException) {
	_exception_controller.set_enable(exception::EXTI_13, true);
	_exception_controller.set_enable(exception::EXTI_12, false);
	_exception_controller.raise_external_interrupt(exception::EXTI_13);
	_exception_controller.raise_external_interrupt(exception::EXTI_12);
	ASSERT_NE(nullptr, _exception_controller.top_pending());
	EXPECT_EQ(exception::EXTI_13, _exception_controller.top_pending()->number());
}

TEST_F(ExceptionVectorTestBench, ShprBasedPriorityExceptionPriorityReadWrite)
{
	_exception_controller.set_priority(exception::SVCALL, 3);
	_exception_controller.set_priority(exception::PENDSV, 2);
	_exception_controller.set_priority(exception::SYSTICK, 1);

	EXPECT_EQ(3, _exception_controller.priority(exception::SVCALL));
	EXPECT_EQ(2, _exception_controller.priority(exception::PENDSV));
	EXPECT_EQ(1, _exception_controller.priority(exception::SYSTICK));
}

TEST_F(ExceptionVectorTestBench, NvicBasedPriorityExceptionPriorityReadWrite)
{
	for(size_t i = exception::EXTI_00; i <= exception::EXTI_15; i++) {
		_exception_controller.set_priority(exception::from_number(i), i % 4);
	}

	for(size_t i = exception::EXTI_00; i <= exception::EXTI_15; i++) {
		EXPECT_EQ(i % 4, _exception_controller.priority(exception::from_number(i)));
	}
}

TEST_F(ExceptionVectorTestBench, ExceptionWithLowerPriorityTakesPecedence)
{
	_exception_controller.set_priority(exception::SVCALL, 3);
	_exception_controller.set_priority(exception::PENDSV, 2);
	_exception_controller.set_priority(exception::SYSTICK, 1);
	_exception_controller.raise_pendsv();
	_exception_controller.raise_svcall();
	_exception_controller.raise_systick();
	ASSERT_NE(nullptr, _exception_controller.top_pending());
	EXPECT_EQ(exception::SYSTICK, _exception_controller.top_pending()->number());
	SimulateExceptionHandling(exception::SYSTICK);

	ASSERT_NE(nullptr, _exception_controller.top_pending());
	EXPECT_EQ(exception::PENDSV, _exception_controller.top_pending()->number());
	SimulateExceptionHandling(exception::PENDSV);

	ASSERT_NE(nullptr, _exception_controller.top_pending());
	EXPECT_EQ(exception::SVCALL, _exception_controller.top_pending()->number());
	SimulateExceptionHandling(exception::SVCALL);

	ASSERT_EQ(nullptr, _exception_controller.top_pending());
}

