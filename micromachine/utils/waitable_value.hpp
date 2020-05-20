
//
// Created by fla on 04.05.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#pragma once

#include <condition_variable>

namespace micromachine::system {

template <typename T>
class waitable_value {
protected:
	T _value;
	bool _interrupted;

	std::condition_variable _signal;
	std::mutex _mutex;

	void set(T value) {
		std::unique_lock<std::mutex> lock(_mutex);
		_value = value;
		_signal.notify_all();
	}

public:

	enum result {
		ok,
		interrupted,
		timeout
	};

	explicit waitable_value(T initial_value)
		: _value(initial_value)
		, _interrupted(false) {}

	void interrupt() {
		_interrupted = true;
		_signal.notify_all();
	}

	/**
	 * Waits indefinitely for the internal flag value to become /p value.
	 */
	result wait(T value) {
		return wait(value, std::chrono::duration_values<std::chrono::milliseconds>::zero());
	}

	/**
	 * Waits for flag value in a polling fashion so that the value is guaranteed to be compared at least
	 * once every interval even when it has not been notified. This method is useful when waiting on a
	 * condition signaled by a notify event that can potentially be lost (this can typically happen when
	 * the event is notified outside of the wait mutex, usually for performance reasons).
	 * @param value
	 * @param interval
	 * @return
	 */
	template <typename _Rep, typename _Period>
	result polling_wait(T value, const std::chrono::duration<_Rep, _Period>& interval) {
		while(true) {
			auto res = wait(value, interval);
			if(res != result::timeout) {
				return res;
			}
		}
	}

	/**
	 * Waits for the internal flag value to become /p value within the given timeout duration.
	 * @param value The value to wait for
	 * @param timeout_duration The timeout duration
	 * @return result::interrupted if the operation has been interrupted by another thread, result::ok if the
	 * flag has been set to value in time, result::timeout otherwise.
	 */
	template <typename _Rep, typename _Period>
	result wait(T value, const std::chrono::duration<_Rep, _Period>& timeout_duration) {

		if(_value != value) {
			std::unique_lock<std::mutex> lock(_mutex);

			if(_interrupted) {
				return result::interrupted;
			}

			if(timeout_duration > std::chrono::duration<_Rep, _Period>::zero()) {

				bool equal_or_interrupted = _signal.wait_for(lock, timeout_duration, [this, value]() {
					return (_interrupted || (_value == value));
				});

				if(!equal_or_interrupted) {
					return result::timeout;
				}

			} else {
				_signal.wait(lock, [this, value]() { return (_interrupted || (_value == value)); });
			}
		}

		return _interrupted ? result::interrupted : result::ok;
	}
};

}
