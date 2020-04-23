/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_TIMER_HPP
#define MICROMACHINE_EMU_TIMER_HPP

#include "exception_controller.hpp"
#include "registers/system_control/systick_control_reg.hpp"

namespace micromachine::system {
class systick {
private:
	exception_controller& _exception_controller;
	systick_control_reg _control;
	systick_current_value_reg _current_value;
	systick_reload_value_reg _reload_value;
	systick_calib_value_reg _calib_value;

public:
	systick(exception_controller& exception_controller, const systick& existing_state)
		: _exception_controller(exception_controller)
		, _control(existing_state._control)
		, _current_value(existing_state._current_value)
		, _reload_value(existing_state._reload_value)
		, _calib_value(existing_state._calib_value)
	{}

	systick(exception_controller& exception_controller)
		: _exception_controller(exception_controller)
		, _current_value(_control)
	{}

	void reset() {
		_control.reset();
		_current_value.reset();
		_reload_value.reset();
		_calib_value.reset();
	}

	void tick() {
		if(!_control.enabled()) {
			return;
		}

		if(0U == _current_value) {
			_current_value.set_internal(_reload_value);
			if(0U == _reload_value) {
				_control.set_enabled(false);
				return;
			}
		} else {
			_current_value.decrement();
			if(0U == _current_value) {
				_control.set_count_flag(true);
				_exception_controller.raise_systick();
			}
		}
	}

	systick_control_reg& control_register() {
		return _control;
	}

	systick_current_value_reg& current_value_register() {
		return _current_value;
	}

	systick_reload_value_reg& reload_value_register() {
		return _reload_value;
	}

	systick_calib_value_reg& calib_value_register() {
		return _calib_value;
	}

};

} // namespace micromachine::system

#endif //MICROMACHINE_EMU_TIMER_HPP
