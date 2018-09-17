//
// Created by fla on 17-5-20.
//

#ifndef MICROMACHINE_EXCEPTION_VECTOR_HPP
#define MICROMACHINE_EXCEPTION_VECTOR_HPP

#include <array>
#include <list>
#include "exception_state.hpp"
#include "types.hpp"

/// Exception state
class exception_state {
	friend class exception_vector;
public:
	exception_state(const exception_type& ex_type, int8_t priority = 0)
		: _priority(priority)
		, _type(ex_type)
		, _active(false)
		, _pending(false)
		, _default_priority(priority) {
		}

	int8_t priority() const {
		return _priority;
	}

	void set_priority(int8_t priority) {
		_priority = priority;
	}

	bool pending() const {
		return _pending;
	}

	bool active() const {
		return _active;
	}

	exception_number number() const {
		return exception_number::from_type(_type);
	}

	exception_type type() const {
		return _type;
	}

	void clear() {
		_priority = _default_priority;
		_active = _pending = false;
	}

private:

	void raise() {
		//fprintf(stderr, "RAISE %d (%d) %p\n", _type, _priority, this);
		_pending = true;
	}

	void activate() {
		//fprintf(stderr, "ACTIVATE %d (%d)\n", _type, _priority);
		_pending = false;
		_active = true;
	}

	void deactivate() {
		//fprintf(stderr, "DEACTIVATE %d (%d)\n", _type, _priority);
		_active = false;
	}

	int8_t _priority;
	const exception_type _type;
	bool _active;
	bool _pending;
	int8_t _default_priority;
};

class exception_vector {

public:
	exception_vector()
		: _exception_states({{
			{exception_type::INVALID, 126},
			{exception_type::RESET, -3},
			{exception_type::NMI, -2},
			{exception_type::HARDFAULT, -1},
			{exception_type::_RESERVED_0},
			{exception_type::_RESERVED_1},
			{exception_type::_RESERVED_2},
			{exception_type::_RESERVED_3},
			{exception_type::_RESERVED_4},
			{exception_type::_RESERVED_5},
			{exception_type::_RESERVED_6},
			{exception_type::SVCALL},
			{exception_type::_RESERVED_7},
			{exception_type::_RESERVED_8},
			{exception_type::PENDSV},
			{exception_type::SYSTICK},
			{exception_type::IRQ_0},
			{exception_type::IRQ_1},
			{exception_type::IRQ_2},
			{exception_type::IRQ_3},
			{exception_type::IRQ_4},
			{exception_type::IRQ_5},
			{exception_type::IRQ_6},
			{exception_type::IRQ_7},
			{exception_type::IRQ_8},
			{exception_type::IRQ_9},
			{exception_type::IRQ_10},
			{exception_type::IRQ_11},
			{exception_type::IRQ_12},
			{exception_type::IRQ_13},
			{exception_type::IRQ_14},
			{exception_type::IRQ_15}
		}})
		, _active_exception_count(0)
	{}

	uint32_t active_exception_count() const {
		return _active_exception_count;
	}

	void raise(exception_type type) {
		// add exception to pending deque
		auto& exception = get_by_type(type);
		_active_exceptions.add_exception(exception);
		exception.raise();
	}

	void activate(exception_type type) {
		activate(get_by_type(type));
	}

	void activate(exception_state& exception) {
		// mark it as active
		exception.activate();
		_active_exception_count++;
	}

	void deactivate(exception_type type) {
		deactivate(get_by_type(type));
	}

	void deactivate(exception_state& exception) {
		// remove from the pending exception list
		_active_exceptions.remove_exception(exception);
		exception.deactivate();
		_active_exception_count--;
	}

	bool is_pending(exception_type type) const {
		return get_by_type(type).pending();
	}

	bool is_active(exception_type type) const {
		return get_by_type(type).active();
	}

	void reset() {
		for(auto& e : _exception_states) {
			e.clear();
		}
	}

	void prioritize() {
		//std::sort(_exception_queue.begin(), _exception_queue.end(), _comparator);
	}

	bool any_active() const {
		return !_active_exceptions.empty();
		//return top_exception().state().pending();
	}

	void print() {
		for(const auto& e : _exception_states) {
			fprintf(stderr, "%d (%d) p=%i, a=%i\n",  (int)e.number(), e.priority(), e.pending(), e.active());
		}
	}

	exception_state* top_pending_exception() {
		return _active_exceptions.top_pending();
	}

private:

	exception_state* get_ptr_by_type(exception_type type) {
		return &_exception_states[static_cast<size_t>(type)];
	}

	exception_state& get_by_type(exception_type type) {
		return _exception_states.at(static_cast<size_t>(type));
	}

	const exception_state& get_by_type(exception_type type) const {
		return _exception_states.at(static_cast<size_t>(type));
	}

	std::array<exception_state, 32> _exception_states;


	class prioritized_exception_list {
	public:
		/// inserts an exception before the first exception that
		/// has a lower priority integer value (=higher priority)
		void add_exception(exception_state& exception) {
			auto insert_it = std::upper_bound(_exceptions.begin(), _exceptions.end(), exception.priority(), []
			(int8_t prio, const
			exception_state& e) {
				return prio < e.priority();
			});
			_exceptions.insert(insert_it, std::ref(exception));
		}

		void remove_exception(exception_state& exception) {
			const exception_number exception_to_find = exception.number();
			auto it = std::find_if(_exceptions.begin(), _exceptions.end(), [exception_to_find](const exception_state& ex){
				return (uint32_t)exception_to_find == (uint32_t)ex.number();
			});
			if(_exceptions.end() != it) {
				_exceptions.erase(it);
			} else {
				fprintf(stderr, "Exception number %d is NOT present is the pending list", (uint32_t)exception_to_find);
			}
		}

		bool empty() const {
			return _exceptions.empty();
		}

		exception_state& top() {
			return _exceptions.front();
		}

		exception_state* top_pending() {
			auto it = std::find_if(_exceptions.begin(), _exceptions.end(), [](const exception_state& ex){
				return ex.pending();
			});

			if(_exceptions.end() != it) {
				return &(it->get());
			} else {
				return nullptr;
			}
		}

		const exception_state& top() const {
			return _exceptions.front();
		}
private:
		std::list<std::reference_wrapper<exception_state>> _exceptions;
	};

	prioritized_exception_list _active_exceptions;
	uint32_t _active_exception_count;
};

#endif //MICROMACHINE_EXCEPTION_VECTOR_HPP
