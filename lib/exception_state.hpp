/* Copyright (C) MindMaze SA - 2016 - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of MindMaze SA.

*/

#ifndef MICROMACHINE_EMU_EXCEPTION_STATE_HPP_HPP
#define MICROMACHINE_EMU_EXCEPTION_STATE_HPP_HPP

#include "exception_type.hpp"

class exception_state {
public:
	exception_state(const exception_type& ex_type)
		: _type(ex_type)
		, _active(false)
		, _pending(false) {
		}

	void raise() {
		_pending = true;
	}

	bool pending() const {
		return _pending;
	}

	bool active() const {
		return _active;
	}

	void activate() {
		_pending = false;
		_active = true;
	}

	void deactivate() {
		_active = false;
	}

	exception_number number() const {
		return exception_number::from_type(_type);
	}

	exception_type type() const {
		return _type;
	}

	void clear() {
		_active = _pending = false;
	}

private:
	const exception_type _type;
	bool _active;
	bool _pending;
};

class prioritized_exception {
	public:

		prioritized_exception(exception_state* exception_state, int8_t priority)
			: _exception_state(exception_state)
			, _priority(priority)
			, _default_priority(priority) {

		}

		int8_t priority() const {
			return _priority;
		}

		void set_priority(int8_t priority) {
			_priority = priority;
		}

		// used to sort exception to handle. lower is better
		int32_t priority_score() const {
			bool not_pending = !_exception_state->pending();
			// ultra-low priority if not pending
			int32_t malus = (0xFFFF * not_pending);
			return priority() + malus;
		}

		exception_state& state() {
			return *_exception_state;
		}

		const exception_state& state() const {
			return *_exception_state;
		}

		void reset() {
			_priority = _default_priority;
			_exception_state->clear();
		}

	private:
		exception_state* _exception_state;
		int8_t _priority;
		int8_t _default_priority;

};

#endif //MICROMACHINE_EMU_EXCEPTION_STATE_HPP_HPP
