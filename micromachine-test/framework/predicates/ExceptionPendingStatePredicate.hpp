//
// Created by fla on 26.04.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#pragma once

#include "ExceptionStatePredicate.hpp"

namespace micromachine::testing {

class ExceptionPendingStatePredicate : public ExceptionStatePredicate {
private:
	const bool _expectedPendingState;

public:
	ExceptionPendingStatePredicate(exception exceptionType, bool expectedPendingState)
		: ExceptionStatePredicate(exceptionType)
		, _expectedPendingState(expectedPendingState) {}

	ExceptionPendingStatePredicate(exception exceptionType, const mcu& expected)
		: ExceptionPendingStatePredicate(exceptionType, expected.exceptions().is_pending(exceptionType)) {}

	void apply(mcu& expected) {
		expected.exceptions().set_pending(_exception, _expectedPendingState);
	}

	void check(const mcu& actual) const {
		EXPECT_PRED_FORMAT2(assertEquality, _expectedPendingState, actual.exceptions().is_pending(_exception));
	}

private:
	::testing::AssertionResult
	assertEquality(const char*, const char*, bool expectedPendingState, bool actualPendingState) const {

		if(expectedPendingState == actualPendingState) {
			return ::testing::AssertionSuccess();
		}

		return ::testing::AssertionFailure()
			   << "Equality check fail for pending status of Exception " << exceptionDetailedName(_exception) << std::endl
			   << " * Expected: " << _exception.name() << " is "
			   << (expectedPendingState ? "pending" : "NOT pending") << std::endl
			   << " * Actual  : " << _exception.name() << " is "
			   << (actualPendingState ? "pending" : "NOT pending") << std::endl;
	}
};

} // namespace micromachine::testing
