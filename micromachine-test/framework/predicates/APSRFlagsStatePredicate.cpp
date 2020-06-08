
//


//

#include "APSRFlagsStatePredicate.hpp"
#include "mcu.hpp"

namespace micromachine::testing {

using namespace micromachine::system;

APSRFlagsStatePredicate::APSRFlagsStatePredicate(const mcu& expected)
	: _expected(getNegativeFlagValueFrom(expected),
				getZeroFlagValueFrom(expected),
				getCarryFlagValueFrom(expected),
				getOverflowFlagValueFrom(expected)) {}

APSRFlagsStatePredicate::APSRFlagsStatePredicate(std::string flagStr)
	: _expected(flagStr) {}

void APSRFlagsStatePredicate::apply(mcu& expected) {
	expected.get_cpu().special_regs().app_status_register().negative_flag() = _expected.negativeFlag;
	expected.get_cpu().special_regs().app_status_register().zero_flag() = _expected.zeroFlag;
	expected.get_cpu().special_regs().app_status_register().carry_flag() = _expected.carryFlag;
	expected.get_cpu().special_regs().app_status_register().overflow_flag() = _expected.overflowFlag;
}

void APSRFlagsStatePredicate::check(const mcu& actual) const {
	EXPECT_PRED_FORMAT3(assertEquality, _expected.negativeFlag, getNegativeFlagValueFrom(actual), "negative");
	EXPECT_PRED_FORMAT3(assertEquality, _expected.zeroFlag, getZeroFlagValueFrom(actual), "zero");
	EXPECT_PRED_FORMAT3(assertEquality, _expected.carryFlag, getCarryFlagValueFrom(actual), "carry");
	EXPECT_PRED_FORMAT3(assertEquality, _expected.overflowFlag, getOverflowFlagValueFrom(actual), "overflow");
}

bool APSRFlagsStatePredicate::getNegativeFlagValueFrom(const mcu& target) {
	return target.get_cpu().special_regs().app_status_register().negative_flag();
}

bool APSRFlagsStatePredicate::getZeroFlagValueFrom(const mcu& target) {
	return target.get_cpu().special_regs().app_status_register().zero_flag();
}

bool APSRFlagsStatePredicate::getCarryFlagValueFrom(const mcu& target) {
	return target.get_cpu().special_regs().app_status_register().carry_flag();
}

bool APSRFlagsStatePredicate::getOverflowFlagValueFrom(const mcu& target) {
	return target.get_cpu().special_regs().app_status_register().overflow_flag();
}

::testing::AssertionResult APSRFlagsStatePredicate::assertEquality(const char*,
																   const char*,
																   const char*,
																   bool expectedFlagStatus,
																   bool actualFlagStatus,
																   const char* flagName) const {

	if(expectedFlagStatus == actualFlagStatus) {
		return ::testing::AssertionSuccess();
	}

	return ::testing::AssertionFailure() << "Equality check fail for APSR " << flagName << " flag" << std::endl
										 << " * Expected: " << flagName << " flag is "
										 << (expectedFlagStatus ? "set" : "clear") << std::endl
										 << " * Actual  : " << flagName << " flag is "
										 << (actualFlagStatus ? "set" : "clear");
}
} // namespace micromachine::testing
