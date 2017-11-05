//
// Created by fla on 17-5-9.
//

#ifndef MICROMACHINE_CPU_HPP
#define MICROMACHINE_CPU_HPP

#include "types.hpp"
#include "registers.hpp"
#include "apsr_register.hpp"
#include "exec.hpp"
#include "instructions.hpp"
#include "exec.hpp"
#include "memory.hpp"
#include "exec_mode_register.hpp"
#include "exception.hpp"
#include "exception_vector.hpp"
#include "exec_dispatcher.hpp"


class cpu {



public:

	cpu()
		: _regs(_exec_mode_reg, _active_exceptions[exception::HARDFAULT])
		, _mem(_active_exceptions[exception::HARDFAULT])
		, _exec_dispatcher(_regs, _status_reg, _mem, _active_exceptions)
	{}

	void execute(const halfword instr, const halfword second_instr) {
		_exec_dispatcher.dispatch_instruction(instr, second_instr);
	}

	void reset() {

		_active_exceptions.clear();
		_exec_mode_reg.set_thread_mode();
		_regs.reset();
		_status_reg = 0;

		// set sp to vector+0
		/*
		uint32_t stack_pointer = _mem.read32(0);
		_regs.set_sp(stack_pointer);
		fprintf(stderr, "reset handler : %08x\n", stack_pointer);

		// branch to entry point
		word reset_handler = _mem.read32(4);
		fprintf(stderr, "reset handler : %08x\n", reset_handler);
		_regs.set_pc(reset_handler);*/

		/*
		SP_main = MemA[vectortable,4] & 0xFFFFFFFC;
		SP_process = ((bits(30) UNKNOWN):’00’);
		LR = bits(32) UNKNOWN; // Value must be initialised by software
		CurrentMode = Mode_Thread;
		APSR = bits(32) UNKNOWN; // Flags UNPREDICTABLE from reset
		IPSR<5:0> = 0x0; // Exception number clearedat reset
		PRIMASK.PM = '0'; // Priority mask cleared at reset
		CONTROL.SPSEL = '0'; // Current stack is Main
		CONTROL.nPRIV = '0'; // Thread is privileged
		ResetSCSRegs(); // Catch-all function for System Control Space reset
		ExceptionActive[*] = '0'; // All exceptions Inactive
		ClearEventRegister(); // See WFE instruction for more information
		start = MemA[vectortable+4,4]; // Load address of reset routine
		BLXWritePC(start); // Start execution of reset routin
	 */
	}

	static bool is_32bit_thumb_encoding(const halfword& instruction) {
		return 0b111 == instruction.uint(13, 3) &&
			   0b00 !=  instruction.uint(11, 2);
	}

	bool step() {

		if(_active_exceptions.any_signaled()) {
			fprintf(stderr, "Exception signaled\n");
			return true;
		} else {


			word current_instr = _regs.get_pc();
			halfword instr = _mem.read16(current_instr);
			halfword second_instruction = 0;
			if(is_32bit_thumb_encoding(instr)) {
				second_instruction = _mem.read16(current_instr + sizeof(halfword));
			}

			fprintf(stderr, "exec PC:%08x\n",
					(uint32_t) current_instr
			);

			_regs.set_pc(current_instr + 4); // prefetch 2 instructions
			_regs.reset_pc_dirty_status();

			execute(instr, second_instruction);

			bool hard_fault = active_exceptions().is_signaled(exception::HARDFAULT);
			bool fault = hard_fault;
			if(!_regs.branch_occured()) {
				if(hard_fault) {
					_regs.set_pc(current_instr);
				} else {
					_regs.set_pc(current_instr + 2);
				}
			}

			if(active_exceptions().any_signaled()) {
				//exception_entry(active_exceptions.storage());
			}

			return hard_fault;
		}
	}

	const exception_vector& active_exceptions() const {
		return _active_exceptions;
	}

	memory& mem() {
		return _mem;
	}

	const memory& mem() const {
		return _mem;
	}

	registers& regs() {
		return _regs;
	}

	const registers& regs() const {
		return _regs;
	}

	// for testing
	apsr_register& apsr() {
		return _status_reg;
	}

	apsr_register& flags() {
		return _status_reg;
	}

private:



	bool is_priviledged_mode() const {
		return _exec_mode_reg.is_handler_mode() &&
			!_regs.control_register().n_priv();
	}

	void push_stack() {

		const size_t stack_frame_size = 32;
		const uint32_t sp_mask = ~((uint32_t)0b100);
		const bool frame_ptr_align = _regs.get_sp().bit(2);

		uint32_t frame_ptr = (_regs.get_sp() - stack_frame_size) & sp_mask;
		_regs.set_sp(frame_ptr);

		// TODO: ReturnAddress()
		uint32_t return_address = regs().get_pc() - 2;

		uint xpsr_status = 	_status_reg.uint(0, 8) |
							(frame_ptr_align << 8) |
							_status_reg.uint(10, 22) << 10;

		_mem.write32(frame_ptr+0,  _regs.get(0));
		_mem.write32(frame_ptr+4,  _regs.get(1));
		_mem.write32(frame_ptr+8,  _regs.get(2));
		_mem.write32(frame_ptr+12, _regs.get(3));
		_mem.write32(frame_ptr+16, _regs.get(12));
		_mem.write32(frame_ptr+20, _regs.get_lr());
		_mem.write32(frame_ptr+24, return_address);
		_mem.write32(frame_ptr+28, xpsr_status);


		if(_exec_mode_reg.is_handler_mode()) {
			_regs.set_lr(0xFFFFFFF1);
		} else if(_regs.control_register().sp_sel()) {
			_regs.set_lr(0xFFFFFFF9);
		} else {
			_regs.set_lr(0xFFFFFFFD);
		}
	}

	void signal_exception(exception ex)  {
		precond(exception::INVALID != ex, "Bad exception identifier");
		_active_exceptions[ex] = true;
	}

	void exception_entry(exception ex) {
		push_stack();
		take_exception(ex);
	}


	void take_exception(exception exception) {
		// enter handler mode
		_exec_mode_reg.set_handler_mode();

		// set ipsr with exception number
		_status_reg = (_status_reg & ~binops::make_mask<uint32_t>(5))
						| (size_t)exception;

		// stack is now SP_main
		_regs.control_register().set_sp_sel(0);

		_active_exceptions[exception] = true;

		//SCS_UpdateStatusRegs();
		//SetEventRegister();
		//InstructionSynchronizationBarrier();
		uint32_t handler_address = _mem.read32(4*(size_t)exception);
		_regs.branch(handler_address);
	}

	exception_vector	_active_exceptions;
	registers 			_regs;
	apsr_register 		_status_reg;
	memory 				_mem;
	exec_mode_register 	_exec_mode_reg;
	exec_dispatcher _exec_dispatcher;


};

#endif //MICROMACHINE_CPU_HPP
