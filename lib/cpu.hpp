//
// Created by fla on 17-5-9.
//

#ifndef MICROMACHINE_CPU_HPP
#define MICROMACHINE_CPU_HPP


#include "types.hpp"
#include "instruction_pair.hpp"
#include "registers/registers.hpp"
#include "registers/apsr_reg.hpp"
#include "registers/custom/generic_io_reg.hpp"
#include "exec.hpp"
#include "instructions.hpp"
#include "exec.hpp"
#include "memory.hpp"
#include "registers/exec_mode_reg.hpp"
#include "context_switcher.hpp"
#include "exec_dispatcher.hpp"
#include "interrupter.hpp"
#include "disasm.hpp"
#include "timer.hpp"
#include "nvic.hpp"

class cpu {



public:

	cpu();

	enum class State {
		RUN,
		FAULT,
		BREAK,
	};

	void reset();
	instruction_pair fetch_instruction(uint32_t address) const;
	instruction_pair fetch_instruction_debug(uint32_t address) const;
	State step();
	const ExceptionStateVector& exceptions() const;
	memory& mem();
	const memory& mem() const;
	registers& regs();
	const registers& regs() const;

	bool load_elf(const std::string& path);

	uint64_t debug_instruction_counter() const {
		return _debug_instruction_counter;
	}

	void set_io_callback(generic_io_reg::callback_t callback) {
		_io_reg_callback = callback;
	}

private:

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

	void execute(const instruction_pair instr);

	uint32_t get_next_instruction_address(uint32_t instr_addr, instruction_pair instruction) const;

	generic_io_reg::callback_t _io_reg_callback;
	registers 			_regs;
	ExceptionStateVector _exception_vector;
	interrupter			_interrupter;
	nvic				_nvic;
	shpr2_reg 			_sphr2_reg;
	shpr3_reg 			_sphr3_reg;
	generic_io_reg		_generic_io_reg;
	systick				_system_timer;
	memory 				_mem;
	bool				_break_signal;
	exec_dispatcher 	_exec_dispatcher;
	context_switcher 	_ctx_switcher;
	//interrupt_manager _interrupt_manager;


	// TODO: this is not needed here
	uint32_t _initial_pc;
	uint64_t _debug_instruction_counter;
};

#endif //MICROMACHINE_CPU_HPP
