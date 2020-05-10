//
// Created by fla on 26.04.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#pragma once

#include "ExceptionStatePredicate.hpp"

namespace micromachine::testing {

class ExceptionPriorityStatePredicate : public ExceptionStatePredicate {
private:
	const exception::priority_t _expectedPriority;

public:
	ExceptionPriorityStatePredicate(exception exceptionType, exception::priority_t expectedPendingState)
		: ExceptionStatePredicate(exceptionType)
		, _expectedPriority(expectedPendingState) {}

	ExceptionPriorityStatePredicate(exception exceptionType, const mcu& expected)
		: ExceptionPriorityStatePredicate(exceptionType, expected.exceptions().priority(exceptionType)) {}

	void apply(mcu& expected) {
		expected.exceptions().set_priority(_exception, _expectedPriority);
	}

	void check(const mcu& actual) const {
		EXPECT_PRED_FORMAT2(assertEquality, _expectedPriority, actual.exceptions().priority(_exception));
	}

private:
	::testing::AssertionResult assertEquality(const char*,
											  const char*,
											  exception::priority_t expectedPriority,
											  exception::priority_t actualPriority) const {

		if(expectedPriority == actualPriority) {
			return ::testing::AssertionSuccess();
		}

		return ::testing::AssertionFailure()
			   << "Equality check fail for priority of Exception " << exceptionDetailedName(_exception) << std::endl
			   << " * Expected: " << _exception.name() << " has a priority of "
			   << (int)expectedPriority << std::endl
			   << " * Actual  : " << _exception.name() << " has a priority of " << (int)actualPriority
			   << std::endl;
	}
};

} // namespace micromachine::testing
