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

class exception_manager : public exception_return_handler{
public:
	exception_manager(
		registers& regs,
		memory& mem,
		exception_vector& exceptions);
	void exception_return(uint32_t ret_address) override ;
	void reset();

	void process_pending_exception(uint32_t current_addr, instruction_pair instruction, uint32_t next_instruction_address) {
		// Check if this pending exception has priority over the current priority of the
		// instruction stream.
		// Lower priority value means higher priority.
		bool smaller = _exception_vector.top_pending_exception()->priority() < _exception_vector.current_priority();
		// TODO: Check Priority grouping.
		// When two pending exceptions have the same group priority, the lower pending exception number has
		// priority over the higher pending number as part of the priority precedence rule.
		if(smaller) {
			// yep, lets activate this exception
			_regs.set_pc(current_addr);
			exception_entry(*_exception_vector.top_pending_exception(), current_addr, instruction, next_instruction_address);
		}
	}

private:

	void exception_entry(exception_state& ex, uint32_t instruction_address, instruction_pair current_instruction, uint32_t next_instruction_address);

	void pop_stack(uint32_t frame_ptr, uint32_t return_address);
	void push_stack(uint32_t return_address);


	registers& 			_regs;
	memory& _mem;
	exception_vector&	_exception_vector;
;
};

#endif //MICROMACHINE_EMU_EXCEPTION_MANAGER_HPP
