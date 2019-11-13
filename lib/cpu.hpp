//
// Created by fla on 17-5-9.
//

#ifndef MICROMACHINE_CPU_HPP
#define MICROMACHINE_CPU_HPP


#include "types.hpp"
#include "exec.hpp"
#include "instructions.hpp"
#include "memory/memory.hpp"
#include "instruction_pair.hpp"
#include "registers/registers.hpp"
#include "registers/apsr_reg.hpp"
#include "registers/custom/generic_io_reg.hpp"
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
	const exception_state_vector& exceptions() const;
	memory& mem();
	const memory& mem() const;
	registers& regs();
	const registers& regs() const;

	uint64_t debug_instruction_counter() const {
		return _debug_instruction_counter;
	}

	void set_io_callback(generic_io_reg::callback_t callback) {
		_io_reg_callback = callback;
	}

	interrupter& interrupt() {
		return _interrupter;
	}

	exception::priority_t current_execution_priority() const {

		exception::priority_t prio = exception::THREAD_MODE_PRIORITY;
		exception::priority_t boosted_prio = exception::THREAD_MODE_PRIORITY;

		for(size_t i = 2; i < 32; i++) {
			const exception_state& e = _exception_vector.at(i);
			if(!e.is_active()) continue;
			if(e.priority() < prio) {
				prio = e.priority();
			}
		}

		// if primask is set, ignore all maskable exceptions by
		// pretending the executing priority is now 0
		if(_regs.primask_register().pm())
			prio = 0;

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

	void execute(const instruction_pair instr);

	uint32_t get_next_instruction_address(uint32_t instr_addr, instruction_pair instruction) const;

	generic_io_reg::callback_t _io_reg_callback;
	registers 			_regs;
	exception_state_vector _exception_vector;
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
