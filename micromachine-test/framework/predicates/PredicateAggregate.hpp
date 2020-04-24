
//
// Created by fla on 27.04.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#pragma once

#include "mcu.hpp"

#include <gtest/gtest.h>

namespace micromachine::testing {

template <typename... Predicates>
class PredicateAggregate {
protected:
	std::tuple<Predicates...> _predicates;

public:
	PredicateAggregate(Predicates... predicates)
		: _predicates(std::forward<Predicates>(predicates)...) {}

	void apply(mcu& expected) {
		std::apply([&](auto&&... predicate) { (predicate.apply(expected), ...); }, _predicates);
	}

	void check(const mcu& actual) const {
		std::apply([&](auto&&... predicate) {
			(predicate.check(actual), ...);
		}, _predicates);
	}
};
} // namespace micromachine::testing
