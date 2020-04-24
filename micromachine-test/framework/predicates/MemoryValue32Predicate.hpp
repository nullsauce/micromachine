//
// Created by fla on 25.04.20.
//

#pragma once

#include <string>
#include <types/types.hpp>

#include "MemoryValuePredicate.hpp"
#include "ValueFormaters.hpp"

namespace micromachine::testing {

class MemoryValue32Predicate : public MemoryValuePredicate {
protected:
	const uint32_t _value;

public:
	MemoryValue32Predicate(uint32_t address, uint32_t value)
		: MemoryValuePredicate(address)
		, _value(value)
	{}

	void apply(mcu& expected) {
		if(!expected.get_memory().write32(_address, _value)) {
			micromachine_fail("MemoryValue32Predicate applied at an unmapped address. address=%08x value=%08x", _address, _value);
		}
	}

	void check(const mcu& actual) const {
		uint32_t actualValue = actual.get_memory().read32_unchecked(_address);
		EXPECT_PRED_FORMAT3(assertMemoryEquality, actualValue, _value, _address);
	}

private:
	static ::testing::AssertionResult assertMemoryEquality(const char*,
														   const char*,
														   const char*,
														   uint32_t expectedValue,
														   uint32_t actualValue,
														   uint32_t address) {

		if(expectedValue == actualValue) {
			return ::testing::AssertionSuccess();
		}

		return ::testing::AssertionFailure()
			<< "Equality check fail for memory at address " << uint32ToStr(address) << std::endl
			<< " * Expected: " << uint32ToStr(expectedValue) << std::endl
			<< " * Actual  : " << uint32ToStr(actualValue) << std::endl;


	}

};

} // namespace micromachine::testing
