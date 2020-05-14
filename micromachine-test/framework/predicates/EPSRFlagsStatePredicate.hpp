
//
// Created by fla on 25.04.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#pragma once

#include "mcu_foward_decl.hpp"

#include <gtest/gtest.h>

namespace micromachine::testing {

class EPSRFlagsStatePredicate {
protected:
	const bool _expectedThumbFlag;

public:
	EPSRFlagsStatePredicate(bool expectedThumbFlag);
	EPSRFlagsStatePredicate(const micromachine::system::mcu& expected);
	void apply(micromachine::system::mcu& expected);
	void check(const micromachine::system::mcu& actual) const;

private:
	static bool getThumbFlagValueFrom(const micromachine::system::mcu& target);
	static bool getStackAlignFlagValueFrom(const micromachine::system::mcu& target);

	::testing::AssertionResult assertEquality(const char*,
											  const char*,
											  const char*,
											  bool expectedFlagValue,
											  bool actualFlagValue,
											  const char* flagName) const;
};

} // namespace micromachine::testing
