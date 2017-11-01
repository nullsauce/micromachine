/* Copyright (C) MindMaze SA - 2016 - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of MindMaze SA.

*/

#ifndef MICROMACHINE_MODE_REGISTER_HPP
#define MICROMACHINE_MODE_REGISTER_HPP

enum class exec_mode {
	thread,
	handler
};

class exec_mode_register {

public:


	exec_mode_register()
		: _exec_mode(exec_mode::thread) {

	}

	bool is_handler_mode() const {
		return exec_mode::handler == _exec_mode;
	}

	bool is_thread_mode() const {
		return exec_mode::thread == _exec_mode;
	}

	void set_handler_mode() {
		set_mode(exec_mode::handler);
	}

	void set_thread_mode() {
		set_mode(exec_mode::thread);
	}

private:

	void set_mode(exec_mode mode) {
		_exec_mode = mode;
	}

	exec_mode _exec_mode;

};


#endif //MICROMACHINE_MODE_REGISTER_HPP
