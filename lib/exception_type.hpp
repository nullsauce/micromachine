//
// Created by fla on 17-5-20.
//

#ifndef MICROMACHINE_EXCEPTION_HPP
#define MICROMACHINE_EXCEPTION_HPP

#include <cstddef>
#include <vector>
#include <functional>
#include <queue>
#include <algorithm>
#include <memory>

using exception_number_t = uint8_t;

enum class exception_type : exception_number_t;

struct exception_number {
	exception_number_t _number;
	static exception_number from_uint(uint32_t number) {
		return { static_cast<uint8_t>(number) };
	}
	static exception_number from_type(exception_type type) {
		return { static_cast<exception_number_t>(type) };
	}
	operator exception_number_t() const {
		return _number;
	}
	operator exception_type() const {
		return static_cast<exception_type>(_number);
	}
	operator uint32_t() const {
		return static_cast<uint32_t>(_number);
	}
};


enum class exception_type : exception_number_t {
	INVALID = 0,
	RESET	= 1,
	NMI		= 2,
	HARDFAULT = 3,

	// 4-10 reserved
	_RESERVED_0 = 4,
	_RESERVED_1 = 5,
	_RESERVED_2 = 6,
	_RESERVED_3 = 7,
	_RESERVED_4 = 8,
	_RESERVED_5 = 9,
	_RESERVED_6 = 10,

	SVCALL	= 11,

	// 12-13 reserved
	_RESERVED_7 = 12,
	_RESERVED_8 = 13,

	PENDSV = 14,
	SYSTICK = 15,

	IRQ_0 = 16,
	IRQ_1 = 17,
	IRQ_2 = 18,
	IRQ_3 = 19,
	IRQ_4 = 20,
	IRQ_5 = 21,
	IRQ_6 = 22,
	IRQ_7 = 23,
	IRQ_8 = 24,
	IRQ_9 = 25,
	IRQ_10 = 26,
	IRQ_11 = 27,
	IRQ_12 = 28,
	IRQ_13 = 29,
	IRQ_14 = 30,
	IRQ_15 = 31,
};

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



#endif //MICROMACHINE_EXCEPTION_HPP
