
//
// Created by fla on 04.05.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#pragma once

#include "waitable_flag.hpp"

namespace micromachine::system {

class waitable_condition {
private:
	waitable_flag _waitable_flag;

public:
	waitable_condition()
		: _waitable_flag(false) {}

	waitable_condition(bool initial_value)
		: _waitable_flag(initial_value) {}

	void set() {
		_waitable_flag.set(true);
	}

	void reset() {
		_waitable_flag.set(false);
	}

	void interrupt() {
		_waitable_flag.interrupt();
	}

	waitable_flag::result wait() {
		return _waitable_flag.wait(true);
	}

	template <typename _Rep, typename _Period>
	waitable_flag::result wait(const std::chrono::duration<_Rep, _Period>& timeout_duration) {
		return _waitable_flag.wait(true, timeout_duration);
	}

	template <typename _Rep, typename _Period>
	waitable_flag::result preemptible_wait(const std::chrono::duration<_Rep, _Period>& interval) {
		return _waitable_flag.preemptible_wait(true, interval);
	}
};

}
