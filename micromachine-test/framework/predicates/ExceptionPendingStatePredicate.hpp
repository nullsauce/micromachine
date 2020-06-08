#pragma once

#include "ExceptionStatePredicate.hpp"

#include "mcu_foward_decl.hpp"
#include <gtest/gtest.h>

namespace micromachine::testing {

class ExceptionPendingStatePredicate : public ExceptionStatePredicate {
private:
	const bool _expectedPendingState;

public:
	ExceptionPendingStatePredicate(micromachine::system::exception exceptionType, bool expectedPendingState);
	ExceptionPendingStatePredicate(micromachine::system::exception exceptionType, const micromachine::system::mcu& expected);
	void apply(micromachine::system::mcu& expected);
	void check(const micromachine::system::mcu& actual) const;

private:
	::testing::AssertionResult
	assertEquality(const char*, const char*, bool expectedPendingState, bool actualPendingState) const;
};

} // namespace micromachine::testing
