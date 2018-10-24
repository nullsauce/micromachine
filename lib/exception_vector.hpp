//
// Created by fla on 17-5-20.
//

#ifndef MICROMACHINE_EXCEPTION_VECTOR_HPP
#define MICROMACHINE_EXCEPTION_VECTOR_HPP

#include <array>
#include <list>
#include <stack>
#include "exception_type.hpp"
#include "types.hpp"
#include "precond.hpp"

/// Exception state
class exception_state {
	friend class exception_vector;
	using priority_t = int32_t;
public:
	exception_state(exception_number::ex_name exception_name, priority_t priority = 0)
		: _priority(priority)
		, _number(exception_number::from_name(exception_name))
		, _active(false)
		, _pending(false)
		, _default_priority(priority) {
		}

	priority_t priority() const {
		switch(_number.name()) {
			case exception_number::ex_name::RESET: return -3;
			case exception_number::ex_name::NMI: return -2;
			case exception_number::ex_name::HARDFAULT: return -1;
			case exception_number::ex_name::SVCALL: return 0; // shpr2.11
			case exception_number::ex_name::PENDSV: return 0; // shpr2.14
			case exception_number::ex_name::SYSTICK: return 0; // shpr2.15
			default: if(_number.name() > exception_number::ex_name::SYSTICK) {
				// External interrupts
				// TODO:
				uint32_t reg = (_number.int_value() - 16) / 4;
				uint32_t section = _number.int_value() % 4;
				//_nvic.external_interrupt_priority(_number.int_value() - 16);
				return 0;
			} else {
				// Reserved exceptions
				return 4;
			}
		}
	}

	void set_priority(priority_t priority) {
		_priority = priority;
	}

	bool pending() const {
		return _pending;
	}

	bool active() const {
		return _active;
	}

	exception_number number() const {
		return _number;
	}

	void clear() {
		_priority = _default_priority;
		_active = _pending = false;
	}

	bool is_lower_priority(const exception_state& other) {
		if(_priority == other._priority) {
			return _number < other._number;
		} else if(_priority < other._priority) {
			return true;
		} else {
			return false;
		}
	}

	bool operator==(const exception_state& other) const {
		return _number == other._number;
	}

private:

	void raise() {
		_pending = true;
	}

	void activate() {
		_pending = false;
		_active = true;
	}

	void deactivate() {
		_active = false;
	}

	priority_t _priority;
	const exception_number _number;
	bool _active;
	bool _pending;
	priority_t _default_priority;
};



namespace {
	bool pending_exception(const exception_state& ex){
		return ex.pending();
	}

	bool active_exception(const exception_state& ex){
		return ex.active();
	}

}

class exception_vector {

public:
	exception_vector()
		: _exception_states({{
			{exception_number::ex_name::INVALID, 126},
			{exception_number::ex_name::RESET, -3},
			{exception_number::ex_name::NMI, -2},
			{exception_number::ex_name::HARDFAULT, -1},
			{exception_number::ex_name::_RESERVED_0},
			{exception_number::ex_name::_RESERVED_1},
			{exception_number::ex_name::_RESERVED_2},
			{exception_number::ex_name::_RESERVED_3},
			{exception_number::ex_name::_RESERVED_4},
			{exception_number::ex_name::_RESERVED_5},
			{exception_number::ex_name::_RESERVED_6},
			{exception_number::ex_name::SVCALL},
			{exception_number::ex_name::_RESERVED_7},
			{exception_number::ex_name::_RESERVED_8},
			{exception_number::ex_name::PENDSV},
			{exception_number::ex_name::SYSTICK},
			{exception_number::ex_name::IRQ_0},
			{exception_number::ex_name::IRQ_1},
			{exception_number::ex_name::IRQ_2},
			{exception_number::ex_name::IRQ_3},
			{exception_number::ex_name::IRQ_4},
			{exception_number::ex_name::IRQ_5},
			{exception_number::ex_name::IRQ_6},
			{exception_number::ex_name::IRQ_7},
			{exception_number::ex_name::IRQ_8},
			{exception_number::ex_name::IRQ_9},
			{exception_number::ex_name::IRQ_10},
			{exception_number::ex_name::IRQ_11},
			{exception_number::ex_name::IRQ_12},
			{exception_number::ex_name::IRQ_13},
			{exception_number::ex_name::IRQ_14},
			{exception_number::ex_name::IRQ_15}
		}})
		, _active_exception_count(0)
		, _current_priority(0)
	{
		_stream_priority.push(126);
	}

	exception_state::priority_t current_priority() const {
		return _stream_priority.top();
	}

	uint32_t active_exception_count() const {
		return _active_exception_count;
	}

	void raise(exception_number exception) {
		// add exception to pending deque
		auto& exception_state = get_by_number(exception);
		// TODO: THe same exception can't be pending twice
		_pending_exceptions.add_exception(exception_state);
		exception_state.raise();
		//fprintf(stderr, "exception_vector: raised %s\n", exception.str().c_str());
	}

	void activate(exception_number exception) {
		activate(get_by_number(exception));
	}

	void activate(exception_state& exception) {
        precond(exception.priority() < current_priority(), "Only higher priority can be activated");
		// move from pending list to active list
		_pending_exceptions.remove_exception(exception);
		_active_exceptions.add_exception(exception);
		exception.activate();
		_stream_priority.push(exception.priority());
		_active_exception_count++;
		//fprintf(stderr, "exception_vector: activated %s\n", exception.number().str().c_str());
	}

	void deactivate(exception_number ex_number) {
		deactivate(get_by_number(ex_number));
	}

	void deactivate(exception_state& exception) {
		// remove from the pending exception list
		_active_exceptions.remove_exception(exception);
		exception.deactivate();
		_stream_priority.pop();
		// the base priority should always remain
        precond(!_stream_priority.empty(), "cant remove the base priority");
		_active_exception_count--;
		//fprintf(stderr, "exception_vector: deactivated %s\n", exception.number().str().c_str());
	}

	bool is_pending(exception_number ex_number) const {
		return get_by_number(ex_number).pending();
	}

	bool is_active(exception_number ex_number) const {
		return get_by_number(ex_number).active();
	}

	void reset() {
		for(auto& e : _exception_states) {
			e.clear();
		}
	}

	bool any_pending() const {
		return !_pending_exceptions.empty();
	}

	void print() {
		for(const auto& e : _exception_states) {
			fprintf(stderr, "%d (%d) p=%i, a=%i\n",  (int)e.number(), e.priority(), e.pending(), e.active());
		}
	}

	exception_state* top_pending_exception() {
		return _pending_exceptions.top_pending();
	}



private:

	exception_state* get_ptr_by_number(exception_number ex_number) {
		return &_exception_states[ex_number];
	}

	exception_state& get_by_number(exception_number ex_number) {
		return _exception_states.at(ex_number);
	}

	const exception_state& get_by_number(exception_number ex_number) const {
		return _exception_states.at(ex_number);
	}

	std::array<exception_state, 32> _exception_states;

	class exception_list {
		public:
		void add_exception(exception_state& exception) {
			_exceptions.push_back(std::ref(exception));
		}

		void remove_exception(exception_state& exception) {
			_exceptions.remove_if([&](const std::reference_wrapper<exception_state>& e){
				return e.get() == exception;
			});
		}

		private:
		std::list<std::reference_wrapper<exception_state>> _exceptions;
	};

	class prioritized_exception_list {

	public:
		/// inserts an exception before the first exception that
		/// has a lower priority integer value (=higher priority)
		void add_exception(exception_state& exception) {
			auto insert_it = std::upper_bound(_exceptions.begin(), _exceptions.end(), exception.priority(), []
			(exception_state::priority_t prio, const exception_state& e) {
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

		template <typename filterFn>
		exception_state* top(filterFn fn) {
			auto it = std::find_if(_exceptions.begin(), _exceptions.end(), fn);
			if(_exceptions.end() != it) {
				return &(it->get());
			} else {
				return nullptr;
			}
		}

		exception_state* top_pending() {
			return top(pending_exception);
		}


private:
		std::list<std::reference_wrapper<exception_state>> _exceptions;
	};

	prioritized_exception_list _pending_exceptions;
	exception_list _active_exceptions;
	uint32_t _active_exception_count;
	exception_state::priority_t _current_priority;
	std::stack<exception_state::priority_t> _stream_priority;
};

#endif //MICROMACHINE_EXCEPTION_VECTOR_HPP
