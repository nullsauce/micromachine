/* Copyright (C) The Micromachine project - 2016 - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of The Micromachine project.

*/

#ifndef MICROMACHINE_EMU_INTERRUPT_MANAGER_HPP
#define MICROMACHINE_EMU_INTERRUPT_MANAGER_HPP

#include <list>
#include <memory>

#include "nvic.hpp"


class InterruptState
{

private:
	bool _pending;
	bool _active;

public:
	using priority_t  = uint8_t;

	virtual priority_t priority() const = 0;

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

	void set_pending()
	{
		_pending = true;
	}

	void clear_pending()
	{
		_pending = false;
	}
};

template<int8_t Priority>
class FixedPriorityInterruptState : public InterruptState {
public:
	priority_t priority() const override {
		return Priority;
	}
};

class Shpr2BasedInterruptState : public InterruptState {
protected:
	const shpr2_reg& _reg;

public:
	Shpr2BasedInterruptState(const shpr2_reg& reg)
		: _reg(reg)
	{}
};

class Shpr3BasedInterruptState : public InterruptState {
protected:
	const shpr3_reg& _reg;

public:
	Shpr3BasedInterruptState(const shpr3_reg& reg)
		: _reg(reg)
	{}
};

class Pri11InterruptState : public Shpr2BasedInterruptState {
public:
	using Shpr2BasedInterruptState::Shpr2BasedInterruptState;
	priority_t priority() const override {
		return _reg.pri11();
	}
};

class Pri14InterruptState : public Shpr3BasedInterruptState {
public:
using Shpr3BasedInterruptState::Shpr3BasedInterruptState;
	priority_t priority() const override {
		return _reg.pri14();
	}
};

class Pri15InterruptState : public Shpr3BasedInterruptState {
public:
	using Shpr3BasedInterruptState::Shpr3BasedInterruptState;
	priority_t priority() const override {
		return _reg.pri15();
	}
};

template<size_t ExternalInterruptNumber>
class NvicBasedInterruptState : public InterruptState {
private:
	const nvic& _nvic;
public:
	NvicBasedInterruptState(const nvic& nvic)
		: _nvic(nvic)
	{}

	priority_t priority() const override {
		return _nvic.external_interrupt_priority(ExternalInterruptNumber);
	}
};

class NonImplementedInterruptState : public FixedPriorityInterruptState<4> {

};


struct InterruptVector {

	InterruptVector(nvic& nvic, shpr2_reg& sph2, shpr3_reg& sph3)
		: svc(sph2)
		, pend_sv(sph3)
		, sys_tick(sph3)
		, ext_interrupt_0(nvic)
		, ext_interrupt_1(nvic)
		, ext_interrupt_2(nvic)
		, ext_interrupt_3(nvic)
		, ext_interrupt_4(nvic)
		, ext_interrupt_5(nvic)
		, ext_interrupt_6(nvic)
		, ext_interrupt_7(nvic)
		, ext_interrupt_8(nvic)
		, ext_interrupt_9(nvic)
		, ext_interrupt_10(nvic)
		, ext_interrupt_11(nvic)
		, ext_interrupt_12(nvic)
		, ext_interrupt_13(nvic)
		, ext_interrupt_14(nvic)
		, ext_interrupt_15(nvic)
	{

	}

	FixedPriorityInterruptState<-3> reset;
	FixedPriorityInterruptState<-2> nmi;
	FixedPriorityInterruptState<-1> hard_fault;
	NonImplementedInterruptState _reserved_0;
	NonImplementedInterruptState _reserved_1;
	NonImplementedInterruptState _reserved_2;
	NonImplementedInterruptState _reserved_3;
	NonImplementedInterruptState _reserved_4;
	NonImplementedInterruptState _reserved_5;
	NonImplementedInterruptState _reserved_6;
	Pri11InterruptState svc;
	NonImplementedInterruptState _reserved_7;
	NonImplementedInterruptState _reserved_8;
	Pri14InterruptState pend_sv;
	Pri15InterruptState sys_tick;
	NvicBasedInterruptState<0> ext_interrupt_0;
	NvicBasedInterruptState<1> ext_interrupt_1;
	NvicBasedInterruptState<2> ext_interrupt_2;
	NvicBasedInterruptState<3> ext_interrupt_3;
	NvicBasedInterruptState<4> ext_interrupt_4;
	NvicBasedInterruptState<5> ext_interrupt_5;
	NvicBasedInterruptState<6> ext_interrupt_6;
	NvicBasedInterruptState<7> ext_interrupt_7;
	NvicBasedInterruptState<8> ext_interrupt_8;
	NvicBasedInterruptState<9> ext_interrupt_9;
	NvicBasedInterruptState<10> ext_interrupt_10;
	NvicBasedInterruptState<11> ext_interrupt_11;
	NvicBasedInterruptState<12> ext_interrupt_12;
	NvicBasedInterruptState<13> ext_interrupt_13;
	NvicBasedInterruptState<14> ext_interrupt_14;
	NvicBasedInterruptState<15> ext_interrupt_15;

	std::array<std::reference_wrapper<InterruptState>, 31> indexed = {{
		reset,
		nmi,
		hard_fault,
		_reserved_0,
		_reserved_1,
		_reserved_2,
		_reserved_3,
		_reserved_4,
		_reserved_5,
		_reserved_6,
		svc,
		_reserved_7,
		_reserved_8,
		pend_sv,
		sys_tick,
		ext_interrupt_0,
		ext_interrupt_1,
		ext_interrupt_2,
		ext_interrupt_3,
		ext_interrupt_4,
		ext_interrupt_5,
		ext_interrupt_6,
		ext_interrupt_7,
		ext_interrupt_8,
		ext_interrupt_9,
		ext_interrupt_10,
		ext_interrupt_11,
		ext_interrupt_12,
		ext_interrupt_13,
		ext_interrupt_14,
		ext_interrupt_15
	}};
};


class interrupt_manager
{

private:

	static constexpr InterruptState::priority_t DEFAULT_PRIORITY = 4;

	// Reference to memory to access interrupt vector table
	memory& _mem;

	InterruptVector _interrupt_vector;

	// priority of the current instruction stream
	InterruptState::priority_t _current_priority;

	// NVIC (external interrupt handler)
	nvic _nvic;



public:

	enum Type : uint32_t
	{
		INVALID = 0,
		RESET = 1,
		NMI = 2,
		HARDFAULT = 3,

		// 4-10 reserved
		_RESERVED_0 = 4,
		_RESERVED_1 = 5,
		_RESERVED_2 = 6,
		_RESERVED_3 = 7,
		_RESERVED_4 = 8,
		_RESERVED_5 = 9,
		_RESERVED_6 = 10,

		SVCALL = 11,

		// 12-13 reserved
		_RESERVED_7 = 12,
		_RESERVED_8 = 13,

		PENDSV = 14,
		SYSTICK = 15,
	};

	interrupt_manager(memory& mem, nvic& nvic, shpr2_reg& sph2, shpr3_reg& sph3)
		: _mem(mem)
		, _interrupt_vector(nvic, sph2, sph3) {
	}


	uint32_t interrupt_handler(Type exception_number) const {
		uint32_t interrupt_vector_table_offset = sizeof(uint32_t) * exception_number;
		return _mem.read32(interrupt_vector_table_offset);
	}

	InterruptState &interrupt_state(Type t)
	{
		return _interrupt_vector.indexed[t];
	}

	const InterruptState &interrupt_state(Type t) const
	{
		return _interrupt_vector.indexed[t];
	}

	void service_pending_interrupt()
	{

	}


};


#endif //MICROMACHINE_EMU_INTERRUPT_MANAGER_HPP
