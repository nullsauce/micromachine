
//
// Created by fla on 04.05.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#pragma once

#include "waitable_flag.hpp"

namespace micromachine::system {

/**
 * A waitable condition is a waitable flag that resolves when set to true and
 * can be reset to false. Initial value can be either true or false.
 */
class waitable_condition : protected waitable_flag {
public:
	waitable_condition()
		: waitable_flag(false) {}

	waitable_condition(bool initial_value)
		: waitable_flag(initial_value) {}

	void set() {
		waitable_flag::set(true);
	}

	void reset() {
		waitable_flag::set(false);
	}

	waitable_flag::result wait() {
		return waitable_flag::wait(true);
	}

	void interrupt() {
		return waitable_flag::interrupt();
	}

	template <typename _Rep, typename _Period>
	waitable_flag::result wait(const std::chrono::duration<_Rep, _Period>& timeout_duration) {
		return waitable_flag::wait(true, timeout_duration);
	}

	template <typename _Rep, typename _Period>
	waitable_flag::result polling_wait(const std::chrono::duration<_Rep, _Period>& interval) {
		return waitable_flag::polling_wait(true, interval);
	}
};

}
