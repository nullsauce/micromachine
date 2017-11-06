#ifndef MICROMACHINE_MODE_REGISTER_HPP
#define MICROMACHINE_MODE_REGISTER_HPP

enum class execution_mode {
	thread,
	handler
};

class exec_mode_reg {

public:


	exec_mode_reg()
		: _exec_mode(execution_mode::thread) {

	}

	bool is_handler_mode() const {
		return execution_mode::handler == _exec_mode;
	}

	bool is_thread_mode() const {
		return execution_mode::thread == _exec_mode;
	}

	void set_handler_mode() {
		set_mode(execution_mode::handler);
	}

	void set_thread_mode() {
		set_mode(execution_mode::thread);
	}

private:

	void set_mode(execution_mode mode) {
		_exec_mode = mode;
	}

	execution_mode _exec_mode;

};


#endif //MICROMACHINE_MODE_REGISTER_HPP
