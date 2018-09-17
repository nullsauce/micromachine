/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#include "exception_manager.hpp"


exception_manager::exception_manager(
		registers& regs,
		memory& mem,
		exception_vector& exceptions)
	: _regs(regs)
	, _mem(mem)
	, _exception_vector(exceptions)
	, _active_exception_count(0) {

}

void exception_manager::reset() {
	enter_thread_mode();
}

void exception_manager::exception_return(uint32_t ret_address) {
	assert(_regs.exec_mode_register().is_handler_mode());
    //fprintf(stderr,"exception_return from %08x\n", ret_address);
	// bits 4 to 24 are all ones
	// TODO: make a nice function
	if(binops::make_mask<24>() != bits<4,24>::of(ret_address)) {
		// unpredicatable
        fprintf(stderr, "unpredicatable\n");
	}

	exception_number returning_from_exception = _regs.interrupt_status_register().exception_num();

	if(!_exception_vector.is_active(returning_from_exception)) {
		// unpredicatable
        fprintf(stderr, "return from an inactive handler\n");
	}

	uint32_t frame_ptr = 0;
    const uint8_t ret_bits = (uint8_t)bits<0,4>::of(ret_address);
    switch(ret_bits) {
		case 0b0001: /* return to handler */ {
			if(_active_exception_count == 1) {
				// unpredictable
				// exception mismatch
				fprintf(stderr, "exception mismatch\n");
			} else {
				frame_ptr = _regs.sp_register().get_specific_banked_sp(sp_reg::StackType::Main);
				enter_handler_mode();
				_regs.control_register().set_sp_sel(0);
			}
			break;
		}
		case 0b1001: /* return to thread using main stack */ {
			if(_active_exception_count != 1) {
				// unpredictable
				// return to thread exception mismatch
				fprintf(stderr, "return to thread exception mismatch\n");
			} else {
				frame_ptr = _regs.sp_register().get_specific_banked_sp(sp_reg::StackType::Main);
				enter_thread_mode();
				_regs.control_register().set_sp_sel(0);
			}
			break;
		}
		case 0b1101: /* return to thread using process stack */ {
			if(_active_exception_count != 1) {
				// unpredictable
				// return to thread exception mismatch
				fprintf(stderr, "return to thread exception mismatch\n");
			} else {
				frame_ptr = _regs.sp_register().get_specific_banked_sp(sp_reg::StackType::Process);
				enter_thread_mode();
				_regs.control_register().set_sp_sel(1);
				// Assigning CurrentMode to Mode_Thread causes a drop in privilege
				// if CONTROL.nPRIV is set to 1
			}
			break;
		}
		default: /* Illegal EXC_RETURN */ {
            fprintf(stderr, "Illegal EXC_RETURN\n");
			break;
		}
	}

	_exception_vector.deactivate(returning_from_exception);
	_active_exception_count--;
	pop_stack(frame_ptr, ret_address);

	if(_regs.exec_mode_register().is_handler_mode()) {
		if(0U == (uint8_t)returning_from_exception) {
			// unpredictable
            fprintf(stderr, "unpredictable\n");
		}
	} else if(0U != (uint8_t)returning_from_exception) {
		// unpredictable
        fprintf(stderr, "unpredictable\n");
	}

	//TODO: SetEventRegister()
	// SetEventRegister()
	//TODO: InstructionSynchronizationBarrier();
	// InstructionSynchronizationBarrier();
	//TODO:
	// if CurrentMode == Mode_Thread && SCR.SLEEPONEXIT == '1' then
	// SleepOnExit();
	// IMPLEMENTATION DEFINED
}

void exception_manager::exception_entry(prioritized_exception& ex, uint32_t instruction_address, instruction_pair
current_instruction) {


	// address of next instruction by default;
	uint32_t return_address = instruction_address + current_instruction.size();

    if(exception_type::HARDFAULT == ex.state().type()) {
		// address of the instruction causing fault
		return_address = instruction_address;
	} else if(exception_type::SVCALL == ex.state().type()) {
		// address of the next instruction after svc
        return_address = instruction_address + current_instruction.size();
	} else if(exception_type::PENDSV == ex.state().type() || exception_type::SYSTICK == ex.state().type()) {
		// address of instruction to be executed after the irq
		return_address = instruction_address + current_instruction.size();
	}

	push_stack(return_address);
	take_exception(ex);
}


void exception_manager::pop_stack(uint32_t frame_ptr, uint32_t return_address) {
	_regs.set(0, _mem.read32(frame_ptr+0));
	_regs.set(1, _mem.read32(frame_ptr+4));
	_regs.set(2, _mem.read32(frame_ptr+8));
	_regs.set(3, _mem.read32(frame_ptr+12));
	_regs.set(12, _mem.read32(frame_ptr+16));
	_regs.set_lr(_mem.read32(frame_ptr+20));
	_regs.set_pc(_mem.read32(frame_ptr+24));
	word psr_bits = _mem.read32(frame_ptr+28);

	switch((uint8_t)bits<0,4>::of(return_address)) {
		case 0b0001: /* return to handler */
		case 0b1001: /* return to thread using main stack */{
			uint32_t sp_main = _regs.sp_register().get_specific_banked_sp(sp_reg::StackType::Main);
			uint32_t new_sp_main = (sp_main + 0x20) | (uint32_t)(psr_bits.bit(9) << 2);
			_regs.sp_register().set_specific_banked_sp(sp_reg::StackType::Main, new_sp_main);
		}
		case 0b1101: /* return to thread using process stack */ {
			uint32_t sp_process = _regs.sp_register().get_specific_banked_sp(sp_reg::StackType::Process);
			uint32_t new_sp_process = (sp_process + 0x20) | (uint32_t)(psr_bits.bit(9) << 2);
			_regs.sp_register().set_specific_banked_sp(sp_reg::StackType::Process, sp_process);
		}
	}

	_regs.app_status_register().copy_bits(psr_bits);
	bool force_thread = _regs.exec_mode_register().is_thread_mode() && _regs.control_register().n_priv();
	if(force_thread) {
		_regs.interrupt_status_register().set_exception_number(exception_number::from_uint(0));
	} else {
		_regs.interrupt_status_register().set_exception_number(exception_number::from_uint(psr_bits.uint(0,6)));
	}

	_regs.execution_status_register().set_thumb_bit(psr_bits.bit(epsr_reg::THUMB_BIT));
}

void exception_manager::push_stack(uint32_t return_address) {

	const size_t stack_frame_size = 32;
	const uint32_t sp_mask = ~((uint32_t)0b100);
	const bool frame_ptr_align = _regs.get_sp().bit(2);

	uint32_t frame_ptr = (_regs.get_sp() - stack_frame_size) & sp_mask;
	_regs.set_sp(frame_ptr);

	uint xpsr_status = 	_regs.xpsr_register().uint(0, 8) |
						(frame_ptr_align << 8) |
						_regs.xpsr_register().uint(10, 22) << 10;

	_mem.write32(frame_ptr+0,  _regs.get(0));
	_mem.write32(frame_ptr+4,  _regs.get(1));
	_mem.write32(frame_ptr+8,  _regs.get(2));
	_mem.write32(frame_ptr+12, _regs.get(3));
	_mem.write32(frame_ptr+16, _regs.get(12));
	_mem.write32(frame_ptr+20, _regs.get_lr());
	_mem.write32(frame_ptr+24, return_address);
	_mem.write32(frame_ptr+28, xpsr_status);


	if(_regs.exec_mode_register().is_handler_mode()) {
		_regs.set_lr(0xFFFFFFF1); // return to handler
    } else if(0 == _regs.control_register().sp_sel()) {
		_regs.set_lr(0xFFFFFFF9); // return to thread using main stack
	} else {
		_regs.set_lr(0xFFFFFFFD); // return to thread using process stack
	}
}



bool exception_manager::is_priviledged_mode() const {
	return _regs.exec_mode_register().is_handler_mode() &&
		!_regs.control_register().n_priv();
}

void exception_manager::take_exception(prioritized_exception& exception) {
	// enter handler mode
	enter_handler_mode();

	// set ipsr with exception number
	_regs.interrupt_status_register().set_exception_number(exception.state().number());

	// stack is now SP_main
	_regs.control_register().set_sp_sel(0);

	exception.state().activate();
	_active_exception_count++;

	//SCS_UpdateStatusRegs();
	//SetEventRegister();
	//InstructionSynchronizationBarrier();
	uint8_t exception_number = exception.state().number();
	uint32_t vector_table_offset = sizeof(word) * exception_number;
	uint32_t handler_address = _mem.read32(vector_table_offset);
	_regs.branch_link_interworking(handler_address);
}


void exception_manager::enter_handler_mode() {
	_regs.exec_mode_register().set_handler_mode();
}

void exception_manager::enter_thread_mode() {
	_regs.exec_mode_register().set_thread_mode();
}
