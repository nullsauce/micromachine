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
		std::make_pair(cpuid_reg::CPUID, std::ref(_cpuid_reg)),
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
	, _enter_low_power_mode_signal(false)
	, _exec_dispatcher(_regs, _mem, _interrupter, _event_register, _break_signal, _enter_low_power_mode_signal)
	, _ctx_switcher(_regs, _mem, _exception_vector)
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
	_regs.reset();
	_regs.set_sp(initial_sp_main);
	_regs.set_pc(initial_pc);
	_regs.set_lr(0);
	_regs.exec_mode_register().set_thread_mode();
	_regs.app_status_register().reset();
	_regs.execution_status_register().set_thumb_bit(true);
	_regs.interrupt_status_register().reset();
	_regs.primask_register().reset();
	_regs.control_register().reset();
	_exception_vector.reset();
	_event_register.clear();
	_system_timer.reset();
	_shpr2_reg.reset();
	_shpr3_reg.reset();
	_nvic.reset();
	_break_signal = false;
	_enter_low_power_mode_signal = false;
	_debug_instruction_counter = 0;
}

instruction_pair cpu::fetch_instruction(uint32_t address) const {
	return {_mem.read32_unchecked(address)};
}

cpu::State cpu::step() {

	if(_break_signal) {
		return cpu::State::BREAK;
	}
	_debug_instruction_counter++;

	// update timer
	_system_timer.tick();


	const uint32_t cur_instruction_address = _regs.get_pc();
	instruction_pair cur_intruction = fetch_instruction(cur_instruction_address);

	if(!_regs.execution_status_register().thumb_bit_set()) {
		// Thumb bit not set
		// all instructions in this state are UNDEFINED .
		_interrupter.raise_hardfault();
	}

	// check for asynchronous interrupt
	exception_state* ex = next_exception_to_take();

	if(nullptr == ex) {
		// execute instruction at current PC
		// simulate prefetch of 2 instructions during execution
		_regs.set_pc(cur_instruction_address + 4);
		_regs.reset_pc_dirty_status();
		execute(cur_intruction);

		// check for synchronous exception that might
		// have been raised during the execution
		ex = next_exception_to_take();
	}
	uint32_t next_instruction_address = get_next_instruction_address(cur_instruction_address, cur_intruction);

	if(ex) {

		// wake up event for wfe or wfi

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


