
//
// Created by fla on 04.05.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#pragma once

#include <condition_variable>

namespace micromachine::system {

class interruptible_signal {
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

	interruptible_signal(bool flag)
		: _flag(flag)
		, _interrupted(false) {}

	interruptible_signal()
		: interruptible_signal(false) {}

	void set() {
		_flag = true;
		_signal.notify_one();
	}

	void interrupt() {
		_interrupted = true;
		set();
	}

	result wait() {
		return wait(std::chrono::duration_values<std::chrono::milliseconds>::zero());
	}

	template <typename _Rep, typename _Period>
	result wait(const std::chrono::duration<_Rep, _Period>& timeout_duration) {

		if(!_flag) {
			std::unique_lock<std::mutex> lock(_mutex);

			if(_interrupted) {
				return result::interrupted;
			}

			if(timeout_duration > std::chrono::duration<_Rep, _Period>::zero()) {

				bool not_timed_out = _signal.wait_for(lock, timeout_duration, [this]() {
					return (_interrupted || _flag);
				});

				if(!not_timed_out) {
					return result::timeout;
				}

			} else {
				_signal.wait(lock, [this]() { return (_interrupted || _flag); });
			}
		}

		_flag = false;

		return _interrupted ? result::interrupted : result::ok;
	}
};

}
