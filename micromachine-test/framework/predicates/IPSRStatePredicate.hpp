//
// Created by fla on 25.04.20.
//

#pragma once

#include "mcu.hpp"

#include <gtest/gtest.h>

namespace micromachine::testing {

class IPSRStatePredicate {
protected:
	const exception::Type _expectedException;

public:
	IPSRStatePredicate(exception::Type expectedException)
		: _expectedException(expectedException) {}

	IPSRStatePredicate(const mcu& expected)
		: _expectedException(getValueFrom(expected)) {}

	void apply(mcu& expected) {
		expected.get_cpu().special_regs().interrupt_status_register().set_exception_number(_expectedException);
	}

	void check(const mcu& actual) const {
		EXPECT_PRED_FORMAT2(assertEquality, _expectedException, getValueFrom(actual));
	}

private:
	static exception::Type getValueFrom(const mcu& target) {
		return target.get_cpu().special_regs().interrupt_status_register().exception_num();
	}

	::testing::AssertionResult
	assertEquality(const char*, const char*, exception::Type expectedException, exception::Type actualException) const {

		if(expectedException == actualException) {
			return ::testing::AssertionSuccess();
		}

		return ::testing::AssertionFailure()
			   << "Equality check fail for IPSR exception number" << std::endl
			   << " * Expected: IPSR exception number is " << exceptionName(expectedException) << std::endl
			   << " * Actual  : IPSR exception number is " << exceptionName(actualException);
	}

	static std::string exceptionName(exception::Type e) {
		return exception::name_of(e) + " (number=" + std::to_string(e) + ")";
	}
};

}
