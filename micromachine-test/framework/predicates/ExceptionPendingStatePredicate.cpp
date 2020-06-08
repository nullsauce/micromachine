#include "ExceptionPendingStatePredicate.hpp"
#include "mcu.hpp"

namespace micromachine::testing {

using namespace micromachine::system;

ExceptionPendingStatePredicate::ExceptionPendingStatePredicate(exception exceptionType,
																		bool expectedPendingState)
	: ExceptionStatePredicate(exceptionType)
	, _expectedPendingState(expectedPendingState) {}

ExceptionPendingStatePredicate::ExceptionPendingStatePredicate(exception exceptionType, const mcu& expected)
	: ExceptionPendingStatePredicate(exceptionType, expected.exceptions().is_pending(exceptionType)) {}

void ExceptionPendingStatePredicate::apply(mcu& expected) {
	expected.exceptions().set_pending(_exception, _expectedPendingState);
}

void ExceptionPendingStatePredicate::check(const mcu& actual) const {
	EXPECT_PRED_FORMAT2(assertEquality, _expectedPendingState, actual.exceptions().is_pending(_exception));
}

::testing::AssertionResult ExceptionPendingStatePredicate::assertEquality(const char*,
																				   const char*,
																				   bool expectedPendingState,
																				   bool actualPendingState) const {

	if(expectedPendingState == actualPendingState) {
		return ::testing::AssertionSuccess();
	}

	return ::testing::AssertionFailure()
		<< "Equality check fail for pending status of Exception " << exceptionDetailedName(_exception) << std::endl
		<< " * Expected: " << _exception.name() << " is "
		<< (expectedPendingState ? "pending" : "NOT pending") << std::endl
		<< " * Actual  : " << _exception.name() << " is "
		<< (actualPendingState ? "pending" : "NOT pending") << std::endl;
}
} // namespace micromachine::testing
