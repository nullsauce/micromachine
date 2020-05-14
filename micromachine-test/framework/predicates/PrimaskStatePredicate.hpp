//
// Created by fla on 25.04.20.
//

#pragma once

#include "mcu_foward_decl.hpp"
#include <gtest/gtest.h>

namespace micromachine::testing {

class PrimaskRegStatePredicate {
protected:
	const bool _expectedPrimaskFlag;

public:
	PrimaskRegStatePredicate(bool expectedPrimaskFlag);
	PrimaskRegStatePredicate(const micromachine::system::mcu& expected);
	void apply(micromachine::system::mcu& expected);
	void check(const micromachine::system::mcu& actual) const;

private:
	static bool getPrimaskFlagFrom(const micromachine::system::mcu& target);
	::testing::AssertionResult
	assertEquality(const char*, const char*, bool expectedFlagStatus, bool actualFlagStatus) const;
};

} // namespace micromachine::testing
