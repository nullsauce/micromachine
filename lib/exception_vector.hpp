//
// Created by fla on 17-5-20.
//

#ifndef MICROMACHINE_EXCEPTION_VECTOR_HPP
#define MICROMACHINE_EXCEPTION_VECTOR_HPP

#include <array>
#include "exception_type.hpp"
#include "types.hpp"


class exception_vector {

public:
	exception_vector()
		: _ex_state({{
			{exception_type::INVALID},
			{exception_type::RESET},
			{exception_type::NMI},
			{exception_type::HARDFAULT},
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
		, _exceptions({
			{get_ptr_by_type(exception_type::RESET), -3},
			{get_ptr_by_type(exception_type::NMI), -2},
			{get_ptr_by_type(exception_type::HARDFAULT), -1},
			{get_ptr_by_type(exception_type::SVCALL), 1},
			{get_ptr_by_type(exception_type::PENDSV), 1},
			{get_ptr_by_type(exception_type::SYSTICK), 1},
			{get_ptr_by_type(exception_type::IRQ_0), 1},
			{get_ptr_by_type(exception_type::IRQ_1), 1},
			{get_ptr_by_type(exception_type::IRQ_2), 1},
			{get_ptr_by_type(exception_type::IRQ_3), 1},
			{get_ptr_by_type(exception_type::IRQ_4), 1},
			{get_ptr_by_type(exception_type::IRQ_5), 1},
			{get_ptr_by_type(exception_type::IRQ_6), 1},
			{get_ptr_by_type(exception_type::IRQ_7), 1},
			{get_ptr_by_type(exception_type::IRQ_8), 1},
			{get_ptr_by_type(exception_type::IRQ_9), 1},
			{get_ptr_by_type(exception_type::IRQ_10), 1},
			{get_ptr_by_type(exception_type::IRQ_11), 1},
			{get_ptr_by_type(exception_type::IRQ_12), 1},
			{get_ptr_by_type(exception_type::IRQ_13), 1},
			{get_ptr_by_type(exception_type::IRQ_14), 1},
			{get_ptr_by_type(exception_type::IRQ_15), 1}
		})
	{}

	void raise(exception_type type) {
		get_by_type(type).raise();
	}

	void activate(exception_type type) {
		get_by_type(type).activate();
	}

	void deactivate(exception_type type) {
		get_by_type(type).deactivate();
	}

	bool is_pending(exception_type type) const {
		return get_by_type(type).pending();
	}

	bool is_active(exception_type type) const {
		return get_by_type(type).active();
	}

	void reset() {
		for(auto& e : _exceptions) {
			e.reset();
		}
	}

	void prioritize() {
		std::sort(_exceptions.begin(), _exceptions.end(), _comparator);
	}

	bool any_pending() {
		return top_exception().state().pending();
	}

	prioritized_exception& top_exception() {
		return _exceptions.at(0);
	}

	void print() {
		for(const auto& e : _exceptions) {
			fprintf(stderr, "%d [%d] %d\n", e.priority(), e.priority_score(), e.state().number());
		}
	}

private:

	exception_state* get_ptr_by_type(exception_type type) {
		return &_ex_state[static_cast<size_t>(type)];
	}

	exception_state& get_by_type(exception_type type) {
		return _ex_state.at(static_cast<size_t>(type));
	}

	const exception_state& get_by_type(exception_type type) const {
		return _ex_state.at(static_cast<size_t>(type));
	}

	std::array<exception_state, 32> _ex_state;
	std::vector<prioritized_exception> _exceptions;


	struct {
		bool operator()(const prioritized_exception& a, const prioritized_exception& b) {
			return a.priority_score() < b.priority_score();
		}
	} _comparator;
};

#endif //MICROMACHINE_EXCEPTION_VECTOR_HPP
