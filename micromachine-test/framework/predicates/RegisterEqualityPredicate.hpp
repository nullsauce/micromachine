//

//

#pragma once

#include "mcu_foward_decl.hpp"
#include "types/types.hpp"

#include <gtest/gtest.h>

namespace micromachine::testing {

class RegisterEqualityPredicate {
protected:
	const micromachine::system::reg_idx _regIdx;
	const uint32_t _expectedValue;

public:
	RegisterEqualityPredicate(micromachine::system::reg_idx regIdx, uint32_t expectedValue);
	void apply(micromachine::system::mcu& expected);
	void check(const micromachine::system::mcu& actual) const;

private:
	void expectEqualityWith(uint32_t actualValue) const;
	::testing::AssertionResult assertEquality(const char*,
											  const char*,
											  const char*,
											  uint32_t expectedRegValue,
											  uint32_t actualRegValue,
											  micromachine::system::reg_idx regIdx) const;
};

} // namespace micromachine::testing
