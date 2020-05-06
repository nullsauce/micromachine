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
	ExceptionActiveStatePredicate(exception::Type exceptionType, bool expectedActiveState)
		: ExceptionStatePredicate(exceptionType)
		, _expectedActiveState(expectedActiveState) {}

	ExceptionActiveStatePredicate(exception::Type exceptionType, const mcu& expected)
		: ExceptionActiveStatePredicate(exceptionType, expected.exceptions().is_active(exceptionType))
	{}

	void apply(mcu& expected) {
		expected.exceptions().set_active(_exceptionType, _expectedActiveState);
	}

	void check(const mcu& actual) const {
		EXPECT_PRED_FORMAT2(assertEquality, _expectedActiveState, actual.exceptions().is_active(_exceptionType));
	}

private:
	::testing::AssertionResult
	assertEquality(const char*, const char*, bool expectedActiveState, bool actualActiveState) const {

		if(expectedActiveState == actualActiveState) {
			return ::testing::AssertionSuccess();
		}

		return ::testing::AssertionFailure()
			   << "Equality check fail for active status of Exception " << exceptionName(_exceptionType) << std::endl
			   << " * Expected: " << exception::name_of(_exceptionType) << " is "
			   << (expectedActiveState ? "active" : "NOT active") << std::endl
			   << " * Actual  : " << exception::name_of(_exceptionType) << " is "
			   << (actualActiveState ? "active" : "NOT active") << std::endl;
	}

	static std::string exceptionName(exception::Type e) {
		return exception::name_of(e) + " (number=" + std::to_string(e) + ")";
	}
};

}
