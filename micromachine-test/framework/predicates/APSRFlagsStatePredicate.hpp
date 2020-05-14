/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#pragma once

#include <algorithm>
#include <cstddef>
#include <gtest/gtest.h>
#include <utility>
#include <string>

#include "mcu_foward_decl.hpp"

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
	APSRFlagsStatePredicate(std::string flagStr);
	APSRFlagsStatePredicate(const micromachine::system::mcu& expected);
	void apply(micromachine::system::mcu& expected);
	void check(const micromachine::system::mcu& actual) const;

private:
	static bool getNegativeFlagValueFrom(const micromachine::system::mcu& target);
	static bool getZeroFlagValueFrom(const micromachine::system::mcu& target);
	static bool getCarryFlagValueFrom(const micromachine::system::mcu& target);
	static bool getOverflowFlagValueFrom(const micromachine::system::mcu& target);
	::testing::AssertionResult assertEquality(const char*,
											  const char*,
											  const char*,
											  bool expectedFlagStatus,
											  bool actualFlagStatus,
											  const char* flagName) const;
};

}
