
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

	bool wait(uint64_t timeout_micros = 0) {

		if(!_flag) {
			std::unique_lock<std::mutex> lock(_mutex);

			if(_interrupted) {
				return false;
			}

			if(timeout_micros) {

				bool timed_out = _signal.wait_for(lock, std::chrono::microseconds(timeout_micros), [this]() {
					return (_interrupted || _flag);
				});

				if(!timed_out) {
					return false;
				}

			} else {
				_signal.wait(lock, [this]() { return (_interrupted || _flag); });
			}
		}

		_flag = false;

		return !_interrupted;
	}
};

}
