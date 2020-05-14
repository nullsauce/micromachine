
//
// Created by fla on 25.04.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#pragma once

#include "execution/execution_mode.hpp"
#include "mcu_foward_decl.hpp"

#include <gtest/gtest.h>

namespace micromachine::testing {

class ExecutionModeStatePredicate {
protected:
	const micromachine::system::execution_mode::mode _expectedExecutionMode;

public:
	ExecutionModeStatePredicate(micromachine::system::execution_mode::mode expectedExecutionMode);
	ExecutionModeStatePredicate(const micromachine::system::mcu& expected);
	void apply(micromachine::system::mcu& expected);
	void check(const micromachine::system::mcu& actual) const;

private:
	static micromachine::system::execution_mode::mode getValueFrom(const micromachine::system::mcu& target);

	::testing::AssertionResult assertEquality(const char*,
											  const char*,
											  micromachine::system::execution_mode::mode expectedMode,
											  micromachine::system::execution_mode::mode actualMode) const;

	static std::string getName(micromachine::system::execution_mode::mode mode);
};

} // namespace micromachine::testing
