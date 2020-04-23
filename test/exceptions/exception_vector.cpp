//
// Created by fla on 17-5-8.
//

#include "exception/exception_vector.hpp"
#include "exception/exception_controller.hpp"
#include "exception/exception_defs.hpp"
#include "nvic.hpp"
#include <gtest/gtest.h>

using namespace micromachine::system;

class ExceptionVectorTestBench : public ::testing::Test {
protected:
	nvic _nvic;
	micromachine::system::shpr2_reg _sph2;
	shpr3_reg _sph3;
	micromachine::system::exception_vector _evec;
	micromachine::system::exception_controller _interrupter;
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

	void SimulateExceptionHandling(micromachine::system::exception::Type e) {
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
	EXPECT_EQ(micromachine::system::exception::Type::HARDFAULT, _evec.top_pending()->number());
	_evec.interrupt_state<micromachine::system::exception::Type::HARDFAULT>().activate();
	EXPECT_FALSE(_evec.interrupt_state<micromachine::system::exception::Type::HARDFAULT>().is_pending());
	EXPECT_FALSE(_evec.any_pending());
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
	EXPECT_EQ(micromachine::system::exception::Type::HARDFAULT, _evec.top_pending()->number());
}


TEST_F(ExceptionVectorTestBench, TopPendingShouldBeHighestPriority)
{
	// Raise a HARDFAULT exception
	_interrupter.raise_hardfault();
	ASSERT_NE(nullptr, _evec.top_pending());
	EXPECT_EQ(micromachine::system::exception::Type::HARDFAULT, _evec.top_pending()->number());

	// Raise a NMI exception
	// Reset is higher priority and should take precedence as top pending
	_interrupter.raise_nmi();
	ASSERT_NE(nullptr, _evec.top_pending());
	EXPECT_EQ(micromachine::system::exception::Type::NMI, _evec.top_pending()->number());

	// Simulate handling of the NMI exception
	SimulateExceptionHandling(micromachine::system::exception::Type::NMI);

	// The HARDFAULT exception should still be pending
	ASSERT_NE(nullptr, _evec.top_pending());
	EXPECT_EQ(micromachine::system::exception::Type::HARDFAULT, _evec.top_pending()->number());

	// Simulate the handling of the HARDFAULT exception
	SimulateExceptionHandling(micromachine::system::exception::Type::HARDFAULT);

	// There should be no more pending exceptions
	EXPECT_EQ(nullptr, _evec.top_pending());
}


TEST_F(ExceptionVectorTestBench, RaiseExternalInterrupt)
{
	_interrupter.enable_external_interrupt(12);
	_interrupter.raise_external_interrupt(12);
	ASSERT_NE(nullptr, _evec.top_pending());
	EXPECT_EQ(micromachine::system::exception::Type::EXTI_12, _evec.top_pending()->number());
}

TEST_F(ExceptionVectorTestBench, RaiseDisabledExternalInterrupt)
{
	_interrupter.disable_external_interrupt(12);
	_interrupter.raise_external_interrupt(12);
	ASSERT_EQ(nullptr, _evec.top_pending());
}

TEST_F(ExceptionVectorTestBench, ExceptionWithLowerNumberTakesPrecedenceOnExceptionWithSamePriority)
{
	_interrupter.enable_external_interrupt(13);
	_interrupter.enable_external_interrupt(12);
	_interrupter.raise_external_interrupt(13);
	_interrupter.raise_external_interrupt(12);
	ASSERT_NE(nullptr, _evec.top_pending());
	EXPECT_EQ(micromachine::system::exception::Type::EXTI_12, _evec.top_pending()->number());
}

TEST_F(ExceptionVectorTestBench, ExceptionWithHigherNumberTakesPrecedenceOnDisabledException) {
	_interrupter.enable_external_interrupt(13);
	_interrupter.disable_external_interrupt(12);
	_interrupter.raise_external_interrupt(13);
	_interrupter.raise_external_interrupt(12);
	ASSERT_NE(nullptr, _evec.top_pending());
	EXPECT_EQ(micromachine::system::exception::Type::EXTI_13, _evec.top_pending()->number());
}

TEST_F(ExceptionVectorTestBench, ShprBasedPriorityExceptionPriorityReadWrite)
{
	_evec.interrupt_state<micromachine::system::exception::Type::SVCALL>().set_priority(3);
	_evec.interrupt_state<micromachine::system::exception::Type::PENDSV>().set_priority(2);
	_evec.interrupt_state<micromachine::system::exception::Type::SYSTICK>().set_priority(1);

	EXPECT_EQ(3, _evec.interrupt_state<micromachine::system::exception::Type::SVCALL>().priority());
	EXPECT_EQ(2, _evec.interrupt_state<micromachine::system::exception::Type::PENDSV>().priority());
	EXPECT_EQ(1, _evec.interrupt_state<micromachine::system::exception::Type::SYSTICK>().priority());
}

TEST_F(ExceptionVectorTestBench, NvicBasedPriorityExceptionPriorityReadWrite)
{
	for(size_t i = 0; i < 16; i++) {
		_evec.interrupt_state(micromachine::system::exception::Type::EXTI_00 + i).set_priority(i % 4);
	}

	for(size_t i = 0; i < 16; i++) {
		EXPECT_EQ(i % 4, _evec.interrupt_state(micromachine::system::exception::Type::EXTI_00 + i).priority());
	}
}

TEST_F(ExceptionVectorTestBench, ExceptionWithLowerPriorityTakesPecedence)
{
	_evec.interrupt_state<micromachine::system::exception::Type::SVCALL>().set_priority(3);
	_evec.interrupt_state<micromachine::system::exception::Type::PENDSV>().set_priority(2);
	_evec.interrupt_state<micromachine::system::exception::Type::SYSTICK>().set_priority(1);
	_interrupter.raise_pendsv();
	_interrupter.raise_svcall();
	_interrupter.raise_systick();
	ASSERT_NE(nullptr, _evec.top_pending());
	EXPECT_EQ(micromachine::system::exception::Type::SYSTICK, _evec.top_pending()->number());
	SimulateExceptionHandling(micromachine::system::exception::Type::SYSTICK);

	ASSERT_NE(nullptr, _evec.top_pending());
	EXPECT_EQ(micromachine::system::exception::Type::PENDSV, _evec.top_pending()->number());
	SimulateExceptionHandling(micromachine::system::exception::Type::PENDSV);

	ASSERT_NE(nullptr, _evec.top_pending());
	EXPECT_EQ(micromachine::system::exception::Type::SVCALL, _evec.top_pending()->number());
	SimulateExceptionHandling(micromachine::system::exception::Type::SVCALL);

	ASSERT_EQ(nullptr, _evec.top_pending());
}

