
#include "EPSRFlagsStatePredicate.hpp"
#include "mcu.hpp"

namespace micromachine::testing {

using namespace micromachine::system;

EPSRFlagsStatePredicate::EPSRFlagsStatePredicate(bool expectedThumbFlag)
	: _expectedThumbFlag(expectedThumbFlag) {}

EPSRFlagsStatePredicate::EPSRFlagsStatePredicate(const mcu& expected)
	: EPSRFlagsStatePredicate(getThumbFlagValueFrom(expected)) {}

void EPSRFlagsStatePredicate::apply(mcu& expected) {
	expected.get_cpu().special_regs().execution_status_register().thumb_flag() = _expectedThumbFlag;
}

void EPSRFlagsStatePredicate::check(const mcu& actual) const {
	EXPECT_PRED_FORMAT3(assertEquality, _expectedThumbFlag, getThumbFlagValueFrom(actual), "T");
	EXPECT_PRED_FORMAT3(assertEquality, false, getStackAlignFlagValueFrom(actual), "a");
}

bool EPSRFlagsStatePredicate::getThumbFlagValueFrom(const mcu& target) {
	return target.get_cpu().special_regs().execution_status_register().thumb_flag();
}

bool EPSRFlagsStatePredicate::getStackAlignFlagValueFrom(const mcu& target) {
	return target.get_cpu().special_regs().execution_status_register().stack_align_flag();
}

::testing::AssertionResult EPSRFlagsStatePredicate::assertEquality(const char*,
																   const char*,
																   const char*,
																   bool expectedFlagValue,
																   bool actualFlagValue,
																   const char* flagName) const {

	if(expectedFlagValue == actualFlagValue) {
		return ::testing::AssertionSuccess();
	}

	return ::testing::AssertionFailure()
		<< "Equality check fail for EPSR." << flagName << std::endl
		<< " * Expected: EPSR." << flagName << " is " << (expectedFlagValue ? "set" : "clear") << std::endl
		<< " * Actual  : EPSR." << flagName << " is " << (actualFlagValue ? "set" : "clear");
}

}
