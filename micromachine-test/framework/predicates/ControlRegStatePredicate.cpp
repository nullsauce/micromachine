//
// Created by fla on 25.04.20.
//

#include "ControlRegStatePredicate.hpp"
#include "mcu.hpp"

namespace micromachine::testing {

ControlRegStatePredicate::ControlRegStatePredicate(bool nPrivFlag, bool spSelectionFlag)
	: _expectednPrivFlag(nPrivFlag)
	, _expectedSpSelectionFlag(spSelectionFlag) {}

ControlRegStatePredicate::ControlRegStatePredicate(const mcu& expected)
	: _expectednPrivFlag(getnPrivFlagFrom(expected))
	, _expectedSpSelectionFlag(getSpSelectionFlagFrom(expected)) {}

void ControlRegStatePredicate::apply(mcu& expected) {
	expected.get_cpu().special_regs().control_register().n_priv() = _expectednPrivFlag;
	expected.get_cpu().special_regs().control_register().sp_sel() = _expectedSpSelectionFlag;
}

void ControlRegStatePredicate::check(const mcu& actual) const {
	EXPECT_PRED_FORMAT3(assertEquality, _expectednPrivFlag, getnPrivFlagFrom(actual), "nPRIV");

	EXPECT_PRED_FORMAT3(assertEquality, _expectednPrivFlag, getSpSelectionFlagFrom(actual), "SPSEL");
}

bool ControlRegStatePredicate::getnPrivFlagFrom(const mcu& target) {
	return target.get_cpu().special_regs().control_register().n_priv();
}

bool ControlRegStatePredicate::getSpSelectionFlagFrom(const mcu& target) {
	return target.get_cpu().special_regs().control_register().sp_sel();
}

::testing::AssertionResult ControlRegStatePredicate::assertEquality(const char*,
																	const char*,
																	const char*,
																	bool expectedFlagStatus,
																	bool actualFlagStatus,
																	const char* flagName) const {

	if(expectedFlagStatus == actualFlagStatus) {
		return ::testing::AssertionSuccess();
	}

	return ::testing::AssertionFailure() << "Equality check fail for CONTROL." << flagName << " flag" << std::endl
										 << " * Expected: CONTROL." << flagName << " flag is "
										 << (expectedFlagStatus ? "set" : "clear") << std::endl
										 << " * Actual  : CONTROL." << flagName << " flag is "
										 << (actualFlagStatus ? "set" : "clear");
}
} // namespace micromachine::testing
