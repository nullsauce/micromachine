//
// Created by fla on 25.04.20.
//

#pragma once

#include "RegisterPredicate.hpp"

namespace micromachine::testing {

class RegisterUnsignedIncrementPredicate : public RegisterPredicate {
protected:
	const reg_idx _regIdx;
	const uint32_t _expectedIncrement;
	uint32_t _expectedValue;
	uint32_t _initialValue;

public:
	RegisterUnsignedIncrementPredicate(reg_idx regIdx, uint32_t expectedIncrement)
		: _regIdx(regIdx)
		, _expectedIncrement(expectedIncrement)
		, _expectedValue(0)
		, _initialValue(0) {}

	void apply(mcu& target) {
		_initialValue = target.get_cpu().regs().get(_regIdx);
		_expectedValue = _initialValue + _expectedIncrement;
		target.get_cpu().regs().set(_regIdx, _expectedValue);
	}

	void check(const mcu& actual) const {
		uint32_t actualValue = actual.get_cpu().regs().get(_regIdx);
		EXPECT_PRED_FORMAT3(assertEquality, _expectedValue, actualValue, _regIdx);
	}

private:
	::testing::AssertionResult assertEquality(const char*,
											  const char*,
											  const char*,
											  uint32_t expectedRegValue,
											  uint32_t actualRegValue,
											  reg_idx regIdx) const {
		if(expectedRegValue == actualRegValue) {
			return ::testing::AssertionSuccess();
		}

		return ::testing::AssertionFailure()
			   << "Equality check fail for register " << registerName(regIdx) << std::endl
			   << " * Expected: increment of " << _expectedIncrement << " over initial value of " << _initialValue
			   << " to amount to final value of " << expectedRegValue << std::endl
			   << " * Actual: final value of " << actualRegValue << " which represents an increment of "
			   << (actualRegValue - _initialValue) << " over the initial value of " << _initialValue << std::endl;
	}
};

}
