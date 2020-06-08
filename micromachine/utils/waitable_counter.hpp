#pragma once

#include "waitable_value.hpp"

namespace micromachine::system {

/**
 * A waitable counter is an unsigned waitable counter on which
 * threads can wait for a specific value.
 */
class waitable_counter : public waitable_value<size_t> {
public:
	using waitable_value::waitable_value;

	void decrement() {
		std::unique_lock<std::mutex> lock(_mutex);
		_value--;
		_signal.notify_all();
	}

	void increment() {
		std::unique_lock<std::mutex> lock(_mutex);
		_value++;
		_signal.notify_all();
	}

};

}
