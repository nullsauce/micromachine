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
		exception_vector& exceptions)
	: _exception_vector(exceptions)
	, _regs(regs)
	, _mem(mem)
	{}


private:

	exception_vector& _exception_vector;
	registers& _regs;
	memory& _mem;

	void invalid_instruction(const halfword& instr) override {
		(void)instr;
		_exception_vector.raise(exception_type::HARDFAULT_PRECISE);
	}

	void invalid_instruction(const instruction_pair& instr) override {
		(void)instr;
		_exception_vector.raise(exception_type::HARDFAULT_PRECISE);
	}

	//TODO: refactor and avoid passing _regs.app_status_register() explicitely
	void dispatch(const nop& instruction) override {
	
	}
	void dispatch(const lsl_imm& instruction) override {
		exec(lsl_imm(instruction), _regs, _regs.app_status_register());
	}
	void dispatch(const lsr_imm& instruction) override {
		exec(lsr_imm(instruction), _regs, _regs.app_status_register());
	}
	void dispatch(const asr_imm& instruction) override {
		exec(asr_imm(instruction), _regs, _regs.app_status_register());
	}
	void dispatch(const add_reg& instruction) override {
		exec(add_reg(instruction), _regs, _regs.app_status_register());
	}
	void dispatch(const subs_reg& instruction) override {
		exec(subs_reg(instruction), _regs, _regs.app_status_register());
	}
	void dispatch(const add_imm& instruction) override {
		exec(add_imm(instruction), _regs, _regs.app_status_register());
	}
	void dispatch(const subs_imm& instruction) override {
		exec(subs_imm(instruction), _regs, _regs.app_status_register());
	}
	void dispatch(const mov_imm& instruction) override {
		exec(mov_imm(instruction), _regs, _regs.app_status_register());
	}
	void dispatch(const movs& instruction) override {
		exec(movs(instruction), _regs, _regs.app_status_register());
	}
	void dispatch(const cmp_imm& instruction) override {
		exec(cmp_imm(instruction), _regs, _regs.app_status_register());
	}
	void dispatch(const add_imm_t2& instruction) override {
		exec(add_imm_t2(instruction), _regs, _regs.app_status_register());
	}
	void dispatch(const subs_imm8& instruction) override {
		exec(subs_imm8(instruction), _regs, _regs.app_status_register());
	}
	void dispatch(const and_reg& instruction) override {
		exec(and_reg(instruction), _regs, _regs.app_status_register());
	}
	void dispatch(const eor_reg& instruction) override {
		exec(eor_reg(instruction), _regs, _regs.app_status_register());
	}
	void dispatch(const lsl_reg& instruction) override {
		exec(lsl_reg(instruction), _regs, _regs.app_status_register());
	}
	void dispatch(const lsr_reg& instruction) override {
		exec(lsr_reg(instruction), _regs, _regs.app_status_register());
	}
	void dispatch(const asr_reg& instruction) override {
		exec(asr_reg(instruction), _regs, _regs.app_status_register());
	}
	void dispatch(const adc& instruction) override {
		exec(adc(instruction), _regs, _regs.app_status_register());
	}
	void dispatch(const sbc& instruction) override {
		exec(sbc(instruction), _regs, _regs.app_status_register());
	}
	void dispatch(const ror_reg& instruction) override {
		exec(ror_reg(instruction), _regs, _regs.app_status_register());
	}
	void dispatch(const tst_reg& instruction) override {
		exec(tst_reg(instruction), _regs, _regs.app_status_register());
	}
	void dispatch(const rsb_imm& instruction) override {
		exec(rsb_imm(instruction), _regs, _regs.app_status_register());
	}
	void dispatch(const cmp_reg& instruction) override {
		exec(cmp_reg(instruction), _regs, _regs.app_status_register());
	}
	void dispatch(const cmn_reg& instruction) override {
		exec(cmn_reg(instruction), _regs, _regs.app_status_register());
	}
	void dispatch(const orr_reg& instruction) override {
		exec(orr_reg(instruction), _regs, _regs.app_status_register());
	}
	void dispatch(const mul_reg& instruction) override {
		exec(mul_reg(instruction), _regs, _regs.app_status_register());
	}
	void dispatch(const bic_reg& instruction) override {
		exec(bic_reg(instruction), _regs, _regs.app_status_register());
	}
	void dispatch(const not_reg& instruction) override {
		exec(not_reg(instruction), _regs, _regs.app_status_register());
	}
	void dispatch(const add_highreg& instruction) override {
		exec(add_highreg(instruction), _regs, _regs.app_status_register());
	}
	void dispatch(const cmp_highreg& instruction) override {
		exec(cmp_highreg(instruction), _regs, _regs.app_status_register());
	}
	void dispatch(const mov_highreg& instruction) override {
		exec(mov_highreg(instruction), _regs, _regs.app_status_register());
	}
	void dispatch(const bx& instruction) override {
		exec(bx(instruction), _regs, _regs.app_status_register());
	}
	void dispatch(const blx& instruction) override {
		exec(blx(instruction), _regs, _regs.app_status_register());
	}
	void dispatch(const ldr_literal& instruction) override {
		exec(ldr_literal(instruction), _regs, _regs.app_status_register(), _mem);
	}
	void dispatch(const str_reg& instruction) override {
		exec(str_reg(instruction), _regs, _regs.app_status_register(), _mem);
	}
	void dispatch(const strh_reg& instruction) override {
		exec(strh_reg(instruction), _regs,
			 _regs.app_status_register(), _mem);
	}
	void dispatch(const strb_reg& instruction) override {
		exec(strb_reg(instruction), _regs, _regs.app_status_register(), _mem);
	}
	void dispatch(const ldrsb_reg& instruction) override {
		exec(ldrsb_reg(instruction), _regs, _regs.app_status_register(), _mem);
	}
	void dispatch(const ldr_reg& instruction) override {
		exec(ldr_reg(instruction), _regs, _regs.app_status_register(), _mem);
	}
	void dispatch(const ldrh_reg& instruction) override {
		exec(ldrh_reg(instruction), _regs,
			 _regs.app_status_register(), _mem);
	}
	void dispatch(const ldrb_reg& instruction) override {
		exec(ldrb_reg(instruction), _regs, _regs.app_status_register(), _mem);
	}
	void dispatch(const ldrsh_reg& instruction) override {
		exec(ldrsh_reg(instruction), _regs,
			 _regs.app_status_register(), _mem);
	}
	void dispatch(const str_imm& instruction) override {
		exec(str_imm(instruction), _regs, _regs.app_status_register(), _mem);
	}
	void dispatch(const ldr_imm& instruction) override {
		exec(ldr_imm(instruction), _regs, _regs.app_status_register(), _mem);
	}
	void dispatch(const strb_imm& instruction) override {
		exec(strb_imm(instruction), _regs, _regs.app_status_register(), _mem);
	}
	void dispatch(const ldrb_imm& instruction) override {
		exec(ldrb_imm(instruction), _regs, _regs.app_status_register(), _mem);
	}
	void dispatch(const strh_imm& instruction) override {
		exec(strh_imm(instruction), _regs, _regs.app_status_register(), _mem);
	}
	void dispatch(const ldrh_imm& instruction) override {
		exec(ldrh_imm(instruction), _regs, _regs.app_status_register(), _mem);
	}
	void dispatch(const str_sp_imm& instruction) override {
		exec(str_sp_imm(instruction), _regs, _regs.app_status_register(), _mem);
	}
	void dispatch(const ldr_sp_imm& instruction) override {
		exec(ldr_sp_imm(instruction), _regs, _regs.app_status_register(), _mem);
	}
	void dispatch(const adr& instruction) override {
		exec(adr(instruction), _regs);
	}
	void dispatch(const add_sp_imm& instruction) override {
		exec(add_sp_imm(instruction), _regs);
	}
	void dispatch(const add_sp_imm_t2& instruction) override {
		exec(add_sp_imm_t2(instruction), _regs);
	}
	void dispatch(const sub_sp_imm& instruction) override {
		exec(sub_sp_imm(instruction), _regs);
	}
	void dispatch(const sxth& instruction) override {
		exec(sxth(instruction), _regs);
	}
	void dispatch(const sxtb& instruction) override {
		exec(sxtb(instruction), _regs);
	}
	void dispatch(const uxth& instruction) override {
		exec(uxth(instruction), _regs);
	}
	void dispatch(const uxtb& instruction) override {
		exec(uxtb(instruction), _regs);
	}
	void dispatch(const push& instruction) override {
		exec(push(instruction), _regs, _mem);
	}
	void dispatch(const cps& instruction) override {
		exec(cps(instruction), _regs);
	}
	void dispatch(const pop& instruction) override {
		exec(pop(instruction), _regs, _mem);
	}
	void dispatch(const rev_word& instruction) override {
		exec(rev_word(instruction), _regs);
	}
	void dispatch(const rev16& instruction) override {
		exec(rev16(instruction), _regs);
	}
	void dispatch(const revsh& instruction) override {
		exec(revsh(instruction), _regs);
	}
	void dispatch(const branch& instruction) override {
		exec(branch(instruction), _regs, _regs.app_status_register());
	}
	void dispatch(const unconditional_branch& instruction) override {
		exec(unconditional_branch(instruction), _regs,
			 _regs.app_status_register());
	}
	void dispatch(const stm& instruction) override {
		exec(stm(instruction), _regs, _mem);
	}
	void dispatch(const ldm& instruction) override {
		exec(ldm(instruction), _regs, _mem);
	}
	void dispatch(const mrs& instruction) override {
		exec(mrs(instruction), _regs, _regs.app_status_register());
	}
	void dispatch(const msr& instruction) override {
		exec(msr(instruction), _regs, _regs.app_status_register());
	}
	void dispatch(const bl_imm& instruction) override {
		exec(bl_imm(instruction), _regs);
	}
	void dispatch(const svc& instruction) override {
		_exception_vector.raise(exception_type::SVCALL);
	}
	void dispatch(const udf& instr) override {
		(void)instr;
		// undefined instruction
		_exception_vector.raise(exception_type::HARDFAULT_PRECISE);
	}
	void dispatch(const udfw& instr) override {
		// undefined instruction
		_exception_vector.raise(exception_type::HARDFAULT_PRECISE);
	}
};

#endif //MICROMACHINE_INTRUCTION_DISPATCHER_HPP
