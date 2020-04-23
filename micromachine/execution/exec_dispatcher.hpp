#pragma once

#include "exception/exception_controller.hpp"
#include "exec.hpp"
#include "instruction/instruction_decoder.hpp"
#include "instruction/instruction_pair.hpp"
#include "interworking_brancher.hpp"
#include "memory/memory.hpp"
#include "registers/core_registers/core_registers.hpp"
#include "registers/special_registers/special_registers.hpp"

namespace micromachine::system {

class exec_dispatcher : public instruction_decoder {
private:
	exception_controller& _exception_controller;
	core_registers& _core_regs;
	special_registers& _special_regs;
	memory& _mem;
	interworking_brancher& _interworking_brancher;
	execution_mode& _execution_mode;
	event_register& _event_register;
	bool& _break_signal;
	bool& _enter_low_power_mode_signal;

public:
	exec_dispatcher(exception_controller& exception_controller,
					core_registers& core_regs,
					special_registers& special_regs,
					memory& mem,
					interworking_brancher& interworking_brancher,
					execution_mode& execution_mode,
					event_register& event_register,
					bool& break_signal,
					bool& enter_low_power_mode_signal)
		: _exception_controller(exception_controller)
		, _core_regs(core_regs)
		, _special_regs(special_regs)
		, _mem(mem)
		, _interworking_brancher(interworking_brancher)
		, _execution_mode(execution_mode)
		, _event_register(event_register)
		, _break_signal(break_signal)
		, _enter_low_power_mode_signal(enter_low_power_mode_signal) {}

private:
	void invalid_instruction(const uint16_t instr) override {
		(void)instr;
		_exception_controller.raise_hardfault();
	}

	void invalid_instruction(const instruction_pair instr) override {
		(void)instr;
		_exception_controller.raise_hardfault();
	}

	void dispatch(const nop) override {
		// do literally nothing
	}
	void dispatch(const yield instruction) override {
		// for os and threading
		exec(instruction);
	}
	void dispatch(const wfe instruction) override {
		exec(instruction, _event_register, _enter_low_power_mode_signal);
	}
	void dispatch(const wfi instruction) override {
		/* If PRIMASK.PM is set to 1, an asynchronous exception that has a higher group priority
		 * than any active exception results in a WFI instruction exit. If the group priority of the
		 * exception is less than or equal to the execution group priority, the exception is
		 * ignored.
		 */
		exec(instruction);
	}
	void dispatch(const sev instruction) override {
		exec(instruction);
	}
	void dispatch(const lsl_imm instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch(const lsr_imm instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch(const asr_imm instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch(const add_reg instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch(const subs_reg instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch(const add_imm instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch(const subs_imm instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch(const mov_imm instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch(const movs instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch(const cmp_imm instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch(const add_imm_t2 instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch(const subs_imm8 instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch(const and_reg instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch(const eor_reg instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch(const lsl_reg instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch(const lsr_reg instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch(const asr_reg instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch(const adc instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch(const sbc instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch(const ror_reg instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch(const tst_reg instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch(const rsb_imm instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch(const cmp_reg instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch(const cmn_reg instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch(const orr_reg instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch(const mul_reg instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch(const bic_reg instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch(const mvn instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch(const add_highreg instruction) override {
		exec(instruction, _core_regs);
	}
	void dispatch(const cmp_highreg instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch(const mov_highreg instruction) override {
		exec(instruction, _core_regs);
	}
	void dispatch(const bx instruction) override {
		exec(instruction, _core_regs, _interworking_brancher);
	}
	void dispatch(const blx instruction) override {
		exec(instruction, _core_regs, _interworking_brancher);
	}
	void dispatch(const ldr_literal instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch(const str_reg instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch(const strh_reg instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch(const strb_reg instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch(const ldrsb_reg instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch(const ldr_reg instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch(const ldrh_reg instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch(const ldrb_reg instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch(const ldrsh_reg instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch(const str_imm instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch(const ldr_imm instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch(const strb_imm instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch(const ldrb_imm instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch(const strh_imm instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch(const ldrh_imm instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch(const str_sp_imm instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch(const ldr_sp_imm instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch(const adr instruction) override {
		exec(instruction, _core_regs);
	}
	void dispatch(const add_sp_imm instruction) override {
		exec(instruction, _core_regs);
	}
	void dispatch(const add_sp_imm_t2 instruction) override {
		exec(instruction, _core_regs);
	}
	void dispatch(const sub_sp_imm instruction) override {
		exec(instruction, _core_regs);
	}
	void dispatch(const sxth instruction) override {
		exec(instruction, _core_regs);
	}
	void dispatch(const sxtb instruction) override {
		exec(instruction, _core_regs);
	}
	void dispatch(const uxth instruction) override {
		exec(instruction, _core_regs);
	}
	void dispatch(const uxtb instruction) override {
		exec(instruction, _core_regs);
	}
	void dispatch(const push instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch(const cps instruction) override {
		exec(instruction, _special_regs);
	}
	void dispatch(const pop instruction) override {
		exec(instruction, _core_regs, _mem, _interworking_brancher);
	}
	void dispatch(const bkpt instruction) override {
		exec(instruction, _break_signal);
	}
	void dispatch(const rev_word instruction) override {
		exec(instruction, _core_regs);
	}
	void dispatch(const rev16 instruction) override {
		exec(instruction, _core_regs);
	}
	void dispatch(const revsh instruction) override {
		exec(instruction, _core_regs);
	}
	void dispatch(const branch instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch(const unconditional_branch instruction) override {
		exec(instruction, _core_regs);
	}
	void dispatch(const stm instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch(const ldm instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch(const mrs instruction) override {
		exec(instruction, _core_regs, _special_regs);
	}
	void dispatch(const msr instruction) override {
		exec(instruction, _core_regs, _special_regs, _execution_mode);
	}
	void dispatch(const bl_imm instruction) override {
		exec(instruction, _core_regs);
	}
	void dispatch(const svc) override {
		_exception_controller.raise_svcall();
	}
	void dispatch(const dmb instruction) override {
		exec(instruction);
	}
	void dispatch(const dsb instruction) override {
		exec(instruction);
	}
	void dispatch(const isb instruction) override {
		exec(instruction);
	}
	void dispatch(const udf instr) override {
		// undefined instruction
		_exception_controller.raise_hardfault();
	}
	void dispatch(const udfw) override {
		// undefined instruction
		_exception_controller.raise_hardfault();
	}
};
} // namespace micromachine::system
