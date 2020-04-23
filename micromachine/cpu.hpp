//
// Created by fla on 17-5-9.
//

#pragma once

#include <utility>

#include "execution/context_switcher.hpp"
#include "execution/exec.hpp"
#include "execution/exec_dispatcher.hpp"
#include "helpers/disassembler.hpp"

#include "exception/exception_controller.hpp"
#include "instruction/instruction_pair.hpp"
#include "instruction/instructions.hpp"

#include "exception/exception.hpp"
#include "exception/exception_vector.hpp"
#include "execution/interworking_brancher.hpp"
#include "memory/memory.hpp"
#include "registers/core_registers.hpp"
#include "registers/special_registers.hpp"
#include "types/types.hpp"

namespace micromachine::system {

class cpu {
private:
	memory& _mem;
	exception_vector& _exception_vector;
	exception_controller& _exception_controller;
	special_registers _special_registers;
	core_registers _core_regs;
	interworking_brancher _interworking_brancher;
	exec_dispatcher _exec_dispatcher;
	context_switcher _ctx_switcher;
	event_register _event_register;
	execution_mode _exec_mode;

	bool _break_signal;
	bool _enter_low_power_mode_signal;

	uint64_t _debug_instruction_counter;

public:
	enum class step_result {
		OK,
		FAULT,
		BREAK,
	};

	cpu& operator=(const cpu& other) = delete;

	cpu(memory& memory,
		exception_vector& exception_vector,
		exception_controller& exception_controller)
		: _mem(memory)
		, _exception_vector(exception_vector)
		, _exception_controller(exception_controller)
		, _core_regs(_exec_mode, _special_registers.control_register(), _ctx_switcher)
		, _interworking_brancher(_exec_mode,
								 _special_registers.execution_status_register(),
								 _ctx_switcher,
								 _core_regs.pc_register())
		, _exec_dispatcher(_exception_controller,
						   _core_regs,
						   _special_registers,
						   _mem,
						   _interworking_brancher,
						   _exec_mode,
						   _event_register,
						   _break_signal,
						   _enter_low_power_mode_signal)
		, _ctx_switcher(_core_regs,
						_special_registers,
						_exec_mode,
						_mem,
						_exception_vector,
						_interworking_brancher)
		, _break_signal(false)
		, _enter_low_power_mode_signal(false)
		, _debug_instruction_counter(0) {}

	cpu(memory& memory,
		exception_vector& exception_vector,
		exception_controller& exception_controller,
		const cpu& other)
		: _mem(memory)
		, _exception_vector(exception_vector)
		, _exception_controller(exception_controller)
		, _special_registers(other._special_registers)
		, _core_regs(_exec_mode,
					 _special_registers.control_register(),
					 _ctx_switcher,
					 other._core_regs)
		, _interworking_brancher(_exec_mode,
								 _special_registers.execution_status_register(),
								 _ctx_switcher,
								 _core_regs.pc_register())
		, _exec_dispatcher(_exception_controller,
						   _core_regs,
						   _special_registers,
						   _mem,
						   _interworking_brancher,
						   _exec_mode,
						   _event_register,
						   _break_signal,
						   _enter_low_power_mode_signal)
		, _ctx_switcher(_core_regs,
						_special_registers,
						_exec_mode,
						_mem,
						_exception_vector,
						_interworking_brancher)
		, _exec_mode(other._exec_mode)
		, _break_signal(other._break_signal)
		, _enter_low_power_mode_signal(other._enter_low_power_mode_signal)
		, _debug_instruction_counter(other._debug_instruction_counter) {}

	void reset(uint32_t initial_pc) {
		uint32_t initial_sp_main = _mem.read32_unchecked(0U) & 0xFFFFFFFC;

		_exec_mode = execution_mode::thread;

		_core_regs.reset();
		_core_regs.set_sp(initial_sp_main);
		_core_regs.set_pc(initial_pc);
		_core_regs.set_lr(0);

		_special_registers.app_status_register().reset();
		_special_registers.execution_status_register().set_thumb_bit(true);
		_special_registers.interrupt_status_register().reset();
		_special_registers.primask_register().reset();
		_special_registers.control_register().reset();

		_exception_vector.reset();
		_event_register.clear();

		_break_signal = false;
		_enter_low_power_mode_signal = false;
		_debug_instruction_counter = 0;
	}

	step_result step() {

		if(_break_signal) {
			return cpu::step_result::BREAK;
		}
		_debug_instruction_counter++;

		const uint32_t cur_instruction_address = _core_regs.get_pc();
		instruction_pair cur_intruction = fetch_instruction(cur_instruction_address);

		if(!_special_registers.execution_status_register().thumb_bit_set()) {
			// Thumb bit not set
			// all instructions in this state are UNDEFINED .
			_exception_controller.raise_hardfault();
		}

		// check for asynchronous interrupt
		exception_state* ex = next_exception_to_take();

		if(nullptr == ex) {
			// execute instruction at current PC
			// simulate prefetch of 2 instructions during execution
			_core_regs.set_pc(cur_instruction_address + 4);
			_core_regs.reset_pc_dirty_status();
			_exec_dispatcher.decode_instruction(cur_intruction);

			// check for synchronous exception that might
			// have been raised during the execution
			ex = next_exception_to_take();
		}
		uint32_t next_instruction_address =
			get_next_instruction_address(cur_instruction_address, cur_intruction);

		if(ex) {

			// wake up event for wfe or wfi

			// an exception to be taken is pending
			_core_regs.set_pc(cur_instruction_address);
			_ctx_switcher.exception_entry(*ex,
										  cur_instruction_address,
										  cur_intruction,
										  next_instruction_address);
		} else {
			// no exception taken, execution will continue at next address
			_core_regs.set_pc(next_instruction_address);
		}

		return cpu::step_result::OK;
	}

	const exception_vector& exceptions() const {
		return _exception_vector;
	}

	memory& mem() {
		return _mem;
	}

	const memory& mem() const {
		return _mem;
	}

	core_registers& regs() {
		return _core_regs;
	}

	const core_registers& regs() const {
		return _core_regs;
	}

	special_registers& special_regs() {
		return _special_registers;
	}

	const special_registers& special_regs() const {
		return _special_registers;
	}

	uint64_t debug_instruction_counter() const {
		return _debug_instruction_counter;
	}

	exception_controller& interrupt() {
		return _exception_controller;
	}

	execution_mode get_execution_mode() const {
		return _exec_mode;
	}

	void set_execution_mode(execution_mode execution_mode) {
		_exec_mode = execution_mode;
	}

	exception::priority_t current_execution_priority() const {
		exception::priority_t prio = exception::THREAD_MODE_PRIORITY;
		exception::priority_t boosted_prio = exception::THREAD_MODE_PRIORITY;

		for(size_t i = 2; i < 32; i++) {
			const exception_state& e = _exception_vector.at(i);
			if(!e.is_active())
				continue;
			if(e.priority() < prio) {
				prio = e.priority();
			}
		}

		// if primask is set, ignore all maskable exceptions by
		// pretending the executing priority is now 0
		if(_special_registers.primask_register().pm()) {
			prio = 0;
		}

		if(boosted_prio < prio) {
			return boosted_prio;
		} else {
			return prio;
		}
	}

private:
	exception_state* next_exception_to_take() {
		exception_state* pending_exception = _exception_vector.top_pending();
		if(nullptr == pending_exception) {
			// no exceptions to process
			return nullptr;
		}

		// compute the current execution priority by looking at all active exceptions
		exception::priority_t current_priority = current_execution_priority();
		exception::priority_t pending_priority = pending_exception->priority();

		// do we need to switch the context to a new exception ?
		if(pending_priority < current_priority) {
			// instruction flow must be interrupted by this higher priority exception
			return pending_exception;
		}

		return nullptr;
	}

	instruction_pair fetch_instruction(uint32_t address) const {
		return {_mem.read32_unchecked(address)};
	}

	uint32_t get_next_instruction_address(uint32_t instr_addr, instruction_pair instruction) const {
		if(!_core_regs.branch_occured()) {
			return instr_addr + instruction.size();
		} else {
			return _core_regs.get_pc();
		}
	}
};
} // namespace micromachine::system
