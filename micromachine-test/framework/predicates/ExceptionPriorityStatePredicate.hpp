//
// Created by fla on 26.04.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#pragma once

#include "ExceptionStatePredicate.hpp"
#include "mcu_foward_decl.hpp"

#include <gtest/gtest.h>

namespace micromachine::testing {

class ExceptionPriorityStatePredicate : public ExceptionStatePredicate {
private:
	const micromachine::system::exception::priority_t _expectedPriority;

public:
	ExceptionPriorityStatePredicate(micromachine::system::exception exceptionType,
									micromachine::system::exception::priority_t expectedPendingState);
	ExceptionPriorityStatePredicate(micromachine::system::exception exceptionType,
									const micromachine::system::mcu& expected);
	void apply(micromachine::system::mcu& expected);
	void check(const micromachine::system::mcu& actual) const;

private:
	::testing::AssertionResult assertEquality(const char*,
											  const char*,
											  micromachine::system::exception::priority_t expectedPriority,
											  micromachine::system::exception::priority_t actualPriority) const;
};

} // namespace micromachine::testing
