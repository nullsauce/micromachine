//
// Created by fla on 25.04.20.
//



#include "IPSRStatePredicate.hpp"
#include "mcu.hpp"

namespace micromachine::testing {

using namespace micromachine::system;

IPSRStatePredicate::IPSRStatePredicate(exception expectedException)
	: _expectedException(expectedException) {}

IPSRStatePredicate::IPSRStatePredicate(const mcu& expected)
	: _expectedException(getValueFrom(expected)) {}

void IPSRStatePredicate::apply(mcu& expected) {
	expected.get_cpu().special_regs().interrupt_status_register().set_exception_number(_expectedException);
}

void IPSRStatePredicate::check(const mcu& actual) const {
	EXPECT_PRED_FORMAT2(assertEquality, _expectedException, getValueFrom(actual));
}

exception IPSRStatePredicate::getValueFrom(const mcu& target) {
	return target.get_cpu().special_regs().interrupt_status_register().exception();
}

::testing::AssertionResult IPSRStatePredicate::assertEquality(const char*,
															  const char*,
															  exception expectedException,
															  exception actualException) const {

	if(expectedException == actualException) {
		return ::testing::AssertionSuccess();
	}

	return ::testing::AssertionFailure() << "Equality check fail for IPSR exception number" << std::endl
										 << " * Expected: IPSR exception number is " << exceptionName(expectedException)
										 << std::endl
										 << " * Actual  : IPSR exception number is " << exceptionName(actualException);
}

std::string IPSRStatePredicate::exceptionName(exception e) {
	return e.name() + " (number=" + std::to_string(e) + ")";
}

} // namespace micromachine::testing
