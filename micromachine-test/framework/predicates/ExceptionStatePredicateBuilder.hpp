
//
// Created by fla on 28.04.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#pragma once

#include "ExceptionStatePredicate.hpp"
#include "ExceptionActiveStatePredicate.hpp"
#include "ExceptionPendingStatePredicate.hpp"
#include "ExceptionPriorityStatePredicate.hpp"

namespace micromachine::testing {

class ExceptionStatePredicateBuilder : ExceptionStatePredicate {
public:
	using ExceptionStatePredicate::ExceptionStatePredicate;

	ExceptionActiveStatePredicate hasActiveStatusOf(bool state) {
		return {_exception, state};
	}

	ExceptionPendingStatePredicate hasPendingStatusOf(bool state) {
		return {_exception, state};
	}

	ExceptionPriorityStatePredicate hasPriorityOf(exception::priority_t priority) {
		return {_exception, priority};
	}

	ExceptionActiveStatePredicate isActive() {
		return hasActiveStatusOf(true);
	}

	ExceptionActiveStatePredicate isNotActive() {
		return hasActiveStatusOf(false);
	}

	ExceptionPendingStatePredicate isPending() {
		return hasPendingStatusOf(true);
	}

	ExceptionPendingStatePredicate isNotPending() {
		return hasPendingStatusOf(false);
	}
};

}
