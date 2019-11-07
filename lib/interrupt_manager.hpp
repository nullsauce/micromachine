/* Copyright (C) MindMaze SA - 2016 - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of MindMaze SA.

*/

#ifndef MICROMACHINE_EMU_INTERRUPT_MANAGER_HPP
#define MICROMACHINE_EMU_INTERRUPT_MANAGER_HPP

#include <list>
#include <memory>

#include "exception_state.hpp"

/*
class interrupt_manager
{

private:

	static constexpr ExceptionState::priority_t DEFAULT_PRIORITY = 4;

	// Reference to memory to access interrupt vector table
	memory& _mem;

	InterruptVector _interrupt_vector;

	// priority of the current instruction stream
	ExceptionState::priority_t _current_priority;

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

	ExceptionState &interrupt_state(Type t)
	{
		return _interrupt_vector.indexed[t];
	}

	const ExceptionState &interrupt_state(Type t) const
	{
		return _interrupt_vector.indexed[t];
	}

	void service_pending_interrupt()
	{

	}


};*/


#endif //MICROMACHINE_EMU_INTERRUPT_MANAGER_HPP
