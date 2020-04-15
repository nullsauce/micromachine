/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#include "cpu.hpp"
#include <thread>

cpu::cpu()
	: _regs(_ctx_switcher)
	, _exception_vector(_nvic, _shpr2_reg, _shpr3_reg)
	, _interrupter(_exception_vector)
	, _generic_io_reg(std::ref(_io_reg_callback))
	, _system_timer(_interrupter)
	, _mem(_interrupter,{
		std::make_pair(shpr2_reg::SHPR2, std::ref(_shpr2_reg)),
		std::make_pair(shpr3_reg::SHPR3, std::ref(_shpr3_reg)),
		std::make_pair(systick_control_reg::SYST_CSR, std::ref(_system_timer.control_register())),
		std::make_pair(systick_control_reg::SYST_RVR, std::ref(_system_timer.reload_value_register())),
		std::make_pair(systick_control_reg::SYST_CVR, std::ref(_system_timer.current_value_register())),
		std::make_pair(systick_control_reg::SYST_CALIB, std::ref(_system_timer.calib_value_register())),
		std::make_pair(generic_io_reg::GIO_IO, std::ref(_generic_io_reg)),
		std::make_pair(nvic::NVIC_ISER, std::ref(_nvic.iser_reg())),
		std::make_pair(nvic::NVIC_ICER, std::ref(_nvic.icer_reg())),
		std::make_pair(nvic::NVIC_ISPR, std::ref(_nvic.ispr_reg())),
		std::make_pair(nvic::NVIC_ICPR, std::ref(_nvic.icpr_reg())),
		std::make_pair(nvic::NVIC_IPR0, std::ref(_nvic.priority_reg<0>())),
		std::make_pair(nvic::NVIC_IPR1, std::ref(_nvic.priority_reg<1>())),
		std::make_pair(nvic::NVIC_IPR2, std::ref(_nvic.priority_reg<2>())),
		std::make_pair(nvic::NVIC_IPR3, std::ref(_nvic.priority_reg<3>())),
		std::make_pair(nvic::NVIC_IPR4, std::ref(_nvic.priority_reg<4>())),
		std::make_pair(nvic::NVIC_IPR5, std::ref(_nvic.priority_reg<5>())),
		std::make_pair(nvic::NVIC_IPR6, std::ref(_nvic.priority_reg<6>())),
		std::make_pair(nvic::NVIC_IPR7, std::ref(_nvic.priority_reg<7>())),
	})
	, _break_signal(false)
	, _exec_dispatcher(_regs, _mem, _interrupter, _break_signal)
	, _ctx_switcher(_regs, _mem, _exception_vector)
//	, _interrupt_manager(_mem, _nvic, _sphr2_reg, _sphr3_reg)
	, _debug_instruction_counter(0)
{
#ifdef MICROMACHINE_ENABLE_PRECOND_CHECKS
	fprintf(stderr, "Warning: The CPU is compiled with addtional safety checks that might slow its performance.\n");
#endif
}


void cpu::execute(instruction_pair instr)
{
	_exec_dispatcher.dispatch_instruction(instr);
}

void cpu::reset(uint32_t initial_pc) {

	uint32_t initial_sp_main = _mem.read32_unchecked(0U) & 0xFFFFFFFC;

	_break_signal = false;
	_regs.exec_mode_register().set_thread_mode();
	_regs.reset();
	_regs.app_status_register().reset();
	_regs.set_sp(initial_sp_main);
	_regs.set_pc(initial_pc);
	_system_timer.reset();
	_shpr2_reg.reset();
	_shpr3_reg.reset();

	// set sp to vector+0
	/*
	uint32_t stack_pointer = _mem.read32(0);
	_regs.set_sp(stack_pointer);
	fprintf(stderr, "reset handler : %08x\n", stack_pointer);

	// branch to entry point
	word reset_handler = _mem.read32(4);
	fprintf(stderr, "reset handler : %08x\n", reset_handler);
	_regs.branch_link_interworking(reset_handler);*/


	/*
	SP_main = MemA[vectortable,4] & 0xFFFFFFFC;
	SP_process = ((bits(30) UNKNOWN):’00’);
	LR = bits(32) UNKNOWN; // Value must be initialised by software
	CurrentMode = Mode_Thread;
	APSR = bits(32) UNKNOWN; // Flags UNPREDICTABLE from reset
	IPSR<5:0> = 0x0; // exception number clearedat reset
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

instruction_pair cpu::fetch_instruction(uint32_t address) const {
	return {_mem.read32_unchecked(address)};
}

cpu::State cpu::step() {

	if(_break_signal) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		return cpu::State::BREAK;
	}
	_debug_instruction_counter++;

	_system_timer.tick();

	const uint32_t cur_instruction_address = _regs.get_pc();
	instruction_pair cur_intruction = fetch_instruction(cur_instruction_address);

	/*
	fprintf(stderr, "S %08x: %s\n",
		cur_instruction_address,
		disasm::disassemble_instruction(cur_intruction, cur_instruction_address).c_str()
	);*/

	if(!_regs.execution_status_register().thumb_bit_set()) {
		// Thumb bit not set
		// all instructions in this state are UNDEFINED .
		_interrupter.raise_hardfault();
	}

	exception_state* ex = next_exception_to_take();

	if(nullptr == ex) {
		// execute instruction at current PC
		// simulate prefetch of 2 instructions during execution
		_regs.set_pc(cur_instruction_address + 4);
		_regs.reset_pc_dirty_status();
		execute(cur_intruction);

		ex = next_exception_to_take();
	}

	uint32_t next_instruction_address = get_next_instruction_address(cur_instruction_address, cur_intruction);

	if(ex) {
		// an exception to be taken is pending
		_regs.set_pc(cur_instruction_address);
		_ctx_switcher.exception_entry(*ex, cur_instruction_address, cur_intruction, next_instruction_address);
	} else {
		// no exception taken, execution will continue at next address
		_regs.set_pc(next_instruction_address);
	}

	return cpu::State::RUN;
}

uint32_t cpu::get_next_instruction_address(uint32_t instr_addr, instruction_pair instruction) const {
	if(!_regs.branch_occured()) {
		return instr_addr + instruction.size();
	} else {
		return _regs.get_pc();
	}
}

const exception_state_vector& cpu::exceptions() const {
	return _exception_vector;
}

memory& cpu::mem() {
	return _mem;
}

const memory& cpu::mem() const {
	return _mem;
}

registers& cpu::regs() {
	return _regs;
}

const registers& cpu::regs() const {
	return _regs;
}


