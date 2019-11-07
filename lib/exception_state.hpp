/* Copyright (C) MindMaze SA - 2016 - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of MindMaze SA.

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

class ExceptionState
{

private:
	bool _pending;
	bool _active;
	const Exception::Type _number;

public:
	ExceptionState(Exception::Type number)
		: _pending(false)
		, _active(false)
		, _number(number)
	{
	}

	using priority_t  = uint8_t;

	static constexpr priority_t DEFAULT_PRIORITY = 4;

	virtual priority_t priority() const = 0;

	Exception::Type number() const {
		return _number;
	}

	bool is_active() const
	{
		return _active;
	}

	void activate()
	{
		_active = true;
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
class FixedPriorityExceptionState : public ExceptionState {
public:
	using ExceptionState::ExceptionState;
	priority_t priority() const override {
		return Priority;
	}
};

class Shpr2BasedExceptionState : public ExceptionState {
protected:
	const shpr2_reg& _reg;

public:
	Shpr2BasedExceptionState(Exception::Type number, const shpr2_reg& reg)
		: ExceptionState(number)
		, _reg(reg)
	{}
};

class Shpr3BasedExceptionState : public ExceptionState {
protected:
	const shpr3_reg& _reg;

public:
	Shpr3BasedExceptionState(Exception::Type number, const shpr3_reg& reg)
		: ExceptionState(number)
		, _reg(reg)
	{}
};

class Pri11ExceptionState : public Shpr2BasedExceptionState {
public:
	using Shpr2BasedExceptionState::Shpr2BasedExceptionState;
	priority_t priority() const override {
		return _reg.pri11();
	}
};

class Pri14ExceptionState : public Shpr3BasedExceptionState {
public:
using Shpr3BasedExceptionState::Shpr3BasedExceptionState;
	priority_t priority() const override {
		return _reg.pri14();
	}
};

class Pri15ExceptionState : public Shpr3BasedExceptionState {
public:
	using Shpr3BasedExceptionState::Shpr3BasedExceptionState;
	priority_t priority() const override {
		return _reg.pri15();
	}
};

template<size_t ExternalInterruptNumber>
class NvicBasedExceptionState : public ExceptionState {
private:
	const nvic& _nvic;
public:
	NvicBasedExceptionState(Exception::Type number, const nvic& nvic)
		: ExceptionState(number)
		, _nvic(nvic)
	{}

	priority_t priority() const override {
		return _nvic.external_interrupt_priority(ExternalInterruptNumber);
	}
};

class NonImplementedExceptionState : public FixedPriorityExceptionState<4> {
public:
	NonImplementedExceptionState()
		: FixedPriorityExceptionState(Exception::INVALID)
	{}
};


struct ExceptionStateVector {
	ExceptionStateVector(const ExceptionStateVector&) = delete;
	void operator=(const ExceptionStateVector&) = delete;
	ExceptionStateVector(nvic& nvic, shpr2_reg& sph2, shpr3_reg& sph3)
		: _reset(Exception::Type::RESET)
		, _nmi(Exception::Type::NMI)
		, _hard_fault(Exception::Type::HARDFAULT)
		, _svc(Exception::Type::SVCALL, sph2)
		, _pend_sv(Exception::Type::PENDSV, sph3)
		, _sys_tick(Exception::Type::SYSTICK, sph3)
		, _ext_interrupt_0(Exception::Type::EXTI_00, nvic)
		, _ext_interrupt_1(Exception::Type::EXTI_01, nvic)
		, _ext_interrupt_2(Exception::Type::EXTI_02, nvic)
		, _ext_interrupt_3(Exception::Type::EXTI_03, nvic)
		, _ext_interrupt_4(Exception::Type::EXTI_04, nvic)
		, _ext_interrupt_5(Exception::Type::EXTI_05, nvic)
		, _ext_interrupt_6(Exception::Type::EXTI_06, nvic)
		, _ext_interrupt_7(Exception::Type::EXTI_07, nvic)
		, _ext_interrupt_8(Exception::Type::EXTI_08, nvic)
		, _ext_interrupt_9(Exception::Type::EXTI_09, nvic)
		, _ext_interrupt_10(Exception::Type::EXTI_10, nvic)
		, _ext_interrupt_11(Exception::Type::EXTI_11, nvic)
		, _ext_interrupt_12(Exception::Type::EXTI_12, nvic)
		, _ext_interrupt_13(Exception::Type::EXTI_13, nvic)
		, _ext_interrupt_14(Exception::Type::EXTI_14, nvic)
		, _ext_interrupt_15(Exception::Type::EXTI_15, nvic)
		, _indexed {
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
		}
	{}

	NonImplementedExceptionState _used_for_sp;
	FixedPriorityExceptionState<-3> _reset;
	FixedPriorityExceptionState<-2> _nmi;
	FixedPriorityExceptionState<-1> _hard_fault;
	NonImplementedExceptionState _reserved_0;
	NonImplementedExceptionState _reserved_1;
	NonImplementedExceptionState _reserved_2;
	NonImplementedExceptionState _reserved_3;
	NonImplementedExceptionState _reserved_4;
	NonImplementedExceptionState _reserved_5;
	NonImplementedExceptionState _reserved_6;
	Pri11ExceptionState _svc;
	NonImplementedExceptionState _reserved_7;
	NonImplementedExceptionState _reserved_8;
	Pri14ExceptionState _pend_sv;
	Pri15ExceptionState _sys_tick;
	NvicBasedExceptionState<0> _ext_interrupt_0;
	NvicBasedExceptionState<1> _ext_interrupt_1;
	NvicBasedExceptionState<2> _ext_interrupt_2;
	NvicBasedExceptionState<3> _ext_interrupt_3;
	NvicBasedExceptionState<4> _ext_interrupt_4;
	NvicBasedExceptionState<5> _ext_interrupt_5;
	NvicBasedExceptionState<6> _ext_interrupt_6;
	NvicBasedExceptionState<7> _ext_interrupt_7;
	NvicBasedExceptionState<8> _ext_interrupt_8;
	NvicBasedExceptionState<9> _ext_interrupt_9;
	NvicBasedExceptionState<10> _ext_interrupt_10;
	NvicBasedExceptionState<11> _ext_interrupt_11;
	NvicBasedExceptionState<12> _ext_interrupt_12;
	NvicBasedExceptionState<13> _ext_interrupt_13;
	NvicBasedExceptionState<14> _ext_interrupt_14;
	NvicBasedExceptionState<15> _ext_interrupt_15;


	std::reference_wrapper<ExceptionState> const _indexed[32];

	//TODO: create a 'fast' version of the indexed array that omits the reserved field to speed up iterations

	template<Exception::Type Ex>
	ExceptionState& interrupt_state()
	{
		return _indexed[Ex];
	}

	template<Exception::Type Ex>
	const ExceptionState& interrupt_state() const
	{
		return _indexed[Ex];
	}

	ExceptionState& interrupt_state(Exception::Type t)
	{
		ExceptionState& s = _indexed[t];
		return s;
	}

	const ExceptionState& interrupt_state(Exception::Type t) const
	{
		return _indexed[t];
	}

	ExceptionState& interrupt_state(uint32_t number)
	{
		return interrupt_state(static_cast<Exception::Type>(number));
	}

	const ExceptionState& interrupt_state(uint32_t number) const
	{
		return interrupt_state(static_cast<Exception::Type>(number));
	}

	size_t active_count() const {
		return std::count_if(std::begin(_indexed), std::end(_indexed), [](const ExceptionState& e){ return e.is_active(); });
	}

	bool any_active() const {
		return std::any_of(std::begin(_indexed), std::end(_indexed), [](const ExceptionState& e){ return e.is_active(); });
	}

	bool any_pending() const {
		return std::any_of(std::begin(_indexed), std::end(_indexed), [](const ExceptionState& e){ return e.is_pending(); });
	}

	ExceptionState* top_pending() {
		ExceptionState* top = nullptr;
		// Find the pending exception with the lowest priority
		for(ExceptionState& e : _indexed) {
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

	ExceptionState::priority_t current_execution_priority() const {

		ExceptionState::priority_t prio = ExceptionState::DEFAULT_PRIORITY;
		ExceptionState::priority_t boosted_prio = ExceptionState::DEFAULT_PRIORITY;

		for(size_t i = 2; i < 32; i++) {
			ExceptionState& e = _indexed[i];
			if(!e.is_active()) continue;
			if(e.priority() < prio) {
				prio = e.priority();
			}
		}

		// TODO primask lookup
		// if primask.pm
		//   boosted_prio = 0

		if(boosted_prio < prio) {
			return boosted_prio;
		} else {
			return prio;
		}
	}

	void reset() {
		for(ExceptionState& e : _indexed) {
			e.reset();
		}
	}

};


#endif //MICROMACHINE_EMU_EXCEPTION_STATE_HPP
