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

	void take_exception(ExceptionState& exception_state, uint32_t instruction_address, instruction_pair
	current_instruction, uint32_t next_instruction_address) {
		_regs.set_pc(instruction_address);
		ctx_switcher.exception_entry(exception_state, instruction_address, current_instruction, next_instruction_address);
	}

	ExceptionState* next_exception_to_take() {

		ExceptionState* pending_exception = _exception_vector.top_pending();
		if(nullptr == pending_exception) {
			// no exceptions to process
			return nullptr;
		}

		// compute the current execution priority by looking at all active exceptions
		ExceptionState::priority_t current_execution_priority = _exception_vector.current_execution_priority();
		ExceptionState::priority_t pending_exception_priority = pending_exception->priority();

		// do we need to switch the context to a new exception ?
		if(pending_exception_priority < current_execution_priority) {
			// instruction flow must be interrupted by this higher priority exception
			return pending_exception;
		}

		return nullptr;
	}

private:
	ExceptionStateVector& _exception_vector;
	registers& _regs;

public:
	// this variable is public because its address is needed before all objects are properly initialized
	context_switcher ctx_switcher;

};

#endif //MICROMACHINE_EMU_EXCEPTION_MANAGER_HPP
