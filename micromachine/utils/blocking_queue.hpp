
//
// Created by fla on 30.04.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#pragma once

#include <condition_variable>
#include <mutex>

template <typename T, size_t Size>
class blocking_bounded_channel {
private:
	std::array<T, Size> _array;
	size_t _head;
	size_t _tail;
	std::mutex _mutex;
	std::condition_variable _not_empty_condition;
	std::condition_variable _not_full_condition;
	bool _aborted;

public:
	enum result {
		success,
		timeout,
		interrupted
	};

	blocking_bounded_channel()
	: _head(0)
	, _tail(0)
	, _aborted(false) {}

	size_t capacity() const {
		return Size;
	}

	size_t size() const {
		return _head - _tail;
	}

	bool full() const {
		return size() == capacity();
	}

	bool empty() const {
		return _head == _tail;
	}

	bool aborted() const {
		return _aborted;
	}

	void clear() {
		_head = 0;
		_tail = 0;
	}

	void abort() {
		std::lock_guard<std::mutex> lock(_mutex);
		_aborted = true;
		_not_empty_condition.notify_all();
		_not_full_condition.notify_all();
	}

	result force_push(T value) {
		if(_aborted) {
			return interrupted;
		}

		{
			std::unique_lock<std::mutex> lock(_mutex);
			if(full()) {
				overwrite(value);
			} else {
				write(value);
			}
		}

		_not_empty_condition.notify_one();
		return success;
	}

	result push(T value, size_t timeout_micros = 0) {

		if(_aborted) {
			return interrupted;
		}

		{
			std::unique_lock<std::mutex> lock(_mutex);

			if(full()) {
				if(timeout_micros) {
					const auto duration = std::chrono::microseconds(timeout_micros);
					if(!_not_full_condition.wait_for(lock, duration, [this] { return !full() || aborted(); })) {
						// timed out
						return timeout;
					}
				} else {
					_not_full_condition.wait(lock, [this] { return !full() || aborted(); });
				}
			}
			if(full() || aborted()) {
				return interrupted;
			}
			write(value);
		}

		_not_empty_condition.notify_one();
		return success;
	}

	result pop(T& value, size_t timeout_micros = 0) {

		if(aborted()) {
			return interrupted;
		}

		{
			std::unique_lock<std::mutex> lock(_mutex);
			if(empty()) {
				if(timeout_micros) {
					const auto duration = std::chrono::microseconds(timeout_micros);
					if(!_not_empty_condition.wait_for(lock, duration, [this] { return !empty() || aborted(); })) {
						// timed out
						return timeout;
					}
				} else {
					// call without timeout
					_not_empty_condition.wait(lock, [this] { return !empty() || aborted(); });
				}
			}
			if(empty() || aborted()) {
				return interrupted;
			}
			read(value);
		}

		_not_empty_condition.notify_one();
		return success;
	}

private:
	void write(T value) {
		_array.at(_head % Size) = std::move(value);
		_head = next(_head);
	}

	void read(T& value) {
		value = std::move(_array.at(_tail % Size));
		_tail = next(_tail);
	}

	void overwrite(T value) {
		_tail = next(_tail);
		_array.at(_head % Size) = std::move(value);
		_head = next(_head);
	}

	size_t next(size_t index) const {
		return (index + 1);
	}

};
