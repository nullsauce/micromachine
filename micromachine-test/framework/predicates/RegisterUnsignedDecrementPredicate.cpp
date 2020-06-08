

#include "RegisterUnsignedDecrementPredicate.hpp"
#include "ValueFormaters.hpp"
#include "mcu.hpp"

namespace micromachine::testing {

using namespace micromachine::system;

RegisterUnsignedDecrementPredicate::RegisterUnsignedDecrementPredicate(reg_idx regIdx, uint32_t expectedDecrement)
	: _regIdx(regIdx)
	, _expectedDecrement(expectedDecrement)
	, _expectedValue(0)
	, _initialValue(0) {}

void RegisterUnsignedDecrementPredicate::apply(mcu& target) {
	_initialValue = target.get_cpu().regs().get(_regIdx);
	_expectedValue = _initialValue - _expectedDecrement;
	target.get_cpu().regs().set(_regIdx, _expectedValue);
}

void RegisterUnsignedDecrementPredicate::check(const mcu& actual) const {
	uint32_t actualValue = actual.get_cpu().regs().get(_regIdx);
	EXPECT_PRED_FORMAT3(assertEquality, _expectedValue, actualValue, _regIdx);
}

::testing::AssertionResult RegisterUnsignedDecrementPredicate::assertEquality(const char*,
																			  const char*,
																			  const char*,
																			  uint32_t expectedRegValue,
																			  uint32_t actualRegValue,
																			  reg_idx regIdx) const {
	if(expectedRegValue == actualRegValue) {
		return ::testing::AssertionSuccess();
	}

	return ::testing::AssertionFailure() << "Equality check fail for register " << registerName(regIdx) << std::endl
										 << " * Expected: increment of " << _expectedDecrement
										 << " over initial value of " << _initialValue
										 << " to amount to final value of " << expectedRegValue << std::endl
										 << " * Actual: final value of " << actualRegValue
										 << " which represents an increment of " << (actualRegValue - _initialValue)
										 << " over the initial value of " << _initialValue << std::endl;
}

} // namespace micromachine::testing
