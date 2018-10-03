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

	void set_io_callback(generic_io_reg::callback_t callback) {
		_io_reg_callback = callback;
	}

private:
	void execute(const instruction_pair instr);

	void enter_handler_mode();
	void enter_thread_mode();
	uint32_t get_next_instruction_address(uint32_t instr_addr, instruction_pair instruction) const;
	uint32_t get_next_instruction_address() const;


	exception_vector	_exception_vector;
	generic_io_reg::callback_t _io_reg_callback;
	registers 			_regs;
	sphr2_reg 			_sphr2_reg;
	sphr3_reg 			_sphr3_reg;
	generic_io_reg		_generic_io_reg;
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
