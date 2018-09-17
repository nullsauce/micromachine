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

	void process_pending_exception(word current_addr, instruction_pair instruction) {
		// TODO: Implement priority checks
		_regs.set_pc(current_addr);
		exception_entry(*_exception_vector.top_pending_exception(), current_addr, instruction);
	}

private:

	void exception_entry(exception_state& ex, uint32_t instruction_address, instruction_pair current_instruction);

	void enter_handler_mode();
	void enter_thread_mode();

	void pop_stack(uint32_t frame_ptr, uint32_t return_address);
	void push_stack(uint32_t return_address);


	bool is_priviledged_mode() const;
	void take_exception(exception_state& exception);

	registers& 			_regs;
	memory& _mem;
	exception_vector&	_exception_vector;
;
};

#endif //MICROMACHINE_EMU_EXCEPTION_MANAGER_HPP
