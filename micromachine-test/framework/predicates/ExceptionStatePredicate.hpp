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

public:
	ExceptionStatePredicate(exception::Type exceptionType)
		: _exceptionType(exceptionType) {}
};

}
