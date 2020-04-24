
//
// Created by fla on 25.04.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#pragma once

#include "mcu.hpp"

#include <gtest/gtest.h>

namespace micromachine::testing {

class EPSRFlagsStatePredicate {
protected:
	const bool _expectedThumbFlag;

public:
	EPSRFlagsStatePredicate(bool expectedThumbFlag)
		: _expectedThumbFlag(expectedThumbFlag) {}

	EPSRFlagsStatePredicate(const mcu& expected)
		: EPSRFlagsStatePredicate(getThumbFlagValueFrom(expected)) {}

	void apply(mcu& expected) {
		expected.get_cpu().special_regs().execution_status_register().thumb_flag() = _expectedThumbFlag;
	}

	void check(const mcu& actual) const {
		EXPECT_PRED_FORMAT3(assertEquality, _expectedThumbFlag, getThumbFlagValueFrom(actual), "T");
		EXPECT_PRED_FORMAT3(assertEquality, false, getStackAlignFlagValueFrom(actual), "a");
	}

private:
	static bool getThumbFlagValueFrom(const mcu& target) {
		return target.get_cpu().special_regs().execution_status_register().thumb_flag();
	}

	static bool getStackAlignFlagValueFrom(const mcu& target) {
		return target.get_cpu().special_regs().execution_status_register().stack_alignment();
	}

	::testing::AssertionResult
	assertEquality(const char*, const char*,
					const char*, bool expectedFlagValue, bool actualFlagValue , const char* flagName) const {

		if(expectedFlagValue == actualFlagValue) {
			return ::testing::AssertionSuccess();
		}

		return ::testing::AssertionFailure()
			   << "Equality check fail for EPSR." << flagName << std::endl
			   << " * Expected: EPSR." << flagName << " is " << (expectedFlagValue ? "set" : "clear") << std::endl
			   << " * Actual  : EPSR." << flagName << " is " << (actualFlagValue ? "set" : "clear");
	}

};

}
