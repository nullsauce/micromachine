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
	ExceptionPriorityStatePredicate(exception::Type exceptionType, exception::priority_t expectedPendingState)
		: ExceptionStatePredicate(exceptionType)
		, _expectedPriority(expectedPendingState) {}

	ExceptionPriorityStatePredicate(exception::Type exceptionType, const mcu& expected)
		: ExceptionPriorityStatePredicate(exceptionType, expected.exceptions().priority(exceptionType)) {}

	void apply(mcu& expected) {
		expected.exceptions().set_priority(_exceptionType, _expectedPriority);
	}

	void check(const mcu& actual) const {
		EXPECT_PRED_FORMAT2(assertEquality, _expectedPriority, actual.exceptions().priority(_exceptionType));
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
			   << "Equality check fail for priority of Exception " << exceptionName(_exceptionType) << std::endl
			   << " * Expected: " << exception::name_of(_exceptionType) << " has a priority of "
			   << (int)expectedPriority << std::endl
			   << " * Actual  : " << exception::name_of(_exceptionType) << " has a priority of " << (int)actualPriority
			   << std::endl;
	}

	static std::string exceptionName(exception::Type e) {
		return exception::name_of(e) + " (number=" + std::to_string(e) + ")";
	}
};

} // namespace micromachine::testing
