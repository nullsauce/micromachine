/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_EXCEPTION_MANAGER_HPP
#define MICROMACHINE_EMU_EXCEPTION_MANAGER_HPP


#include "registers/registers.hpp"
#include "instruction_pair.hpp"
#include "memory.hpp"
#include "exception_return_handler.hpp"
#include "context_switcher.hpp"
#include "exception_state.hpp"

class exception_manager {
public:

	exception_manager(registers& regs, memory& mem, ExceptionStateVector& exception_vector)
		: _exception_vector(exception_vector)
		, _regs(regs)
		, ctx_switcher(regs, mem, _exception_vector) {

	}

	void reset() {
		_regs.exec_mode_register().set_thread_mode();
	}

	void process_pending_exception(uint32_t current_addr, instruction_pair instruction, uint32_t next_instruction_address) {
		// Check if this pending exception has priority over the current priority of the
		// instruction stream.
		// Lower priority value means higher priority.
		//bool smaller = _exception_vector.top_pending_exception()->priority() < _exception_vector.current_priority();

		// TODO: Check Priority grouping.
		// When two pending exceptions have the same group priority, the lower pending exception number has
		// priority over the higher pending number as part of the priority precedence rule.
		//if(smaller) {
			// yep, lets activate this exception
		//	_regs.set_pc(current_addr);
		//	context_switcher.exception_entry(*_exception_vector.top_pending_exception(), current_addr, instruction, next_instruction_address);
		//}

	}

private:
	ExceptionStateVector& _exception_vector;
	registers& _regs;

public:
	// this variable is public because its address is needed before all objects are properly initialized
	context_switcher ctx_switcher;

};

#endif //MICROMACHINE_EMU_EXCEPTION_MANAGER_HPP
