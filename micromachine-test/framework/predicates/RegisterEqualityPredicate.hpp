//
// Created by fla on 25.04.20.
//

#pragma once

#include "RegisterPredicate.hpp"
#include "mcu.hpp"

#include <gtest/gtest.h>

namespace micromachine::testing {

class RegisterEqualityPredicate : public RegisterPredicate {
protected:
	const reg_idx _regIdx;
	const uint32_t _expectedValue;

public:
	RegisterEqualityPredicate(reg_idx regIdx, uint32_t expectedValue)
		: _regIdx(regIdx)
		, _expectedValue(expectedValue) {}

	void apply(mcu& expected) {
		expected.get_cpu().regs().set(_regIdx, _expectedValue);
	}

	void check(const mcu& actual) const {
		expectEqualityWith(actual.get_cpu().regs().get(_regIdx));
	}

private:
	void expectEqualityWith(uint32_t actualValue) const {
		EXPECT_PRED_FORMAT3(assertEquality, _expectedValue, actualValue, _regIdx);
	}

	::testing::AssertionResult assertEquality(const char*,
											  const char*,
											  const char*,
											  uint32_t expectedRegValue,
											  uint32_t actualRegValue,
											  reg_idx regIdx) const {
		if(expectedRegValue == actualRegValue) {
			return ::testing::AssertionSuccess();
		}

		return ::testing::AssertionFailure() << "Equality check fail for register " << registerName(regIdx) << std::endl
											 << " * Expected: " << expectedRegValue << std::endl
											 << " * Actual  : " << actualRegValue;
	}
};

}
