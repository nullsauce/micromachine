//
// Created by fla on 25.04.20.
//



#include "PrimaskStatePredicate.hpp"
#include "mcu.hpp"

namespace micromachine::testing {

using namespace micromachine::system;

PrimaskRegStatePredicate::PrimaskRegStatePredicate(bool expectedPrimaskFlag)
	: _expectedPrimaskFlag(expectedPrimaskFlag) {}

PrimaskRegStatePredicate::PrimaskRegStatePredicate(const mcu& expected)
	: PrimaskRegStatePredicate(getPrimaskFlagFrom(expected)) {}

void PrimaskRegStatePredicate::apply(mcu& expected) {
	expected.get_cpu().special_regs().primask_register().pm() = _expectedPrimaskFlag;
}

void PrimaskRegStatePredicate::check(const mcu& actual) const {
	EXPECT_PRED_FORMAT2(assertEquality, _expectedPrimaskFlag, getPrimaskFlagFrom(actual));
}

bool PrimaskRegStatePredicate::getPrimaskFlagFrom(const mcu& target) {
	return target.get_cpu().special_regs().primask_register().pm();
}

::testing::AssertionResult PrimaskRegStatePredicate::assertEquality(const char*,
																	const char*,
																	bool expectedFlagStatus,
																	bool actualFlagStatus) const {

	if(expectedFlagStatus == actualFlagStatus) {
		return ::testing::AssertionSuccess();
	}

	return ::testing::AssertionFailure() << "Equality check fail for PRIMASK.PM flag" << std::endl
										 << " * Expected: PRIMASK.PM flag is " << (expectedFlagStatus ? "set" : "clear")
										 << std::endl
										 << " * Actual  : PRIMASK.PM flag is " << (actualFlagStatus ? "set" : "clear");
}

} // namespace micromachine::testing
