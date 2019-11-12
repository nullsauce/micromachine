//
// Created by fla on 17-5-8.
//

#include <gtest/gtest.h>
#include "exception_state.hpp"
#include "interrupter.hpp"

class ExceptionVectorTestBench : public ::testing::Test {
protected:
	nvic _nvic;
	shpr2_reg _sph2;
	shpr3_reg _sph3;
	exception_state_vector _evec;
	interrupter _interrupter;
	ExceptionVectorTestBench()
		: _evec(_nvic, _sph2, _sph3)
		, _interrupter(_evec)
	{}

	void SetUp() override {
		_nvic.reset();
		_sph2.reset();
		_sph3.reset();
		_evec.reset();
	}

	void TearDown() override {

	}

	void SimulateExceptionHandling(exception::Type e) {
		_evec.interrupt_state(e).activate();
		_evec.interrupt_state(e).clear_pending();
		_evec.interrupt_state(e).deactivate();
	}

};

TEST_F(ExceptionVectorTestBench, NotPendingWhenEmpty)
{
	EXPECT_FALSE(_evec.any_active());
}

TEST_F(ExceptionVectorTestBench, ActivatingClearsPendingFlag)
{
	_interrupter.raise_hardfault();
	EXPECT_TRUE(_evec.any_pending());
	EXPECT_EQ(exception::Type::HARDFAULT, _evec.top_pending()->number());
	_evec.interrupt_state<exception::Type::HARDFAULT>().activate();
	EXPECT_FALSE(_evec.interrupt_state<exception::Type::HARDFAULT>().is_pending());
	EXPECT_FALSE(_evec.any_pending());
	EXPECT_EQ(nullptr, _evec.top_pending()->number());
}

TEST_F(ExceptionVectorTestBench, RasingOnePendingShouldBePending)
{
	_interrupter.raise_hardfault();
	EXPECT_TRUE(_evec.any_pending());
}

TEST_F(ExceptionVectorTestBench, RasingOnePendingShouldBeTheOneSeenAsTopPending)
{
	_interrupter.raise_hardfault();
	EXPECT_TRUE(_evec.any_pending());
	ASSERT_NE(nullptr, _evec.top_pending());
	EXPECT_EQ(exception::Type::HARDFAULT, _evec.top_pending()->number());
}


TEST_F(ExceptionVectorTestBench, TopPendingShouldBeHighestPriority)
{
	// Raise a HARDFAULT exception
	_interrupter.raise_hardfault();
	ASSERT_NE(nullptr, _evec.top_pending());
	EXPECT_EQ(exception::Type::HARDFAULT, _evec.top_pending()->number());

	// Raise a NMI exception
	// Reset is higher priority and should take precedence as top pending
	_interrupter.raise_nmi();
	ASSERT_NE(nullptr, _evec.top_pending());
	EXPECT_EQ(exception::Type::NMI, _evec.top_pending()->number());

	// Simulate handling of the NMI exception
	SimulateExceptionHandling(exception::Type::NMI);

	// The HARDFAULT exception should still be pending
	ASSERT_NE(nullptr, _evec.top_pending());
	EXPECT_EQ(exception::Type::HARDFAULT, _evec.top_pending()->number());

	// Simulate the handling of the HARDFAULT exception
	SimulateExceptionHandling(exception::Type::HARDFAULT);

	// There should be no more pending exceptions
	EXPECT_EQ(nullptr, _evec.top_pending());
}


TEST_F(ExceptionVectorTestBench, RaiseExternalInterrupt)
{
	_interrupter.raise_external_interrupt(12);
	ASSERT_NE(nullptr, _evec.top_pending());
	EXPECT_EQ(exception::Type::EXTI_12, _evec.top_pending()->number());
}

TEST_F(ExceptionVectorTestBench, ExceptionWithLowerNumberTakesPrecedenceOnExceptionWithSamePriority)
{
	_interrupter.raise_external_interrupt(13);
	_interrupter.raise_external_interrupt(12);
	ASSERT_NE(nullptr, _evec.top_pending());
	EXPECT_EQ(exception::Type::EXTI_12, _evec.top_pending()->number());
}

TEST_F(ExceptionVectorTestBench, ShprBasedPriorityExceptionPriorityReadWrite)
{
	_evec.interrupt_state<exception::Type::SVCALL>().set_priority(3);
	_evec.interrupt_state<exception::Type::PENDSV>().set_priority(2);
	_evec.interrupt_state<exception::Type::SYSTICK>().set_priority(1);

	EXPECT_EQ(3, _evec.interrupt_state<exception::Type::SVCALL>().priority());
	EXPECT_EQ(2, _evec.interrupt_state<exception::Type::PENDSV>().priority());
	EXPECT_EQ(1, _evec.interrupt_state<exception::Type::SYSTICK>().priority());
}

TEST_F(ExceptionVectorTestBench, NvicBasedPriorityExceptionPriorityReadWrite)
{
	for(size_t i = 0; i < 16; i++) {
		_evec.interrupt_state(exception::Type::EXTI_00 + i).set_priority(i % 4);
	}

	for(size_t i = 0; i < 16; i++) {
		EXPECT_EQ(i % 4, _evec.interrupt_state(exception::Type::EXTI_00 + i).priority());
	}
}

TEST_F(ExceptionVectorTestBench, ExceptionWithLowerPriorityTakesPecedence)
{
	_evec.interrupt_state<exception::Type::SVCALL>().set_priority(3);
	_evec.interrupt_state<exception::Type::PENDSV>().set_priority(2);
	_evec.interrupt_state<exception::Type::SYSTICK>().set_priority(1);
	_interrupter.raise_pendsv();
	_interrupter.raise_svcall();
	_interrupter.raise_systick();
	ASSERT_NE(nullptr, _evec.top_pending());
	EXPECT_EQ(exception::Type::SYSTICK, _evec.top_pending()->number());
	SimulateExceptionHandling(exception::Type::SYSTICK);

	ASSERT_NE(nullptr, _evec.top_pending());
	EXPECT_EQ(exception::Type::PENDSV, _evec.top_pending()->number());
	SimulateExceptionHandling(exception::Type::PENDSV);

	ASSERT_NE(nullptr, _evec.top_pending());
	EXPECT_EQ(exception::Type::SVCALL, _evec.top_pending()->number());
	SimulateExceptionHandling(exception::Type::SVCALL);

	ASSERT_EQ(nullptr, _evec.top_pending());
}

