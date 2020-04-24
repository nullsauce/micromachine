//
// Created by fla on 26.04.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#pragma once

#include "exception/exception.hpp"
#include "mcu.hpp"

#include <gtest/gtest.h>

namespace micromachine::testing {

class ExceptionStatePredicate {
protected:
	const exception::Type _exceptionType;

	exception_state& stateOf(mcu& target) {
		return target.get_exception_vector().interrupt_state(_exceptionType);
	}

	const exception_state& stateOf(const mcu& target) const {
		return target.get_exception_vector().interrupt_state(_exceptionType);
	}

public:
	ExceptionStatePredicate(exception::Type exceptionType)
		: _exceptionType(exceptionType) {}
};

}
