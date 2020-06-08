#pragma once

#include "exception/exception.hpp"

namespace micromachine::testing {

class ExceptionStatePredicate {
protected:
	const micromachine::system::exception _exception;
	static std::string exceptionDetailedName(micromachine::system::exception e);

public:
	ExceptionStatePredicate(micromachine::system::exception exception);
};

} // namespace micromachine::testing
