#pragma once

#include "exception/exception_controller.hpp"
#include "exec.hpp"
#include "instruction/fast_instruction_decoder.hpp"
#include "instruction/instruction_decoder.hpp"
#include "instruction/instruction_pair.hpp"
#include "interworking_brancher.hpp"
#include "memory/memory.hpp"
#include "registers/core_registers/core_registers.hpp"
#include "registers/special_registers/special_registers.hpp"

namespace micromachine::system {

class exec_dispatcher : public fast_instruction_decoder {
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

	void dispatch_nop(const nop) override {
		// do literally nothing
	}
	void dispatch_yield(const yield instruction) override {
		// for os and threading
		exec(instruction);
	}
	void dispatch_wfe(const wfe instruction) override {
		exec(instruction, _event_register, _enter_low_power_mode_signal);
	}
	void dispatch_wfi(const wfi instruction) override {
		/* If PRIMASK.PM is set to 1, an asynchronous exception that has a higher group priority
		 * than any active exception results in a WFI instruction exit. If the group priority of the
		 * exception is less than or equal to the execution group priority, the exception is
		 * ignored.
		 */
		exec(instruction);
	}
	void dispatch_sev(const sev instruction) override {
		exec(instruction);
	}
	void dispatch_lsl_imm(const lsl_imm instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch_lsr_imm(const lsr_imm instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch_asr_imm(const asr_imm instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch_add_reg(const add_reg instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch_subs_reg(const subs_reg instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch_add_imm(const add_imm instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch_subs_imm(const subs_imm instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch_mov_imm(const mov_imm instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch_movs(const movs instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch_cmp_imm(const cmp_imm instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch_add_imm_t2(const add_imm_t2 instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch_subs_imm8(const subs_imm8 instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch_and_reg(const and_reg instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch_eor_reg(const eor_reg instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch_lsl_reg(const lsl_reg instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch_lsr_reg(const lsr_reg instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch_asr_reg(const asr_reg instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch_adc(const adc instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch_sbc(const sbc instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch_ror_reg(const ror_reg instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch_tst_reg(const tst_reg instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch_rsb_imm(const rsb_imm instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch_cmp_reg(const cmp_reg instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch_cmn_reg(const cmn_reg instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch_orr_reg(const orr_reg instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch_mul_reg(const mul_reg instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch_bic_reg(const bic_reg instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch_mvn(const mvn instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch_add_highreg(const add_highreg instruction) override {
		exec(instruction, _core_regs);
	}
	void dispatch_cmp_highreg(const cmp_highreg instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch_mov_highreg(const mov_highreg instruction) override {
		exec(instruction, _core_regs);
	}
	void dispatch_bx(const bx instruction) override {
		exec(instruction, _core_regs, _interworking_brancher);
	}
	void dispatch_blx(const blx instruction) override {
		exec(instruction, _core_regs, _interworking_brancher);
	}
	void dispatch_ldr_literal(const ldr_literal instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch_str_reg(const str_reg instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch_strh_reg(const strh_reg instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch_strb_reg(const strb_reg instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch_ldrsb_reg(const ldrsb_reg instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch_ldr_reg(const ldr_reg instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch_ldrh_reg(const ldrh_reg instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch_ldrb_reg(const ldrb_reg instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch_ldrsh_reg(const ldrsh_reg instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch_str_imm(const str_imm instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch_ldr_imm(const ldr_imm instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch_strb_imm(const strb_imm instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch_ldrb_imm(const ldrb_imm instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch_strh_imm(const strh_imm instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch_ldrh_imm(const ldrh_imm instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch_str_sp_imm(const str_sp_imm instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch_ldr_sp_imm(const ldr_sp_imm instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch_adr(const adr instruction) override {
		exec(instruction, _core_regs);
	}
	void dispatch_add_sp_imm(const add_sp_imm instruction) override {
		exec(instruction, _core_regs);
	}
	void dispatch_add_sp_imm_t2(const add_sp_imm_t2 instruction) override {
		exec(instruction, _core_regs);
	}
	void dispatch_sub_sp_imm(const sub_sp_imm instruction) override {
		exec(instruction, _core_regs);
	}
	void dispatch_sxth(const sxth instruction) override {
		exec(instruction, _core_regs);
	}
	void dispatch_sxtb(const sxtb instruction) override {
		exec(instruction, _core_regs);
	}
	void dispatch_uxth(const uxth instruction) override {
		exec(instruction, _core_regs);
	}
	void dispatch_uxtb(const uxtb instruction) override {
		exec(instruction, _core_regs);
	}
	void dispatch_push(const push instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch_cps(const cps instruction) override {
		exec(instruction, _special_regs);
	}
	void dispatch_pop(const pop instruction) override {
		exec(instruction, _core_regs, _mem, _interworking_brancher);
	}
	void dispatch_bkpt(const bkpt instruction) override {
		exec(instruction, _break_signal);
	}
	void dispatch_rev_word(const rev_word instruction) override {
		exec(instruction, _core_regs);
	}
	void dispatch_rev16(const rev16 instruction) override {
		exec(instruction, _core_regs);
	}
	void dispatch_revsh(const revsh instruction) override {
		exec(instruction, _core_regs);
	}
	void dispatch_branch(const branch instruction) override {
		exec(instruction, _core_regs, _special_regs.app_status_register());
	}
	void dispatch_unconditional_branch(const unconditional_branch instruction) override {
		exec(instruction, _core_regs);
	}
	void dispatch_stm(const stm instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch_ldm(const ldm instruction) override {
		exec(instruction, _core_regs, _mem);
	}
	void dispatch_mrs(const mrs instruction) override {
		exec(instruction, _core_regs, _special_regs);
	}
	void dispatch_msr(const msr instruction) override {
		exec(instruction, _core_regs, _special_regs, _execution_mode);
	}
	void dispatch_bl_imm(const bl_imm instruction) override {
		exec(instruction, _core_regs);
	}
	void dispatch_svc(const svc) override {
		_exception_controller.raise_svcall();
	}
	void dispatch_dmb(const dmb instruction) override {
		exec(instruction);
	}
	void dispatch_dsb(const dsb instruction) override {
		exec(instruction);
	}
	void dispatch_isb(const isb instruction) override {
		exec(instruction);
	}
	void dispatch_udf(const udf instr) override {
		// undefined instruction
		_exception_controller.raise_hardfault();
	}
	void dispatch_udfw(const udfw) override {
		// undefined instruction
		_exception_controller.raise_hardfault();
	}
};
} // namespace micromachine::system
