//
// Created by fla on 17-5-9.
//

#ifndef MICROMACHINE_CPU_HPP
#define MICROMACHINE_CPU_HPP


#include <utility>

#include "types.hpp"
#include "exec.hpp"
#include "instructions.hpp"
#include "memory/memory.hpp"
#include "instruction_pair.hpp"
#include "registers/registers.hpp"
#include "registers/apsr_reg.hpp"
#include "registers/custom/generic_io_reg.hpp"
#include "registers/exec_mode_reg.hpp"
#include "registers/event_register.hpp"
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

	void reset(uint32_t initial_pc);
	instruction_pair fetch_instruction(uint32_t address) const;
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
		_io_reg_callback = std::move(callback);
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

	cpu(const cpu& existing_cpu)
		: _io_reg_callback(existing_cpu._io_reg_callback)
		, _regs(_ctx_switcher, existing_cpu.regs())
		, _exception_vector(_nvic, _shpr2_reg, _shpr3_reg, existing_cpu._exception_vector)
		, _interrupter(_exception_vector)
		, _nvic(existing_cpu._nvic)
		, _shpr2_reg(existing_cpu._shpr2_reg)
		, _shpr3_reg(existing_cpu._shpr3_reg)
		, _generic_io_reg(std::ref(_io_reg_callback))
		, _event_register(existing_cpu._event_register)
		, _system_timer(_interrupter, existing_cpu._system_timer)
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
		, _break_signal(existing_cpu._break_signal)
		, _enter_low_power_mode_signal(existing_cpu._enter_low_power_mode_signal)
		, _exec_dispatcher(_regs, _mem, _interrupter, _event_register, _break_signal, _enter_low_power_mode_signal)
		, _ctx_switcher(_regs, _mem, _exception_vector)
		, _debug_instruction_counter(existing_cpu._debug_instruction_counter)
	{}

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

	void execute(instruction_pair instr);

	uint32_t get_next_instruction_address(uint32_t instr_addr, instruction_pair instruction) const;

	generic_io_reg::callback_t _io_reg_callback;
	registers 			_regs;
	exception_state_vector _exception_vector;
	interrupter			_interrupter;
	nvic				_nvic;
	shpr2_reg 			_shpr2_reg;
	shpr3_reg 			_shpr3_reg;
	generic_io_reg		_generic_io_reg;
	event_register		_event_register;
	systick				_system_timer;
	memory 				_mem;
	bool				_break_signal;
	bool 				_enter_low_power_mode_signal;
	exec_dispatcher 	_exec_dispatcher;
	context_switcher 	_ctx_switcher;

	// TODO: this is not needed here
	uint64_t _debug_instruction_counter;

};

#endif //MICROMACHINE_CPU_HPP
