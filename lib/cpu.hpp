//
// Created by fla on 17-5-9.
//

#ifndef MICROMACHINE_CPU_HPP
#define MICROMACHINE_CPU_HPP

#include "types.hpp"
#include "instruction_pair.hpp"
#include "registers/registers.hpp"
#include "registers/apsr_reg.hpp"
#include "exec.hpp"
#include "instructions.hpp"
#include "exec.hpp"
#include "memory.hpp"
#include "registers/exec_mode_reg.hpp"
#include "exception_vector.hpp"
#include "exception_manager.hpp"
#include "exec_dispatcher.hpp"
#include "disasm.hpp"
#include "timer.hpp"

class cpu {



public:

	cpu();

	void reset();
	instruction_pair fetch_instruction(word address) const;
	instruction_pair fetch_instruction_debug(word address) const;
	bool step();
	const exception_vector& exceptions() const;
	memory& mem();
	const memory& mem() const;
	registers& regs();
	const registers& regs() const;

	bool load_elf(const std::string& path);

	uint64_t debug_instruction_counter() const {
		return _debug_instruction_counter;
	}

private:
	void execute(const instruction_pair instr);

	void enter_handler_mode();
	void enter_thread_mode();


	exception_vector	_exception_vector;
	registers 			_regs;
	sphr2_reg 			_sphr2_reg;
	sphr3_reg 			_sphr3_reg;
	systick				_system_timer;
	memory 				_mem;
	bool				_break_signal;
	exec_dispatcher 	_exec_dispatcher;
	exception_manager 	_exception_manager;

	// TODO: this is not needed here
	uint32_t _initial_sp;
	uint32_t _initial_pc;
	uint64_t _debug_instruction_counter;


};

#endif //MICROMACHINE_CPU_HPP
