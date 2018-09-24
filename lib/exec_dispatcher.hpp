#ifndef MICROMACHINE_INTRUCTION_DISPATCHER_HPP
#define MICROMACHINE_INTRUCTION_DISPATCHER_HPP

#include "dispatcher.hpp"
#include "exec.hpp"
#include "exception_vector.hpp"
#include "memory.hpp"
#include "registers/registers.hpp"
#include "registers/apsr_reg.hpp"

class exec_dispatcher : public dispatcher {
public:
	exec_dispatcher(
		registers& regs,
		memory& mem,
		exception_vector& exceptions,
		bool& break_signal)
	: _exception_vector(exceptions)
	, _regs(regs)
	, _mem(mem)
	, _break_signal(break_signal)
	{}


private:

	exception_vector& _exception_vector;
	registers& _regs;
	memory& _mem;
	bool& _break_signal;

	void invalid_instruction(const halfword instr) override {
		(void)instr;
		_exception_vector.raise(exception_number::ex_name::HARDFAULT);
	}

	void invalid_instruction(const instruction_pair instr) override {
		(void)instr;
		_exception_vector.raise(exception_number::ex_name::HARDFAULT);
	}

	//TODO: refactor and avoid passing _regs.app_status_register() explicitely
	void dispatch(const nop instruction) override {

	}
	void dispatch(const lsl_imm instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const lsr_imm instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const asr_imm instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const add_reg instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const subs_reg instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const add_imm instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const subs_imm instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const mov_imm instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const movs instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const cmp_imm instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const add_imm_t2 instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const subs_imm8 instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const and_reg instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const eor_reg instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const lsl_reg instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const lsr_reg instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const asr_reg instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const adc instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const sbc instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const ror_reg instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const tst_reg instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const rsb_imm instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const cmp_reg instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const cmn_reg instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const orr_reg instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const mul_reg instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const bic_reg instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const not_reg instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const add_highreg instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const cmp_highreg instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const mov_highreg instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const bx instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const blx instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const ldr_literal instruction) override {
		exec(instruction, _regs, _regs.app_status_register(), _mem);
	}
	void dispatch(const str_reg instruction) override {
		exec(instruction, _regs, _regs.app_status_register(), _mem);
	}
	void dispatch(const strh_reg instruction) override {
		exec(instruction, _regs,
			 _regs.app_status_register(), _mem);
	}
	void dispatch(const strb_reg instruction) override {
		exec(instruction, _regs, _regs.app_status_register(), _mem);
	}
	void dispatch(const ldrsb_reg instruction) override {
		exec(instruction, _regs, _regs.app_status_register(), _mem);
	}
	void dispatch(const ldr_reg instruction) override {
		exec(instruction, _regs, _regs.app_status_register(), _mem);
	}
	void dispatch(const ldrh_reg instruction) override {
		exec(instruction, _regs,
			 _regs.app_status_register(), _mem);
	}
	void dispatch(const ldrb_reg instruction) override {
		exec(instruction, _regs, _regs.app_status_register(), _mem);
	}
	void dispatch(const ldrsh_reg instruction) override {
		exec(instruction, _regs,
			 _regs.app_status_register(), _mem);
	}
	void dispatch(const str_imm instruction) override {
		exec(instruction, _regs, _regs.app_status_register(), _mem);
	}
	void dispatch(const ldr_imm instruction) override {
		exec(instruction, _regs, _regs.app_status_register(), _mem);
	}
	void dispatch(const strb_imm instruction) override {
		exec(instruction, _regs, _regs.app_status_register(), _mem);
	}
	void dispatch(const ldrb_imm instruction) override {
		exec(instruction, _regs, _regs.app_status_register(), _mem);
	}
	void dispatch(const strh_imm instruction) override {
		exec(instruction, _regs, _regs.app_status_register(), _mem);
	}
	void dispatch(const ldrh_imm instruction) override {
		exec(instruction, _regs, _regs.app_status_register(), _mem);
	}
	void dispatch(const str_sp_imm instruction) override {
		exec(instruction, _regs, _regs.app_status_register(), _mem);
	}
	void dispatch(const ldr_sp_imm instruction) override {
		exec(instruction, _regs, _regs.app_status_register(), _mem);
	}
	void dispatch(const adr instruction) override {
		exec(instruction, _regs);
	}
	void dispatch(const add_sp_imm instruction) override {
		exec(instruction, _regs);
	}
	void dispatch(const add_sp_imm_t2 instruction) override {
		exec(instruction, _regs);
	}
	void dispatch(const sub_sp_imm instruction) override {
		exec(instruction, _regs);
	}
	void dispatch(const sxth instruction) override {
		exec(instruction, _regs);
	}
	void dispatch(const sxtb instruction) override {
		exec(instruction, _regs);
	}
	void dispatch(const uxth instruction) override {
		exec(instruction, _regs);
	}
	void dispatch(const uxtb instruction) override {
		exec(instruction, _regs);
	}
	void dispatch(const push instruction) override {
		exec(instruction, _regs, _mem);
	}
	void dispatch(const cps instruction) override {
		exec(instruction, _regs);
	}
	void dispatch(const pop instruction) override {
		exec(instruction, _regs, _mem);
	}
	void dispatch(const bkpt instruction) override {
		exec(instruction, _break_signal);
	}
	void dispatch(const rev_word instruction) override {
		exec(instruction, _regs);
	}
	void dispatch(const rev16 instruction) override {
		exec(instruction, _regs);
	}
	void dispatch(const revsh instruction) override {
		exec(instruction, _regs);
	}
	void dispatch(const branch instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const unconditional_branch instruction) override {
		exec(instruction, _regs,
			 _regs.app_status_register());
	}
	void dispatch(const stm instruction) override {
		exec(instruction, _regs, _mem);
	}
	void dispatch(const ldm instruction) override {
		exec(instruction, _regs, _mem);
	}
	void dispatch(const mrs instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const msr instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const bl_imm instruction) override {
		exec(instruction, _regs);
	}
	void dispatch(const svc instruction) override {
		_exception_vector.raise(exception_number::ex_name::SVCALL);
	}
	void dispatch(const udf instr) override {
		(void)instr;
		// undefined instruction
		_exception_vector.raise(exception_number::ex_name::HARDFAULT);
	}
	void dispatch(const udfw instr) override {
		// undefined instruction
		_exception_vector.raise(exception_number::ex_name::HARDFAULT);
	}
};

#endif //MICROMACHINE_INTRUCTION_DISPATCHER_HPP
