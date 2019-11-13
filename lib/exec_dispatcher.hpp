#ifndef MICROMACHINE_INTRUCTION_DISPATCHER_HPP
#define MICROMACHINE_INTRUCTION_DISPATCHER_HPP

#include "dispatcher.hpp"
#include "exec.hpp"
#include "memory/memory.hpp"
#include "registers/registers.hpp"
#include "registers/apsr_reg.hpp"
#include "interrupter.hpp"

class exec_dispatcher : public dispatcher {
public:
	exec_dispatcher(
		registers& regs,
		memory& mem,
		interrupter& interrupter,
		bool& break_signal)
	: _interrupter(interrupter)
	, _regs(regs)
	, _mem(mem)
	, _break_signal(break_signal)
	{}


private:

	interrupter& _interrupter;
	registers& _regs;
	memory& _mem;
	bool& _break_signal;

	void invalid_instruction(const uint16_t instr) override {
		(void)instr;
		_interrupter.raise_hardfault();
	}

	void invalid_instruction(const instruction_pair instr) override {
		(void)instr;
		_interrupter.raise_hardfault();
	}

	//TODO: refactor and avoid passing _regs.app_status_register() explicitely
	void dispatch(const nop) override {
		// do literally nothing
	}
	void dispatch(const yield) override {
		// for os and threading
	}
	void dispatch(const wfe) override {
		// wait for event in event register
		/* TODO: Implement WFE event sources as specified below
		The following events are WFE wake-up events:
		- the execution of an SEV instruction on any other processor in a multiprocessor system
		- any exception entering the pending state if SEVONPEND in the System Control Register is set to 1
		- an asynchronous exception at a priority that preempts any currently active exceptions
		- a debug event with debug enabled.
		*/
	}
	void dispatch(const wfi) override {
		// wait for interrupt
	}
	void dispatch(const sev) override {
		// causes an event to be signaled to all processors in a
		// multiprocessor system
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
	void dispatch(const mvn instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const add_highreg instruction) override {
		exec(instruction, _regs);
	}
	void dispatch(const cmp_highreg instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const mov_highreg instruction) override {
		exec(instruction, _regs);
	}
	void dispatch(const bx instruction) override {
		exec(instruction, _regs);
	}
	void dispatch(const blx instruction) override {
		exec(instruction, _regs);
	}
	void dispatch(const ldr_literal instruction) override {
		exec(instruction, _regs, _mem);
	}
	void dispatch(const str_reg instruction) override {
		exec(instruction, _regs, _mem);
	}
	void dispatch(const strh_reg instruction) override {
		exec(instruction, _regs, _mem);
	}
	void dispatch(const strb_reg instruction) override {
		exec(instruction, _regs, _mem);
	}
	void dispatch(const ldrsb_reg instruction) override {
		exec(instruction, _regs, _mem);
	}
	void dispatch(const ldr_reg instruction) override {
		exec(instruction, _regs, _mem);
	}
	void dispatch(const ldrh_reg instruction) override {
		exec(instruction, _regs, _mem);
	}
	void dispatch(const ldrb_reg instruction) override {
		exec(instruction, _regs, _mem);
	}
	void dispatch(const ldrsh_reg instruction) override {
		exec(instruction, _regs, _mem);
	}
	void dispatch(const str_imm instruction) override {
		exec(instruction, _regs, _mem);
	}
	void dispatch(const ldr_imm instruction) override {
		exec(instruction, _regs, _mem);
	}
	void dispatch(const strb_imm instruction) override {
		exec(instruction, _regs, _mem);
	}
	void dispatch(const ldrb_imm instruction) override {
		exec(instruction, _regs, _mem);
	}
	void dispatch(const strh_imm instruction) override {
		exec(instruction, _regs, _mem);
	}
	void dispatch(const ldrh_imm instruction) override {
		exec(instruction, _regs, _mem);
	}
	void dispatch(const str_sp_imm instruction) override {
		exec(instruction, _regs, _mem);
	}
	void dispatch(const ldr_sp_imm instruction) override {
		exec(instruction, _regs, _mem);
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
		exec(instruction, _regs);
	}
	void dispatch(const stm instruction) override {
		exec(instruction, _regs, _mem);
	}
	void dispatch(const ldm instruction) override {
		exec(instruction, _regs, _mem);
	}
	void dispatch(const mrs instruction) override {
		exec(instruction, _regs);
	}
	void dispatch(const msr instruction) override {
		exec(instruction, _regs, _regs.app_status_register());
	}
	void dispatch(const bl_imm instruction) override {
		exec(instruction, _regs);
	}
	void dispatch(const svc) override {
		_interrupter.raise_svcall();
	}
	void dispatch(const udf instr) override {
		// undefined instruction
		_interrupter.raise_hardfault();
	}
	void dispatch(const udfw) override {
		// undefined instruction
		_interrupter.raise_hardfault();
	}
};

#endif //MICROMACHINE_INTRUCTION_DISPATCHER_HPP
