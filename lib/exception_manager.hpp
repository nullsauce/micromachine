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

	bool process_pending_exception(uint32_t current_addr, instruction_pair instruction, uint32_t next_instruction_address) {

		ExceptionState* pending_exception = _exception_vector.top_pending();
		if(nullptr == pending_exception) {
			// no exceptions to process
			return false;
		}

		// compute the current execution priority by looking at all active exceptions
		ExceptionState::priority_t current_execution_priority = _exception_vector.current_execution_priority();
		ExceptionState::priority_t pending_exception_priority = pending_exception->priority();

		// do we need to switch the context to a new exception ?
		if(pending_exception_priority < current_execution_priority) {
			// instruction flow must be interrupted by this higher priority exception
			_regs.set_pc(current_addr);
			ctx_switcher.exception_entry(*pending_exception, current_addr, instruction, next_instruction_address);
			return true;
		}

		return false;
	}

private:
	ExceptionStateVector& _exception_vector;
	registers& _regs;

public:
	// this variable is public because its address is needed before all objects are properly initialized
	context_switcher ctx_switcher;

};

#endif //MICROMACHINE_EMU_EXCEPTION_MANAGER_HPP
