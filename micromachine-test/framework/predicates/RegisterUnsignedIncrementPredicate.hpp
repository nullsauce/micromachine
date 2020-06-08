#pragma once

#include "mcu_foward_decl.hpp"
#include "types/types.hpp"
#include <gtest/gtest.h>

namespace micromachine::testing {

class RegisterUnsignedIncrementPredicate {
protected:
	const micromachine::system::reg_idx _regIdx;
	const uint32_t _expectedIncrement;
	uint32_t _expectedValue;
	uint32_t _initialValue;

public:
	RegisterUnsignedIncrementPredicate(micromachine::system::reg_idx regIdx, uint32_t expectedIncrement);
	void apply(micromachine::system::mcu& target);
	void check(const micromachine::system::mcu& actual) const;

private:
	::testing::AssertionResult assertEquality(const char*,
											  const char*,
											  const char*,
											  uint32_t expectedRegValue,
											  uint32_t actualRegValue,
											  micromachine::system::reg_idx regIdx) const;
};

}
