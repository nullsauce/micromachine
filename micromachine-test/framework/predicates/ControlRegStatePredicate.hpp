//
// Created by fla on 25.04.20.
//

#pragma once

#include "mcu_foward_decl.hpp"

#include <gtest/gtest.h>

namespace micromachine::testing {

class ControlRegStatePredicate {
protected:
	const bool _expectednPrivFlag;
	const bool _expectedSpSelectionFlag;

public:
	ControlRegStatePredicate(bool nPrivFlag, bool spSelectionFlag);
	ControlRegStatePredicate(const micromachine::system::mcu& expected);
	void apply(micromachine::system::mcu& expected);
	void check(const micromachine::system::mcu& actual) const;

private:
	static bool getnPrivFlagFrom(const micromachine::system::mcu& target);
	static bool getSpSelectionFlagFrom(const micromachine::system::mcu& target);
	::testing::AssertionResult assertEquality(const char*,
											  const char*,
											  const char*,
											  bool expectedFlagStatus,
											  bool actualFlagStatus,
											  const char* flagName) const;
};

}
