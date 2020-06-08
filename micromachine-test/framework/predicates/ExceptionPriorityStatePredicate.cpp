#include "ExceptionPriorityStatePredicate.hpp"
#include "mcu.hpp"

namespace micromachine::testing {

using namespace micromachine::system;

ExceptionPriorityStatePredicate::ExceptionPriorityStatePredicate(exception exceptionType,
																 exception::priority_t expectedPendingState)
	: ExceptionStatePredicate(exceptionType)
	, _expectedPriority(expectedPendingState) {}

ExceptionPriorityStatePredicate::ExceptionPriorityStatePredicate(exception exceptionType, const mcu& expected)
	: ExceptionPriorityStatePredicate(exceptionType, expected.exceptions().priority(exceptionType)) {}

void ExceptionPriorityStatePredicate::apply(mcu& expected) {
	expected.exceptions().set_priority(_exception, _expectedPriority);
}

void ExceptionPriorityStatePredicate::check(const mcu& actual) const {
	EXPECT_PRED_FORMAT2(assertEquality, _expectedPriority, actual.exceptions().priority(_exception));
}

::testing::AssertionResult ExceptionPriorityStatePredicate::assertEquality(const char*,
																		   const char*,
																		   exception::priority_t expectedPriority,
																		   exception::priority_t actualPriority) const {

	if(expectedPriority == actualPriority) {
		return ::testing::AssertionSuccess();
	}

	return ::testing::AssertionFailure() << "Equality check fail for priority of Exception "
										 << exceptionDetailedName(_exception) << std::endl
										 << " * Expected: " << _exception.name() << " has a priority of "
										 << (int)expectedPriority << std::endl
										 << " * Actual  : " << _exception.name() << " has a priority of "
										 << (int)actualPriority << std::endl;
}
} // namespace micromachine::testing
