//
// Created by fla on 25.04.20.
//

#pragma once

#include "mcu.hpp"

#include <gtest/gtest.h>

namespace micromachine::testing {

class ControlRegStatePredicate {
protected:
	const bool _expectednPrivFlag;
	const bool _expectedSpSelectionFlag;

public:
	ControlRegStatePredicate(bool nPrivFlag, bool spSelectionFlag)
		: _expectednPrivFlag(nPrivFlag)
		, _expectedSpSelectionFlag(spSelectionFlag) {}

	ControlRegStatePredicate(const mcu& expected)
		: _expectednPrivFlag(getnPrivFlagFrom(expected))
		, _expectedSpSelectionFlag(getSpSelectionFlagFrom(expected))
	{}

	void apply(mcu& expected) {
		expected.get_cpu().special_regs().control_register().set_n_priv(_expectednPrivFlag);
		expected.get_cpu().special_regs().control_register().set_sp_sel(_expectedSpSelectionFlag);
	}

	void check(const mcu& actual) const {
		EXPECT_PRED_FORMAT3(assertEquality,
							_expectednPrivFlag, getnPrivFlagFrom(actual),
							"nPRIV");

		EXPECT_PRED_FORMAT3(assertEquality,
							_expectednPrivFlag, getSpSelectionFlagFrom(actual),
							"SPSEL");
	}

private:
	static bool getnPrivFlagFrom(const mcu& target) {
		return target.get_cpu().special_regs().control_register().n_priv();
	}

	static bool getSpSelectionFlagFrom(const mcu& target) {
		return target.get_cpu().special_regs().control_register().sp_sel();
	}

	::testing::AssertionResult assertEquality(const char*,
											  const char*,
											  const char*,
											  bool expectedFlagStatus,
											  bool actualFlagStatus,
											  const char* flagName) const {

		if(expectedFlagStatus == actualFlagStatus) {
			return ::testing::AssertionSuccess();
		}

		return ::testing::AssertionFailure()
			   << "Equality check fail for CONTROL." << flagName << " flag" << std::endl
			   << " * Expected: CONTROL." << flagName << " flag is " << (expectedFlagStatus ? "set" : "clear")
			   << std::endl
			   << " * Actual  : CONTROL." << flagName << " flag is " << (actualFlagStatus ? "set" : "clear");
	}
};

}
