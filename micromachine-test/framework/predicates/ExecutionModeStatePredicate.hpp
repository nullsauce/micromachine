
//
// Created by fla on 25.04.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#pragma once

#include "mcu.hpp"

#include <gtest/gtest.h>

namespace micromachine::testing {

class ExecutionModeStatePredicate {
protected:
	const execution_mode::mode _expectedExecutionMode;

public:
	ExecutionModeStatePredicate(execution_mode::mode expectedExecutionMode)
		: _expectedExecutionMode(expectedExecutionMode) {}

	ExecutionModeStatePredicate(const mcu& expected)
		: ExecutionModeStatePredicate(getValueFrom(expected)) {}

	void apply(mcu& expected) {
		expected.get_cpu().set_execution_mode(_expectedExecutionMode);
	}

	void check(const mcu& actual) const {
		EXPECT_PRED_FORMAT2(assertEquality, _expectedExecutionMode, getValueFrom(actual));
	}

private:
	static execution_mode::mode getValueFrom(const mcu& target) {
		return target.get_cpu().get_execution_mode();
	}

	::testing::AssertionResult
	assertEquality(const char*, const char*, execution_mode::mode expectedMode,
											  execution_mode::mode actualMode) const {

		if(expectedMode == actualMode) {
			return ::testing::AssertionSuccess();
		}

		return ::testing::AssertionFailure()
			   << "Equality check fail for execution mode: " << std::endl
			   << " * Expected: execution mode to be " << getName(expectedMode) << std::endl
			   << " * Actual  : execution mode is " << getName(actualMode) << std::endl;
	}

	static std::string getName(execution_mode::mode mode) {
		static const char * names[] = {
			"Thread mode",
			"Handler mode"
		};
		return names[mode & 1];
	}
};

}
