/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_EXCEPTION_STATE_HPP
#define MICROMACHINE_EMU_EXCEPTION_STATE_HPP


#include <list>
#include <memory>
#include <algorithm>

#include "exception_defs.hpp"
#include "nvic.hpp"
#include "registers/system_control/shpr2.hpp"
#include "registers/system_control/shpr3.hpp"

class exception_state
{

private:
	bool _pending;
	bool _active;
	const exception::Type _number;

public:
	exception_state(exception::Type number)
		: _pending(false)
		, _active(false)
		, _number(number)
	{
	}

	virtual exception::priority_t priority() const = 0;
	virtual void set_priority(exception::priority_t priority) = 0;

	exception::Type number() const {
		return _number;
	}

	bool is_active() const
	{
		return _active;
	}

	void activate()
	{
		_active = true;
		_pending = false;
	}

	void deactivate()
	{
		_active = false;
	}

	bool is_pending() const
	{
		return _pending;
	}

	void set_pending()
	{
		_pending = true;
	}

	void clear_pending()
	{
		_pending = false;
	}

	void reset() {
		clear_pending();
		deactivate();
	}
};

template<int8_t Priority>
class fixed_priority_exception_state : public exception_state {
public:
	using exception_state::exception_state;
	exception::priority_t priority() const override {
		return Priority;
	}

	void set_priority(exception::priority_t priority) override {
		// not supported
		precond(false, "Can't set priority of a fixed priority exception");
	}
};

class shpr2_exception_state : public exception_state {
protected:
	shpr2_reg& _reg;

public:
	shpr2_exception_state(exception::Type number, shpr2_reg& reg)
		: exception_state(number)
		, _reg(reg)
	{}

};

class shpr3_exception_state : public exception_state {
protected:
	shpr3_reg& _reg;

public:
	shpr3_exception_state(exception::Type number, shpr3_reg& reg)
		: exception_state(number)
		, _reg(reg)
	{}
};

class pri11_exception_state : public shpr2_exception_state {
public:
	using shpr2_exception_state::shpr2_exception_state;

	exception::priority_t priority() const override {
		return _reg.pri11();
	}

	void set_priority(exception::priority_t priority) override {
		precond(priority > -1 && priority < 4, "priority not withing range");
		_reg.pri11() = (uint8_t)priority;
	}
};

class pri14_exception_state : public shpr3_exception_state {
public:
	using shpr3_exception_state::shpr3_exception_state;

	exception::priority_t priority() const override {
		return _reg.pri14();
	}

	void set_priority(exception::priority_t priority) override {
		_reg.pri14() = (uint8_t)priority;
	}
};

class pri15_exception_state : public shpr3_exception_state {
public:
	using shpr3_exception_state::shpr3_exception_state;

	exception::priority_t priority() const override {
		return _reg.pri15();
	}

	void set_priority(exception::priority_t priority) override {
		_reg.pri15() = (uint8_t)priority;
	}
};

template<size_t ExternalInterruptNumber>
class nvic_based_exception_state : public exception_state {
private:
	nvic& _nvic;
public:
	nvic_based_exception_state(exception::Type number, nvic& nvic)
		: exception_state(number)
		, _nvic(nvic)
	{}

	exception::priority_t priority() const override {
		return _nvic.priority_bits_for<ExternalInterruptNumber>();
	}

	void set_priority(exception::priority_t priority) override {
		_nvic.priority_bits_for<ExternalInterruptNumber>() = priority;
	}
};

class non_implemented_exception_state : public fixed_priority_exception_state<4> {
public:
	non_implemented_exception_state()
		: fixed_priority_exception_state(exception::INVALID)
	{}

	exception::priority_t priority() const override {
		// not supported
		precond(false, "Can't get priority of an unimplemented exception");
		return 126;
	}

	void set_priority(exception::priority_t priority) override {
		// not supported
		precond(false, "Can't set priority of an unimplemented exception");
	}
};


class exception_state_vector {
public:
	exception_state_vector(const exception_state_vector&) = delete;
	void operator=(const exception_state_vector&) = delete;

	exception_state_vector(nvic& nvic, shpr2_reg& sph2, shpr3_reg& sph3)
		: _reset(exception::Type::RESET)
		, _nmi(exception::Type::NMI)
		, _hard_fault(exception::Type::HARDFAULT)
		, _svc(exception::Type::SVCALL, sph2)
		, _pend_sv(exception::Type::PENDSV, sph3)
		, _sys_tick(exception::Type::SYSTICK, sph3)
		, _ext_interrupt_0(exception::Type::EXTI_00, nvic)
		, _ext_interrupt_1(exception::Type::EXTI_01, nvic)
		, _ext_interrupt_2(exception::Type::EXTI_02, nvic)
		, _ext_interrupt_3(exception::Type::EXTI_03, nvic)
		, _ext_interrupt_4(exception::Type::EXTI_04, nvic)
		, _ext_interrupt_5(exception::Type::EXTI_05, nvic)
		, _ext_interrupt_6(exception::Type::EXTI_06, nvic)
		, _ext_interrupt_7(exception::Type::EXTI_07, nvic)
		, _ext_interrupt_8(exception::Type::EXTI_08, nvic)
		, _ext_interrupt_9(exception::Type::EXTI_09, nvic)
		, _ext_interrupt_10(exception::Type::EXTI_10, nvic)
		, _ext_interrupt_11(exception::Type::EXTI_11, nvic)
		, _ext_interrupt_12(exception::Type::EXTI_12, nvic)
		, _ext_interrupt_13(exception::Type::EXTI_13, nvic)
		, _ext_interrupt_14(exception::Type::EXTI_14, nvic)
		, _ext_interrupt_15(exception::Type::EXTI_15, nvic)
		, _indexed {{
			_used_for_sp,
			_reset,
			_nmi,
			_hard_fault,
			_reserved_0,
			_reserved_1,
			_reserved_2,
			_reserved_3,
			_reserved_4,
			_reserved_5,
			_reserved_6,
			_svc,
			_reserved_7,
			_reserved_8,
			_pend_sv,
			_sys_tick,
			_ext_interrupt_0,
			_ext_interrupt_1,
			_ext_interrupt_2,
			_ext_interrupt_3,
			_ext_interrupt_4,
			_ext_interrupt_5,
			_ext_interrupt_6,
			_ext_interrupt_7,
			_ext_interrupt_8,
			_ext_interrupt_9,
			_ext_interrupt_10,
			_ext_interrupt_11,
			_ext_interrupt_12,
			_ext_interrupt_13,
			_ext_interrupt_14,
			_ext_interrupt_15
		}}
	{}

	size_t highest_accepted_exception_number() {
		return _indexed.size() - 1;
	}

private:
	non_implemented_exception_state _used_for_sp;
	fixed_priority_exception_state<exception::RESET_PRIORITY> _reset;
	fixed_priority_exception_state<exception::NMI_PRIORITY> _nmi;
	fixed_priority_exception_state<exception::HARDFAULT_PRIORITY> _hard_fault;
	non_implemented_exception_state _reserved_0;
	non_implemented_exception_state _reserved_1;
	non_implemented_exception_state _reserved_2;
	non_implemented_exception_state _reserved_3;
	non_implemented_exception_state _reserved_4;
	non_implemented_exception_state _reserved_5;
	non_implemented_exception_state _reserved_6;
	pri11_exception_state _svc;
	non_implemented_exception_state _reserved_7;
	non_implemented_exception_state _reserved_8;
	pri14_exception_state _pend_sv;
	pri15_exception_state _sys_tick;
	// TODO: Map the 16 remaining NVIC interrupts (NVIC 16-31) for a total of 48 exception states
	nvic_based_exception_state<0> _ext_interrupt_0;
	nvic_based_exception_state<1> _ext_interrupt_1;
	nvic_based_exception_state<2> _ext_interrupt_2;
	nvic_based_exception_state<3> _ext_interrupt_3;
	nvic_based_exception_state<4> _ext_interrupt_4;
	nvic_based_exception_state<5> _ext_interrupt_5;
	nvic_based_exception_state<6> _ext_interrupt_6;
	nvic_based_exception_state<7> _ext_interrupt_7;
	nvic_based_exception_state<8> _ext_interrupt_8;
	nvic_based_exception_state<9> _ext_interrupt_9;
	nvic_based_exception_state<10> _ext_interrupt_10;
	nvic_based_exception_state<11> _ext_interrupt_11;
	nvic_based_exception_state<12> _ext_interrupt_12;
	nvic_based_exception_state<13> _ext_interrupt_13;
	nvic_based_exception_state<14> _ext_interrupt_14;
	nvic_based_exception_state<15> _ext_interrupt_15;

	std::array<std::reference_wrapper<exception_state>, 32> _indexed;

public:

	const exception_state& at(size_t index) const {
		return _indexed.at(index);
	}

	template<exception::Type Ex>
	exception_state& interrupt_state() {
		return _indexed[Ex];
	}

	template<exception::Type Ex>
	const exception_state& interrupt_state() const {
		return _indexed[Ex];
	}

	exception_state& interrupt_state(exception::Type t) {
		return _indexed.at(t);
	}

	const exception_state& interrupt_state(exception::Type t) const {
		return _indexed.at(t);
	}

	exception_state& interrupt_state(uint32_t number) {
		return interrupt_state(static_cast<exception::Type>(number));
	}

	const exception_state& interrupt_state(uint32_t number) const {
		return interrupt_state(static_cast<exception::Type>(number));
	}

	size_t active_count() const {
		return std::count_if(std::begin(_indexed), std::end(_indexed), [](const exception_state& e){ return e.is_active(); });
	}

	bool any_active() const {
		return std::any_of(std::begin(_indexed), std::end(_indexed), [](const exception_state& e){ return e.is_active(); });
	}

	bool any_pending() const {
		return std::any_of(std::begin(_indexed), std::end(_indexed), [](const exception_state& e){ return e.is_pending(); });
	}

	exception_state* top_pending() {
		exception_state* top = nullptr;
		// Find the pending exception with the lowest priority
		for(exception_state& e : _indexed) {
			// ignore exception that are not pending
			if(!e.is_pending()) continue;
			// select any exception with lower priority value
			if(top == nullptr || e.priority() < top->priority()) {
				top = &e;
			} else if(e.priority() == top->priority()) {
				// When two pending exceptions have the same group priority, the lower pending exception number has
				// priority over the higher pending number as part of the priority precedence rule
				if(e.number() < top->number()) {
					top = &e;
				}
			}
		}
		return top;
	}

	void reset() {
		for(exception_state& e : _indexed) {
			e.reset();
		}
	}

};


#endif //MICROMACHINE_EMU_EXCEPTION_DEFS_HPP
