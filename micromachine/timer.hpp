/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_TIMER_HPP
#define MICROMACHINE_EMU_TIMER_HPP

#include "registers/system_control/systick.hpp"
#include "interrupter.hpp"

class systick {
public:

	systick(interrupter& interrupter)
		: _interrupter(interrupter)
		, _current_value(_control) {

	}

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

		//fprintf(stderr, "sistick current=%d, reload=%d\n", (uint32_t)_current_value, (uint32_t)_reload_value);

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
				_interrupter.raise_systick();
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

private:
	interrupter& _interrupter;
	systick_control_reg _control;
	systick_current_value_reg _current_value;
	systick_reload_value_reg _reload_value;
	systick_calib_value_reg _calib_value;

};

#endif //MICROMACHINE_EMU_TIMER_HPP