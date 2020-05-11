//
// Created by fla on 25.04.20.
//

#pragma once

#include "mcu.hpp"

#include <gtest/gtest.h>

namespace micromachine::testing {

class PrimaskRegStatePredicate {
protected:
	const bool _expectedPrimaskFlag;

public:
	PrimaskRegStatePredicate(bool expectedPrimaskFlag)
		: _expectedPrimaskFlag(expectedPrimaskFlag) {}

	PrimaskRegStatePredicate(const mcu& expected)
		: PrimaskRegStatePredicate(getPrimaskFlagFrom(expected))
	{}

	void apply(mcu& expected) {
		expected.get_cpu().special_regs().primask_register().pm() = _expectedPrimaskFlag;
	}

	void check(const mcu& actual) const {
		EXPECT_PRED_FORMAT2(assertEquality,
							_expectedPrimaskFlag, getPrimaskFlagFrom(actual));
	}

private:
	static bool getPrimaskFlagFrom(const mcu& target) {
		return target.get_cpu().special_regs().primask_register().pm();
	}

	::testing::AssertionResult
	assertEquality(const char*, const char*, bool expectedFlagStatus, bool actualFlagStatus) const {

		if(expectedFlagStatus == actualFlagStatus) {
			return ::testing::AssertionSuccess();
		}

		return ::testing::AssertionFailure()
			   << "Equality check fail for PRIMASK.PM flag" << std::endl
			   << " * Expected: PRIMASK.PM flag is " << (expectedFlagStatus ? "set" : "clear") << std::endl
			   << " * Actual  : PRIMASK.PM flag is " << (actualFlagStatus ? "set" : "clear");
	}
};

}
