/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#pragma once


#include "mcu.hpp"

#include <algorithm>
#include <cstddef>
#include <gtest/gtest.h>
#include <utility>
#include <string>

namespace micromachine::testing {

struct APSRFlags {
public:
	bool negativeFlag;
	bool zeroFlag;
	bool carryFlag;
	bool overflowFlag;

	APSRFlags(bool n, bool z, bool c, bool v)
		: negativeFlag(n)
		, zeroFlag(z)
		, carryFlag(c)
		, overflowFlag(v) {}

	APSRFlags(std::string flagStr)
		: negativeFlag(false)
		, zeroFlag(false)
		, carryFlag(false)
		, overflowFlag(false) {

		validateFlagStr(flagStr);

		for(char c : flagStr) {
			switch(c) {
				case 'n':
					negativeFlag = false;
					break;
				case 'N':
					negativeFlag = true;
					break;
				case 'z':
					zeroFlag = false;
					break;
				case 'Z':
					zeroFlag = true;
					break;
				case 'c':
					carryFlag = false;
					break;
				case 'C':
					carryFlag = true;
					break;
				case 'v':
					overflowFlag = false;
					break;
				case 'V':
					overflowFlag = true;
					break;
			}
		}
	}

private:
	static void validateFlagStr(const std::string& flagStr) {

		assert(flagStr.length() == 4);

		const std::string allowedChars = "nNzZcCvV";
		for(char c : flagStr) {
			if(0 == std::count(allowedChars.begin(), allowedChars.end(), c)) {
				assert(false && "flagStr can contain only the following characters: nNcCvV");
			}
		}

		std::string uppercaseFlagStr(flagStr);
		std::transform(uppercaseFlagStr.begin(), uppercaseFlagStr.end(), uppercaseFlagStr.begin(), ::toupper);
		for(char c : "NZCV") {
			if(std::count(uppercaseFlagStr.begin(), uppercaseFlagStr.end(), c) > 1) {
				assert(false && "flagStr can't contain the same flag more than once");
			}
		}
	}
};


class APSRFlagsStatePredicate {
protected:
	APSRFlags _expected;

public:
	APSRFlagsStatePredicate(std::string flagStr)
		: _expected(flagStr) {
	}

	APSRFlagsStatePredicate(const mcu& expected)
		: _expected(
			getNegativeFlagValueFrom(expected),
			getZeroFlagValueFrom(expected),
			getCarryFlagValueFrom(expected),
			getOverflowFlagValueFrom(expected)
		) {}

	void apply(mcu& expected) {
		expected.get_cpu().special_regs().app_status_register().negative_flag() = _expected.negativeFlag;
		expected.get_cpu().special_regs().app_status_register().zero_flag() = _expected.zeroFlag;
		expected.get_cpu().special_regs().app_status_register().carry_flag() = _expected.carryFlag;
		expected.get_cpu().special_regs().app_status_register().overflow_flag() = _expected.overflowFlag;
	}

	void check(const mcu& actual) const {
		EXPECT_PRED_FORMAT3(assertEquality, _expected.negativeFlag, getNegativeFlagValueFrom(actual), "negative");
		EXPECT_PRED_FORMAT3(assertEquality, _expected.zeroFlag, getZeroFlagValueFrom(actual), "zero");
		EXPECT_PRED_FORMAT3(assertEquality, _expected.carryFlag, getCarryFlagValueFrom(actual), "carry");
		EXPECT_PRED_FORMAT3(assertEquality, _expected.overflowFlag, getOverflowFlagValueFrom(actual), "overflow");
	}

private:
	static bool getNegativeFlagValueFrom(const mcu& target) {
		return target.get_cpu().special_regs().app_status_register().negative_flag();
	}

	static bool getZeroFlagValueFrom(const mcu& target) {
		return target.get_cpu().special_regs().app_status_register().zero_flag();
	}

	static bool getCarryFlagValueFrom(const mcu& target) {
		return target.get_cpu().special_regs().app_status_register().carry_flag();
	}

	static bool getOverflowFlagValueFrom(const mcu& target) {
		return target.get_cpu().special_regs().app_status_register().overflow_flag();
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
			   << "Equality check fail for APSR " << flagName << " flag" << std::endl
			   << " * Expected: " << flagName << " flag is " << (expectedFlagStatus ? "set" : "clear") << std::endl
			   << " * Actual  : " << flagName << " flag is " << (actualFlagStatus ? "set" : "clear");
	}
};

}
