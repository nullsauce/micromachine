#pragma once

#include "ExceptionStatePredicate.hpp"

#include "mcu_foward_decl.hpp"
#include <gtest/gtest.h>

namespace micromachine::testing {

class ExceptionActiveStatePredicate : public ExceptionStatePredicate {
private:
	const bool _expectedActiveState;

public:
	ExceptionActiveStatePredicate(micromachine::system::exception exceptionType, bool expectedActiveState);
	ExceptionActiveStatePredicate(micromachine::system::exception exceptionType, const micromachine::system::mcu& expected);
	void apply(micromachine::system::mcu& expected);
	void check(const micromachine::system::mcu& actual) const;

private:
	::testing::AssertionResult
	assertEquality(const char*, const char*, bool expectedActiveState, bool actualActiveState) const;
};

} // namespace micromachine::testing
