#pragma once

#include "mcu_foward_decl.hpp"
#include "exception/exception.hpp"

#include <gtest/gtest.h>

namespace micromachine::testing {

class IPSRStatePredicate {
protected:
	const micromachine::system::exception _expectedException;

public:
	IPSRStatePredicate(micromachine::system::exception expectedException);
	IPSRStatePredicate(const micromachine::system::mcu& expected);
	void apply(micromachine::system::mcu& expected);
	void check(const micromachine::system::mcu& actual) const;

private:
	static micromachine::system::exception getValueFrom(const micromachine::system::mcu& target);
	::testing::AssertionResult assertEquality(const char*,
											  const char*,
											  micromachine::system::exception expectedException,
											  micromachine::system::exception actualException) const;
	static std::string exceptionName(micromachine::system::exception e);
};

} // namespace micromachine::testing
