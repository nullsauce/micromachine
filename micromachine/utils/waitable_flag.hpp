
//
// Created by fla on 04.05.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#pragma once

#include <condition_variable>

namespace micromachine::system {

class waitable_flag {
private:
	bool _flag;
	bool _interrupted;

	std::condition_variable _signal;
	std::mutex _mutex;

public:

	enum result {
		ok,
		interrupted,
		timeout
	};

	waitable_flag(bool flag)
		: _flag(flag)
		, _interrupted(false) {}

	waitable_flag()
		: waitable_flag(false) {}

	void set(bool value) {
		_flag = value;
		_signal.notify_all();
	}

	void interrupt() {
		_interrupted = true;
		_signal.notify_all();
	}

	result wait(bool value) {
		return wait(value, std::chrono::duration_values<std::chrono::milliseconds>::zero());
	}

	template <typename _Rep, typename _Period>
	result wait(bool value, const std::chrono::duration<_Rep, _Period>& timeout_duration) {

		if(_flag != value) {
			std::unique_lock<std::mutex> lock(_mutex);

			if(_interrupted) {
				return result::interrupted;
			}

			if(timeout_duration > std::chrono::duration<_Rep, _Period>::zero()) {

				bool not_timed_out = _signal.wait_for(lock, timeout_duration, [this, value]() {
					return (_interrupted || (_flag == value));
				});

				if(!not_timed_out) {
					return result::timeout;
				}

			} else {
				_signal.wait(lock, [this]() { return (_interrupted || _flag); });
			}
		}

		return _interrupted ? result::interrupted : result::ok;
	}
};

}
