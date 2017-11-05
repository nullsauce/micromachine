/* Copyright (C) The Micromachine project - 2016 - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of The Micromachine project.

*/

#ifndef MICROMACHINE_INTRUCTION_DISPATCHER_HPP
#define MICROMACHINE_INTRUCTION_DISPATCHER_HPP

#include "dispatcher.hpp"
#include "exec.hpp"
#include "exception_vector.hpp"
#include "memory.hpp"
#include "registers.hpp"
#include "apsr_register.hpp"

class exec_dispatcher : public dispatcher {
public:
	exec_dispatcher(
		registers& regs,
		memory& mem,
		exception_vector& exceptions)
	: dispatcher(exceptions)
	, _regs(regs)
	, _mem(mem)
	{}

private:

	registers& _regs;
	memory& _mem;
	exception_vector _exceptions;

	//TODO: refactor and avoid passing _regs.status_register() explicitely
	void dispatch(const nop& instruction) override {
	
	}
	void dispatch(const lsl_imm& instruction) override {
		exec(lsl_imm(instruction), _regs, _regs.status_register());
	}
	void dispatch(const lsr_imm& instruction) override {
		exec(lsr_imm(instruction), _regs, _regs.status_register());
	}
	void dispatch(const asr_imm& instruction) override {
		exec(asr_imm(instruction), _regs, _regs.status_register());
	}
	void dispatch(const add_reg& instruction) override {
		exec(add_reg(instruction), _regs, _regs.status_register());
	}
	void dispatch(const subs_reg& instruction) override {
		exec(subs_reg(instruction), _regs, _regs.status_register());
	}
	void dispatch(const add_imm& instruction) override {
		exec(add_imm(instruction), _regs, _regs.status_register());
	}
	void dispatch(const subs_imm& instruction) override {
		exec(subs_imm(instruction), _regs, _regs.status_register());
	}
	void dispatch(const mov_imm& instruction) override {
		exec(mov_imm(instruction), _regs, _regs.status_register());
	}
	void dispatch(const movs& instruction) override {
		exec(movs(instruction), _regs, _regs.status_register());
	}
	void dispatch(const cmp_imm& instruction) override {
		exec(cmp_imm(instruction), _regs, _regs.status_register());
	}
	void dispatch(const add_imm_t2& instruction) override {
		exec(add_imm_t2(instruction), _regs, _regs.status_register());
	}
	void dispatch(const subs_imm8& instruction) override {
		exec(subs_imm8(instruction), _regs, _regs.status_register());
	}
	void dispatch(const and_reg& instruction) override {
		exec(and_reg(instruction), _regs, _regs.status_register());
	}
	void dispatch(const eor_reg& instruction) override {
		exec(eor_reg(instruction), _regs, _regs.status_register());
	}
	void dispatch(const lsl_reg& instruction) override {
		exec(lsl_reg(instruction), _regs, _regs.status_register());
	}
	void dispatch(const lsr_reg& instruction) override {
		exec(lsr_reg(instruction), _regs, _regs.status_register());
	}
	void dispatch(const asr_reg& instruction) override {
		exec(asr_reg(instruction), _regs, _regs.status_register());
	}
	void dispatch(const adc& instruction) override {
		exec(adc(instruction), _regs, _regs.status_register());
	}
	void dispatch(const sub_c_reg& instruction) override {
		exec(sub_c_reg(instruction), _regs, _regs.status_register());
	}
	void dispatch(const ror_reg& instruction) override {
		exec(ror_reg(instruction), _regs, _regs.status_register());
	}
	void dispatch(const tst_reg& instruction) override {
		exec(tst_reg(instruction), _regs, _regs.status_register());
	}
	void dispatch(const rsb_imm& instruction) override {
		exec(rsb_imm(instruction), _regs, _regs.status_register());
	}
	void dispatch(const cmp_reg& instruction) override {
		exec(cmp_reg(instruction), _regs, _regs.status_register());
	}
	void dispatch(const cmn_reg& instruction) override {
		exec(cmn_reg(instruction), _regs, _regs.status_register());
	}
	void dispatch(const lor_reg& instruction) override {
		exec(lor_reg(instruction), _regs, _regs.status_register());
	}
	void dispatch(const mul_reg& instruction) override {
		exec(mul_reg(instruction), _regs, _regs.status_register());
	}
	void dispatch(const bic_reg& instruction) override {
		exec(bic_reg(instruction), _regs, _regs.status_register());
	}
	void dispatch(const not_reg& instruction) override {
		exec(not_reg(instruction), _regs, _regs.status_register());
	}
	void dispatch(const add_highreg& instruction) override {
		exec(add_highreg(instruction), _regs, _regs.status_register());
	}
	void dispatch(const cmp_highreg& instruction) override {
		exec(cmp_highreg(instruction), _regs, _regs.status_register());
	}
	void dispatch(const mov_highreg& instruction) override {
		exec(mov_highreg(instruction), _regs, _regs.status_register());
	}
	void dispatch(const bx& instruction) override {
		exec(bx(instruction), _regs, _regs.status_register());
	}
	void dispatch(const blx& instruction) override {
		exec(blx(instruction), _regs, _regs.status_register());
	}
	void dispatch(const ldr_literal& instruction) override {
		exec(ldr_literal(instruction), _regs, _regs.status_register(), _mem);
	}
	void dispatch(const store_reg_word_reg& instruction) override {
		exec(store_reg_word_reg(instruction), _regs, _regs.status_register(), _mem);
	}
	void dispatch(const store_reg_halfword_reg& instruction) override {
		exec(store_reg_halfword_reg(instruction), _regs, _regs.status_register(), _mem);
	}
	void dispatch(const store_reg_byte_reg& instruction) override {
		exec(store_reg_byte_reg(instruction), _regs, _regs.status_register(), _mem);
	}
	void dispatch(const load_reg_sbyte_reg& instruction) override {
		exec(load_reg_sbyte_reg(instruction), _regs, _regs.status_register(), _mem);
	}
	void dispatch(const ldr_reg& instruction) override {
		exec(ldr_reg(instruction), _regs, _regs.status_register(), _mem);
	}
	void dispatch(const load_reg_halfword_reg& instruction) override {
		exec(load_reg_halfword_reg(instruction), _regs, _regs.status_register(), _mem);
	}
	void dispatch(const load_reg_byte_reg& instruction) override {
		exec(load_reg_byte_reg(instruction), _regs, _regs.status_register(), _mem);
	}
	void dispatch(const load_reg_shalfword_reg& instruction) override {
		exec(load_reg_shalfword_reg(instruction), _regs, _regs.status_register(), _mem);
	}
	void dispatch(const store_word_imm& instruction) override {
		exec(store_word_imm(instruction), _regs, _regs.status_register(), _mem);
	}
	void dispatch(const ldr_imm& instruction) override {
		exec(ldr_imm(instruction), _regs, _regs.status_register(), _mem);
	}
	void dispatch(const store_byte_imm& instruction) override {
		exec(store_byte_imm(instruction), _regs, _regs.status_register(), _mem);
	}
	void dispatch(const load_byte_imm& instruction) override {
		exec(load_byte_imm(instruction), _regs, _regs.status_register(), _mem);
	}
	void dispatch(const store_halfword_imm& instruction) override {
		exec(store_halfword_imm(instruction), _regs, _regs.status_register(), _mem);
	}
	void dispatch(const load_halfword_imm& instruction) override {
		exec(load_halfword_imm(instruction), _regs, _regs.status_register(), _mem);
	}
	void dispatch(const store_word_sp_imm& instruction) override {
		exec(store_word_sp_imm(instruction), _regs, _regs.status_register(), _mem);
	}
	void dispatch(const ldr_imm_sp& instruction) override {
		exec(ldr_imm_sp(instruction), _regs, _regs.status_register(), _mem);
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
	void dispatch(const pop& instruction) override {
		exec(pop(instruction), _regs, _mem);
	}
	void dispatch(const rev_word& instruction) override {
		exec(rev_word(instruction), _regs);
	}
	void dispatch(const rev_packed_halfword& instruction) override {
		exec(rev_packed_halfword(instruction), _regs);
	}
	void dispatch(const rev_packed_signed_halfword& instruction) override {
		exec(rev_packed_signed_halfword(instruction), _regs);
	}
	void dispatch(const branch& instruction) override {
		exec(branch(instruction), _regs, _regs.status_register());
	}
	void dispatch(const unconditional_branch& instruction) override {
		exec(unconditional_branch(instruction), _regs, _regs.status_register());
	}
	void dispatch(const stm& instruction) override {
		exec(stm(instruction), _regs, _mem);
	}
	void dispatch(const ldm& instruction) override {
		exec(ldm(instruction), _regs, _mem);
	}
	void dispatch(const mrs& instruction) override {
		exec(mrs(instruction), _regs, _regs.status_register());
	}
	void dispatch(const msr& instruction) override {
		exec(msr(instruction), _regs, _regs.status_register());
	}
	void dispatch(const bl_imm& instruction) override {
		exec(bl_imm(instruction), _regs);
	}
	void dispatch(const svc& instruction) override {
		exec(svc(instruction));
	}

};

#endif //MICROMACHINE_INTRUCTION_DISPATCHER_HPP
