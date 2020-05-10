//
// Created by fla on 26.04.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#pragma once

#include "ExceptionStatePredicate.hpp"

namespace micromachine::testing {

class ExceptionActiveStatePredicate : public ExceptionStatePredicate {
private:
	const bool _expectedActiveState;

public:
	ExceptionActiveStatePredicate(exception exceptionType, bool expectedActiveState)
		: ExceptionStatePredicate(exceptionType)
		, _expectedActiveState(expectedActiveState) {}

	ExceptionActiveStatePredicate(exception exceptionType, const mcu& expected)
		: ExceptionActiveStatePredicate(exceptionType, expected.exceptions().is_active(exceptionType))
	{}

	void apply(mcu& expected) {
		expected.exceptions().set_active(_exception, _expectedActiveState);
	}

	void check(const mcu& actual) const {
		EXPECT_PRED_FORMAT2(assertEquality, _expectedActiveState, actual.exceptions().is_active(_exception));
	}

private:
	::testing::AssertionResult
	assertEquality(const char*, const char*, bool expectedActiveState, bool actualActiveState) const {

		if(expectedActiveState == actualActiveState) {
			return ::testing::AssertionSuccess();
		}

		return ::testing::AssertionFailure()
			   << "Equality check fail for active status of Exception " << exceptionDetailedName(_exception) << std::endl
			   << " * Expected: " << _exception.name() << " is "
			   << (expectedActiveState ? "active" : "NOT active") << std::endl
			   << " * Actual  : " << _exception.name() << " is "
			   << (actualActiveState ? "active" : "NOT active") << std::endl;
	}
};

}
