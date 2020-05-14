//
// Created by fla on 26.04.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//



#include "ExceptionStatePredicate.hpp"

namespace micromachine::testing {

ExceptionStatePredicate::ExceptionStatePredicate(micromachine::system::exception exception)
	: _exception(exception) {}

std::string ExceptionStatePredicate::exceptionDetailedName(micromachine::system::exception e) {
	return e.name() + " (number=" + std::to_string(e) + ")";
}

} // namespace micromachine::testing
