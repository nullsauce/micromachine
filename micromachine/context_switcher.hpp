/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_CONTEXT_SWITCHING_HPP
#define MICROMACHINE_EMU_CONTEXT_SWITCHING_HPP


#include "exception_state.hpp"

class context_switcher : public exception_return_handler {

private:
	registers& _regs;
	memory& _mem;
	exception_state_vector& _exception_state_vector;

public:

	context_switcher(registers& regs, memory& mem, exception_state_vector& exception_vector)
		: _regs(regs)
		, _mem(mem)
		, _exception_state_vector(exception_vector)
	{}

	void exception_return(uint32_t ret_address) override {
		assert(_regs.exec_mode_register().is_handler_mode());

		// Ensure that bits 4 to 24 are set
		if((uint32_t)binops::make_mask<24>() != (uint32_t)bits<4,24>::of(ret_address)) {
			// unpredictable
			fprintf(stderr, "unpredictable.\n");
		}

		exception::Type exception_returning_from = _regs.interrupt_status_register().exception_num();

		// The maximum theoretical value of exception_num from IPSR is 63
		// This implementation might not support that many exceptions.
		// Check that the exception number is within accepted range.
		if(exception_returning_from >= _exception_state_vector.highest_accepted_exception_number()) {
			fprintf(stderr, "unpredictable.\n");
		}

		if(!_exception_state_vector.interrupt_state(exception_returning_from).is_active()) {
			// unpredictable
			fprintf(stderr, "return from an inactive handler\n");
		}

		uint32_t frame_ptr = 0;
		const uint8_t ret_bits = (uint8_t)bits<0,4>::of(ret_address).extract();
		switch(ret_bits) {
			case 0b0001: /* return to handler EXC_RETURN=0xFFFFFFF1 */ {
				if(_exception_state_vector.active_count() == 1) {
					// unpredictable
					// exception mismatch
					fprintf(stderr, "exception mismatch\n");
				} else {
					frame_ptr = _regs.sp_register().get_specific_banked_sp(sp_reg::StackType::Main);
					_regs.exec_mode_register().set_handler_mode();
					_regs.control_register().set_sp_sel(0);
				}
				break;
			}
			case 0b1001: /* return to thread using main stack EXC_RETURN=0xFFFFFFF9  */ {
				if(_exception_state_vector.active_count() != 1) {
					// unpredictable
					// return to thread exception mismatch
					fprintf(stderr, "return to thread exception mismatch\n");
				} else {
					frame_ptr = _regs.sp_register().get_specific_banked_sp(sp_reg::StackType::Main);
					_regs.exec_mode_register().set_thread_mode();
					_regs.control_register().set_sp_sel(0);
				}
				break;
			}
			case 0b1101: /* return to thread using process stack EXC_RETURN=0xFFFFFFFD*/ {
				if(_exception_state_vector.active_count() != 1) {
					// unpredictable
					// return to thread exception mismatch
					fprintf(stderr, "return to thread exception mismatch\n");
				} else {
					frame_ptr = _regs.sp_register().get_specific_banked_sp(sp_reg::StackType::Process);
					_regs.exec_mode_register().set_thread_mode();
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

		_exception_state_vector.interrupt_state(exception_returning_from).deactivate();
		//_active_exception_count--;
		pop_stack(frame_ptr, ret_address);

		if(_regs.exec_mode_register().is_handler_mode()) {
			if(0U == _regs.interrupt_status_register().exception_num()) {
				// unpredictable
				fprintf(stderr, "unpredictable. (is_handler_mode but exception_returning_from is zero)\n");
			}
		} else if(0U != _regs.interrupt_status_register().exception_num()) {
			// unpredictable
			fprintf(stderr, "unpredictable. (not in handler mode, but exception_returning_from is %d)\n", (uint8_t)_regs.interrupt_status_register().exception_num());
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

	void exception_entry(exception_state& exception_state, uint32_t instruction_address, instruction_pair
	current_instruction, uint32_t next_instruction_address) {

		uint32_t return_address;

		// 1. Compute the return address
		switch(exception_state.number()) {
			// address of the instruction causing fault
			case exception::Type::HARDFAULT: return_address = instruction_address; break;
			case exception::Type::SVCALL: return_address = next_instruction_address; break;
			case exception::Type::PENDSV: return_address = next_instruction_address; break;
			default: return_address = next_instruction_address;
		}

		// 2. Save context
		push_stack(return_address);

		// 3. Take exception

		// enter handler mode
		_regs.exec_mode_register().set_handler_mode();

		// set ipsr with exception number
		_regs.interrupt_status_register().set_exception_number(exception_state.number());

		// stack is now SP_main
		_regs.control_register().set_sp_sel(0);

		// activate it
		exception_state.activate();

		//SCS_UpdateStatusRegs();
		//SetEventRegister();
		//InstructionSynchronizationBarrier();
		uint32_t vector_table_offset = sizeof(uint32_t) * exception_state.number();
		uint32_t handler_address = _mem.read32(vector_table_offset);
		_regs.branch_link_interworking(handler_address);
	}


	void pop_stack(uint32_t frame_ptr, uint32_t return_address) {

		_regs.set(0, _mem.read32(frame_ptr+0));
		_regs.set(1, _mem.read32(frame_ptr+4));
		_regs.set(2, _mem.read32(frame_ptr+8));
		_regs.set(3, _mem.read32(frame_ptr+12));
		_regs.set(12, _mem.read32(frame_ptr+16));
		_regs.set_lr(_mem.read32(frame_ptr+20));
		_regs.set_pc(_mem.read32(frame_ptr+24));
		uint32_t xpsr_status = _mem.read32(frame_ptr + 28);
		uint32_t stack_align = bits<8>::of(xpsr_status) << 2;

		switch((uint8_t)bits<0,4>::of(return_address).extract()) {
			case 0b0001: /* return to handler */
			case 0b1001: /* return to thread using main stack */{
				uint32_t sp_main = _regs.sp_register().get_specific_banked_sp(sp_reg::StackType::Main);
				uint32_t new_sp_main = (sp_main + 0x20) | stack_align;
				_regs.sp_register().set_specific_banked_sp(sp_reg::StackType::Main, new_sp_main);
				break;
			}
			case 0b1101: /* return to thread using process stack */ {
				uint32_t sp_process = _regs.sp_register().get_specific_banked_sp(sp_reg::StackType::Process);
				uint32_t new_sp_process = (sp_process + 0x20) | stack_align;
				_regs.sp_register().set_specific_banked_sp(sp_reg::StackType::Process, new_sp_process);
				break;
			}
		}

		// Restore the APSR, IPSR and EPSR bits of XPSR
		// This is done by first copying the pop'd XPSR value into the
		// XPSR register but with the reserved bits ignored.
		// (ESPR stack alignment is also considered reserved and is therefore ignored too)
		// And secondly, optionally zeroing the IPSR exception number.
		const uint32_t reserved_bits_mask = apsr_reg::flags_bits::as_mask<uint32_t>() |
											ipsr_reg::ipsr_bits::as_mask<uint32_t>() |
											epsr_reg::thumb_bit::as_mask<uint32_t>() |
											epsr_reg::stack_align_bit::as_mask<uint32_t>();
		// Zero out reserved bits for good measure.
		// Note that the stack alignment value is ignored.
		_regs.xpsr_register() = xpsr_status & reserved_bits_mask;

		bool force_thread = _regs.exec_mode_register().is_thread_mode() && _regs.control_register().n_priv();
		// If force_thread is true, the IPSR (interrupt status register)
		// exception number is zeroed.
		if(force_thread) {
			_regs.interrupt_status_register().set_exception_number(exception::from_number(0));
		}
	}

	void push_stack(uint32_t return_address) {

		const size_t stack_frame_size = 32;
		const uint32_t sp_mask = ~((uint32_t)0b100);
		const bool frame_ptr_align = bits<2>::of(_regs.get_sp());

		//TODO: Check docs for conditional stack switch
		uint32_t frame_ptr = (_regs.get_sp() - stack_frame_size) & sp_mask;

		_regs.set_sp(frame_ptr);

		// the 9th bit of XPSR status is set with the stack alignment indicator.
		// We copy its value and overwrite the bit at index 8.
		uint32_t xpsr_status = _regs.xpsr_register();
		bits<8>::of(xpsr_status) = (bool)(frame_ptr_align & 1);

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
};

#endif //MICROMACHINE_EMU_CONTEXT_SWITCHING_HPP
