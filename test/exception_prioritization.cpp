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
	ExceptionStateVector _evec;
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

};

TEST_F(ExceptionVectorTestBench, NotPendingWhenEmpty)
{
	EXPECT_FALSE(_evec.any_active());
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
	EXPECT_EQ(Exception::Type::HARDFAULT, _evec.top_pending()->number());
}


TEST_F(ExceptionVectorTestBench, TopPendingShouldBeHighestPriority)
{
	// Raise a HARDFAULT exception
	_interrupter.raise_hardfault();
	ASSERT_NE(nullptr, _evec.top_pending());
	EXPECT_EQ(Exception::Type::HARDFAULT, _evec.top_pending()->number());

	// Raise a NMI exception
	// Reset is higher priority and should take precedence as top pending
	_interrupter.raise_nmi();
	ASSERT_NE(nullptr, _evec.top_pending());
	EXPECT_EQ(Exception::Type::NMI, _evec.top_pending()->number());

	// Simulate handling of the NMI exception
	_evec.interrupt_state(Exception::Type::NMI).activate();
	_evec.interrupt_state(Exception::Type::NMI).clear_pending();
	_evec.interrupt_state(Exception::Type::NMI).deactivate();

	// The HARDFAULT exception should still be pending
	ASSERT_NE(nullptr, _evec.top_pending());
	EXPECT_EQ(Exception::Type::HARDFAULT, _evec.top_pending()->number());

	// Simulate the handling of the HARDFAULT exception
	_evec.interrupt_state(Exception::Type::HARDFAULT).activate();
	_evec.interrupt_state(Exception::Type::HARDFAULT).clear_pending();
	_evec.interrupt_state(Exception::Type::HARDFAULT).deactivate();

	// There should be no more pending exceptions
	EXPECT_EQ(nullptr, _evec.top_pending());
}
