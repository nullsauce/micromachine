
#include "RegisterEqualityPredicate.hpp"
#include "ValueFormaters.hpp"
#include "mcu.hpp"

namespace micromachine::testing {

using namespace micromachine::system;

RegisterEqualityPredicate::RegisterEqualityPredicate(reg_idx regIdx, uint32_t expectedValue)
	: _regIdx(regIdx)
	, _expectedValue(expectedValue) {}

void RegisterEqualityPredicate::apply(mcu& expected) {
	expected.get_cpu().regs().set(_regIdx, _expectedValue);
}

void RegisterEqualityPredicate::check(const mcu& actual) const {
	expectEqualityWith(actual.get_cpu().regs().get(_regIdx));
}

void RegisterEqualityPredicate::expectEqualityWith(uint32_t actualValue) const {
	EXPECT_PRED_FORMAT3(assertEquality, _expectedValue, actualValue, _regIdx);
}

::testing::AssertionResult RegisterEqualityPredicate::assertEquality(const char*,
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
} // namespace micromachine::testing
