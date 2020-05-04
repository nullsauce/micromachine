/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#pragma once

#include "exception.hpp"
#include "nvic.hpp"
#include "registers/system_control/shpr2_reg.hpp"
#include "registers/system_control/shpr3_reg.hpp"

#include <algorithm>
#include <list>
#include <memory>

namespace micromachine::system {

class exception_state {
private:
	bool _active;
	const exception::Type _number;

public:
	exception_state(exception::Type number)
		: _active(false)
		, _number(number) {}

	virtual exception::priority_t priority() const = 0;
	virtual void set_priority(exception::priority_t priority) = 0;
	virtual void set_pending(bool pending) = 0;
	virtual bool is_pending() const = 0;
	virtual bool is_enabled() const = 0;
	virtual void set_enable(bool enable) = 0;

	exception::Type number() const {
		return _number;
	}

	bool is_active() const {
		return _active;
	}

	void activate() {
		_active = true;
		set_pending(false);
	}

	void deactivate() {
		_active = false;
	}

	void clear_pending() {
		set_pending(false);
	}

	void reset() {
		clear_pending();
		deactivate();
		set_enable(false);
	}

	void copy_state_from(const exception_state& other) {
		set_active(other.is_active());
		set_priority(other.priority());
		set_pending(other.is_pending());
		set_enable(other.is_enabled());
	}

	void set_active(bool active) {
		_active = active;
	}
};

class internal_exception_state : public exception_state {
private:
	bool _pending;

public:
	internal_exception_state(exception::Type number)
		: exception_state(number)
		, _pending(false) {}

	bool is_pending() const override {
		return _pending;
	}

	void set_pending(bool pending) override {
		_pending = pending;
	}

	bool is_enabled() const override {
		// internal exceptions are always enabled
		return true;
	}

	void set_enable(bool enable) override {
		// Can't disable an internal exception
	}
};

template <int8_t Priority>
class fixed_priority_exception_state : public internal_exception_state {
public:
	using internal_exception_state::internal_exception_state;
	exception::priority_t priority() const override {
		return Priority;
	}

	void set_priority(exception::priority_t priority) override {
		// Can't set priority of a fixed priority exception
	}
};

class shpr2_exception_state : public internal_exception_state {
protected:
	shpr2_reg& _reg;

public:
	shpr2_exception_state(exception::Type number, shpr2_reg& reg)
		: internal_exception_state(number)
		, _reg(reg) {}
};

class shpr3_exception_state : public internal_exception_state {
protected:
	shpr3_reg& _reg;

public:
	shpr3_exception_state(exception::Type number, shpr3_reg& reg)
		: internal_exception_state(number)
		, _reg(reg) {}
};

class pri11_exception_state : public shpr2_exception_state {
public:
	using shpr2_exception_state::shpr2_exception_state;

	exception::priority_t priority() const override {
		return _reg.pri11();
	}

	void set_priority(exception::priority_t priority) override {
		micromachine_check(priority > -1 && priority < 4, "priority not withing range");
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

template <size_t ExternalInterruptNumber>
class nvic_based_exception_state : public exception_state {
private:
	nvic& _nvic;

public:
	nvic_based_exception_state(exception::Type number, nvic& nvic)
		: exception_state(number)
		, _nvic(nvic) {}

	exception::priority_t priority() const override {
		return _nvic.priority_bits_for<ExternalInterruptNumber>();
	}

	void set_priority(exception::priority_t priority) override {
		_nvic.priority_bits_for<ExternalInterruptNumber>() = priority;
	}

	bool is_pending() const override {
		return _nvic.pending_bit_for<ExternalInterruptNumber>();
	}

	void set_pending(bool pending) override {
		_nvic.pending_bit_for<ExternalInterruptNumber>() = pending;
	}

	bool is_enabled() const override {
		return _nvic.enable_bit_for<ExternalInterruptNumber>();
	}

	void set_enable(bool enable) override {
		_nvic.enable_bit_for<ExternalInterruptNumber>() = enable;
	}
};

class non_implemented_exception_state : public fixed_priority_exception_state<4> {
public:
	non_implemented_exception_state()
		: fixed_priority_exception_state(exception::INVALID) {}

	exception::priority_t priority() const override {
		// not supported
		return exception::MAX_PRIORITY;
	}

	void set_priority(exception::priority_t priority) override {
		// not supported
	}
};

class exception_vector {
public:
	// copy constructor
	exception_vector(nvic& nvic,
					 shpr2_reg& sph2,
					 shpr3_reg& sph3,
					 const exception_vector& existing_state)
		: exception_vector(nvic, sph2, sph3) {
		// Initializes everything as usual.
		// Then copy the exception states from the existing state
		for(exception::Type type : _implemented_exception_types) {
			auto& current = _indexed[type].get();
			auto& existing = existing_state._indexed[type].get();
			current.copy_state_from(existing);
		}
	}

	exception_vector(nvic& nvic, shpr2_reg& sph2, shpr3_reg& sph3)
		: _reset(exception::RESET)
		, _nmi(exception::NMI)
		, _hard_fault(exception::HARDFAULT)
		, _svc(exception::SVCALL, sph2)
		, _pend_sv(exception::PENDSV, sph3)
		, _sys_tick(exception::SYSTICK, sph3)
		, _ext_interrupt_0(exception::EXTI_00, nvic)
		, _ext_interrupt_1(exception::EXTI_01, nvic)
		, _ext_interrupt_2(exception::EXTI_02, nvic)
		, _ext_interrupt_3(exception::EXTI_03, nvic)
		, _ext_interrupt_4(exception::EXTI_04, nvic)
		, _ext_interrupt_5(exception::EXTI_05, nvic)
		, _ext_interrupt_6(exception::EXTI_06, nvic)
		, _ext_interrupt_7(exception::EXTI_07, nvic)
		, _ext_interrupt_8(exception::EXTI_08, nvic)
		, _ext_interrupt_9(exception::EXTI_09, nvic)
		, _ext_interrupt_10(exception::EXTI_10, nvic)
		, _ext_interrupt_11(exception::EXTI_11, nvic)
		, _ext_interrupt_12(exception::EXTI_12, nvic)
		, _ext_interrupt_13(exception::EXTI_13, nvic)
		, _ext_interrupt_14(exception::EXTI_14, nvic)
		, _ext_interrupt_15(exception::EXTI_15, nvic)
		, _implemented_exception_types{{
			exception::RESET,
			exception::NMI,
			exception::HARDFAULT,
			exception::SVCALL,
			exception::PENDSV,
			exception::SYSTICK,
			exception::EXTI_00,
			exception::EXTI_01,
			exception::EXTI_02,
			exception::EXTI_03,
			exception::EXTI_04,
			exception::EXTI_05,
			exception::EXTI_06,
			exception::EXTI_07,
			exception::EXTI_08,
			exception::EXTI_09,
			exception::EXTI_10,
			exception::EXTI_11,
			exception::EXTI_12,
			exception::EXTI_13,
			exception::EXTI_14,
			exception::EXTI_15,
		}}
		, _indexed{{_used_for_sp,
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
					_ext_interrupt_15}} {}

	size_t highest_accepted_exception_number() const {
		return _indexed.size() - 1;
	}

	size_t supported_exception_count() const {
		return _indexed.size();
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

	const std::array<exception::Type, 22> _implemented_exception_types;
	std::array<std::reference_wrapper<exception_state>, 32> _indexed;

public:
	const exception_state& at(size_t index) const {
		return _indexed.at(index);
	}

	template <exception::Type Ex>
	exception_state& interrupt_state() {
		return _indexed[Ex];
	}

	template <exception::Type Ex>
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
		return std::count_if(std::begin(_indexed),
							 std::end(_indexed),
							 [](const exception_state& e) { return e.is_active(); });
	}

	bool any_active() const {
		return std::any_of(std::begin(_indexed), std::end(_indexed), [](const exception_state& e) {
			return e.is_active();
		});
	}

	bool any_pending() const {
		return std::any_of(std::begin(_indexed), std::end(_indexed), [](const exception_state& e) {
			return e.is_pending();
		});
	}

	exception_state* top_pending() {
		exception_state* top = nullptr;
		// Find the pending exception with the lowest priority
		for(exception_state& e : _indexed) {
			// ignore exception that are not pending
			if(!e.is_pending())
				continue;

			// ignore exception that are not enabled
			if(!e.is_enabled())
				continue;

			// select any exception with lower priority value
			if(top == nullptr || e.priority() < top->priority()) {
				top = &e;
			} else if(e.priority() == top->priority()) {
				// When two pending exceptions have the same group priority, the lower pending
				// exception number has priority over the higher pending number as part of the
				// priority precedence rule
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

		_svc.set_priority(0);
		_pend_sv.set_priority(0);
		_sys_tick.set_priority(0);
	}
};
} // namespace micromachine::system
