/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#pragma once

#include "exception/exception.hpp"
#include "exception/exception_return_handler.hpp"
#include "exception/exception_controller.hpp"
#include "instruction/instruction_pair.hpp"
#include "interworking_brancher.hpp"
#include "memory/memory.hpp"
#include "registers/core_registers/core_registers.hpp"
#include "registers/special_registers/special_registers.hpp"
#include "types/bits.hpp"

namespace micromachine::system {

class context_switcher : public exception_return_handler {
private:
	core_registers& _core_regs;
	special_registers& _special_regs;
	execution_mode& _execution_mode;
	memory& _mem;
	exception_controller& _exception_controller;
	interworking_brancher& _interworking_brancher;

public:
	context_switcher(core_registers& core_regs,
					 special_registers& special_regs,
					 execution_mode& execution_mode,
					 memory& mem,
					 exception_controller& exception_controller,
					 interworking_brancher& interworking_brancher)
		: _core_regs(core_regs)
		, _special_regs(special_regs)
		, _execution_mode(execution_mode)
		, _mem(mem)
		, _exception_controller(exception_controller)
		, _interworking_brancher(interworking_brancher) {}

	void exception_return(uint32_t ret_address) override {
		assert(_execution_mode.is_in_handler_mode());

		if(!bits<4, 24>::of(ret_address).are_set()) {
			// unpredictable
			fprintf(stderr, "unpredictable.\n");
		}

		exception exception_returning_from = _special_regs.interrupt_status_register().exception();

		// The maximum theoretical value of exception_num from IPSR is 63
		// This implementation might not support that many exceptions.
		// Check that the exception number is within accepted range.
		if(exception_returning_from >= _exception_controller.highest_accepted_exception_number()) {
			fprintf(stderr, "unpredictable.\n");
		}

		if(!_exception_controller.is_active(exception_returning_from)) {
			// unpredictable
			fprintf(stderr, "return from an inactive handler\n");
		}

		uint32_t frame_ptr = 0;
		const uint8_t ret_bits = (uint8_t)bits<0, 4>::of(ret_address).extract();
		switch(ret_bits) {
			case 0b0001: /* return to handler EXC_RETURN=0xFFFFFFF1 */ {
				if(_exception_controller.active_count() == 1) {
					// unpredictable
					// exception mismatch
					fprintf(stderr, "exception mismatch\n");
				} else {
					frame_ptr =
						_core_regs.sp().get_specific_banked_sp(sp_reg::stack_type::main);
					_execution_mode.enter_handler_mode();
					_special_regs.control_register().sp_sel() = false;
				}
				break;
			}
			case 0b1001: /* return to thread using main stack EXC_RETURN=0xFFFFFFF9  */ {
				if(_exception_controller.active_count() != 1) {
					// unpredictable
					// return to thread exception mismatch
					fprintf(stderr, "return to thread exception mismatch\n");
				} else {
					frame_ptr =
						_core_regs.sp().get_specific_banked_sp(sp_reg::stack_type::main);
					_execution_mode.enter_thread_mode();
					_special_regs.control_register().sp_sel() = false;
				}
				break;
			}
			case 0b1101: /* return to thread using process stack EXC_RETURN=0xFFFFFFFD*/ {
				if(_exception_controller.active_count() != 1) {
					// unpredictable
					// return to thread exception mismatch
					fprintf(stderr, "return to thread exception mismatch\n");
				} else {
					frame_ptr =
						_core_regs.sp().get_specific_banked_sp(sp_reg::stack_type::process);
					_execution_mode.enter_thread_mode();
					_special_regs.control_register().sp_sel() = true;
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

		_exception_controller.set_active(exception_returning_from, false);
		//_active_exception_count--;
		pop_stack(frame_ptr, ret_address);

		if(_execution_mode.is_in_handler_mode()) {
			if(0U == _special_regs.interrupt_status_register().exception()) {
				// unpredictable
				fprintf(stderr,
						"unpredictable. (is_handler_mode but exception_returning_from is zero)\n");
			}
		} else if(0U != _special_regs.interrupt_status_register().exception()) {
			// unpredictable
			fprintf(stderr,
					"unpredictable. (not in handler mode, but exception_returning_from is %d)\n",
					(uint8_t)_special_regs.interrupt_status_register().exception());
		}

		// TODO: SetEventRegister()
		// SetEventRegister()
		// TODO: InstructionSynchronizationBarrier();
		// InstructionSynchronizationBarrier();
		// TODO:
		// if CurrentMode == Mode_Thread && SCR.SLEEPONEXIT == '1' then
		// SleepOnExit();
		// IMPLEMENTATION DEFINED
	}

	void exception_entry(exception_state& exception_state,
						 uint32_t instruction_address,
						 instruction_pair current_instruction,
						 uint32_t next_instruction_address) {

		uint32_t return_address;

		// 1. Compute the return address
		switch(exception_state.kind()) {
			// address of the instruction causing fault
			case exception::HARDFAULT:
				return_address = instruction_address;
				break;
			case exception::SVCALL:
				return_address = next_instruction_address;
				break;
			case exception::PENDSV:
				return_address = next_instruction_address;
				break;
			default:
				return_address = next_instruction_address;
		}

		// 2. Save context
		push_stack(return_address);

		// 3. Take exception

		// enter handler mode
		_execution_mode.enter_handler_mode();

		// set ipsr with exception number
		_special_regs.interrupt_status_register().set_exception_number(exception_state.kind());

		// stack is now SP_main
		_special_regs.control_register().set_sp_sel(0);

		// activate it. Pending flag is cleared automatically
		exception_state.set_active(true);
		exception_state.set_pending(false);

		// SCS_UpdateStatusRegs();
		// SetEventRegister();
		// InstructionSynchronizationBarrier();
		uint32_t vector_table_offset = sizeof(uint32_t) * exception_state.kind();
		uint32_t handler_address = _mem.read32(vector_table_offset);
		_interworking_brancher.branch_link_interworking(handler_address);
	}

	void pop_stack(uint32_t frame_ptr, uint32_t return_address) {

		_core_regs.set(0, _mem.read32(frame_ptr + 0));
		_core_regs.set(1, _mem.read32(frame_ptr + 4));
		_core_regs.set(2, _mem.read32(frame_ptr + 8));
		_core_regs.set(3, _mem.read32(frame_ptr + 12));
		_core_regs.set(12, _mem.read32(frame_ptr + 16));
		_core_regs.lr() = _mem.read32(frame_ptr + 20);
		_core_regs.pc() = _mem.read32(frame_ptr + 24);
		uint32_t xpsr_status = _mem.read32(frame_ptr + 28);
		uint32_t stack_align = bits<9>::of(xpsr_status) << 2;

		switch((uint8_t)bits<0, 4>::of(return_address).extract()) {
			case 0b0001: /* return to handler */
			case 0b1001: /* return to thread using main stack */ {
				uint32_t sp_main =
					_core_regs.sp().get_specific_banked_sp(sp_reg::stack_type::main);
				uint32_t new_sp_main = (sp_main + 0x20) | stack_align;
				_core_regs.sp().set_specific_banked_sp(sp_reg::stack_type::main,
																new_sp_main);
				break;
			}
			case 0b1101: /* return to thread using process stack */ {
				uint32_t sp_process =
					_core_regs.sp().get_specific_banked_sp(sp_reg::stack_type::process);
				uint32_t new_sp_process = (sp_process + 0x20) | stack_align;
				_core_regs.sp().set_specific_banked_sp(sp_reg::stack_type::process,
																new_sp_process);
				break;
			}
		}

		// Restore the APSR, IPSR and EPSR bits of XPSR
		// This is done by first copying the pop'd XPSR value into the
		// XPSR register but with the reserved bits ignored.
		// (ESPR stack alignment is also considered reserved and is therefore ignored too)
		// And secondly, optionally zeroing the IPSR exception number.
		const uint32_t reserved_bits_mask = apsr_reg::all_flags_bits::as_mask<uint32_t>() |
											ipsr_reg::ipsr_bits::as_mask<uint32_t>() |
											epsr_reg::epsr_bits::as_mask<uint32_t>() |
											epsr_reg::stack_align_flag_bit::as_mask<uint32_t>();
		// Zero out reserved bits for good measure.
		// Note that the stack alignment value is ignored.
		_special_regs.xpsr_register() = xpsr_status & reserved_bits_mask;

		bool force_thread = _execution_mode.is_in_thread_mode() && _special_regs.control_register().n_priv();
		// If force_thread is true, the IPSR (interrupt status register)
		// exception number is zeroed.
		if(force_thread) {
			_special_regs.interrupt_status_register().set_exception_number(
				exception::from_number(0));
		}
	}

	void push_stack(uint32_t return_address) {

		const size_t stack_frame_size = 32;

		// The 3rd bit of SP is zeroed to ensure proper 8-byte alignment of the
		// stack frame
		// It is moved into the 10th bit of the saved XPSR value to be later
		// restored as 3rd bit of SP when the stack is popped.
		const uint32_t sp_mask = ~((uint32_t)0b100);
		const bool frame_ptr_align = bits<2>::of<uint32_t>(_core_regs.sp());

		uint32_t frame_ptr = (_core_regs.sp() - stack_frame_size) & sp_mask;

		_core_regs.sp() = frame_ptr;

		// the 10th bit of XPSR status is set with the stack alignment indicator.
		// We copy its value and overwrite the bit at index 8.
		uint32_t xpsr_status = _special_regs.xpsr_register();
		bits<9>::of(xpsr_status) = frame_ptr_align;

		_mem.write32(frame_ptr + 0, _core_regs.get(0));
		_mem.write32(frame_ptr + 4, _core_regs.get(1));
		_mem.write32(frame_ptr + 8, _core_regs.get(2));
		_mem.write32(frame_ptr + 12, _core_regs.get(3));
		_mem.write32(frame_ptr + 16, _core_regs.get(12));
		_mem.write32(frame_ptr + 20, _core_regs.lr());
		_mem.write32(frame_ptr + 24, return_address);
		_mem.write32(frame_ptr + 28, xpsr_status);

		if(_execution_mode.is_in_handler_mode()) {
			_core_regs.lr() = 0xFFFFFFF1; // return to handler
		} else if(0 == _special_regs.control_register().sp_sel()) {
			_core_regs.lr() = 0xFFFFFFF9; // return to thread using main stack
		} else {
			_core_regs.lr() = 0xFFFFFFFD; // return to thread using process stack
		}
	}
};

} // namespace micromachine::system
