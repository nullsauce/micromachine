

#pragma once

namespace micromachine::system {

class execution_mode {
public:
	enum mode { thread, handler };

	execution_mode(const execution_mode& other)
		: _current_mode(other._current_mode) {}

	execution_mode(mode mode)
		: _current_mode(mode) {}

	execution_mode()
		: execution_mode(thread) {}

	void enter_thread_mode() {
		_current_mode = thread;
	}

	void enter_handler_mode() {
		_current_mode = handler;
	}

	mode current_mode() const {
		return _current_mode;
	}

	void enter_mode(mode mode) {
		_current_mode = mode;
	}

	bool is_in_thread_mode() const {
		return current_mode() == thread;
	}

	bool is_in_handler_mode() const {
		return current_mode() == handler;
	}

private:
	mode _current_mode;
};

} // namespace micromachine::system
