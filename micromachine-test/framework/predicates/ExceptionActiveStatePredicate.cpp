//


//



#include "ExceptionActiveStatePredicate.hpp"
#include "mcu.hpp"
#include <gtest/gtest.h>

namespace micromachine::testing {

using namespace micromachine::system;

ExceptionActiveStatePredicate::ExceptionActiveStatePredicate(exception exceptionType, bool expectedActiveState)
	: ExceptionStatePredicate(exceptionType)
	, _expectedActiveState(expectedActiveState) {}

ExceptionActiveStatePredicate::ExceptionActiveStatePredicate(exception exceptionType, const mcu& expected)
	: ExceptionActiveStatePredicate(exceptionType, expected.exceptions().is_active(exceptionType)) {}

void ExceptionActiveStatePredicate::apply(mcu& expected) {
	expected.exceptions().set_active(_exception, _expectedActiveState);
}

void ExceptionActiveStatePredicate::check(const mcu& actual) const {
	EXPECT_PRED_FORMAT2(assertEquality, _expectedActiveState, actual.exceptions().is_active(_exception));
}

::testing::AssertionResult ExceptionActiveStatePredicate::assertEquality(const char*,
																		 const char*,
																		 bool expectedActiveState,
																		 bool actualActiveState) const {

	if(expectedActiveState == actualActiveState) {
		return ::testing::AssertionSuccess();
	}

	return ::testing::AssertionFailure() << "Equality check fail for active status of Exception "
										 << exceptionDetailedName(_exception) << std::endl
										 << " * Expected: " << _exception.name() << " is "
										 << (expectedActiveState ? "active" : "NOT active") << std::endl
										 << " * Actual  : " << _exception.name() << " is "
										 << (actualActiveState ? "active" : "NOT active") << std::endl;
}

} // namespace micromachine::testing
