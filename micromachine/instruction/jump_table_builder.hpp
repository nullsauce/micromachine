
//
// Created by fla on 03.05.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#pragma once

#include "instruction_decoder.hpp"

namespace micromachine::system {

template <typename T>
class jumptable16_builder : public instruction_decoder {
public:
	jumptable16_builder(T* target)
		: _target(target) {}

	void reset() {
		_function = nullptr;
	}

	std::function<void()> function() {
		return _function;
	}

public:
	T* const _target;
	std::function<void()> _function;

	void invalid_instruction(const uint16_t instr) override {
		(void)instr;
		//_exception_controller.raise_hardfault();
	}

	void invalid_instruction(const instruction_pair instr) override {
		(void)instr;
		//_exception_controller.raise_hardfault();
	}

	void dispatch_nop(const nop) override {
		// do literally nothing
	}
	void dispatch_yield(const yield instruction) override {
		// for os and threading
	}
	void dispatch_wfe(const wfe instruction) override {
		_function = std::bind(&T::dispatch_wfe, _target, instruction);
	}
	void dispatch_wfi(const wfi instruction) override {
		/* If PRIMASK.PM is set to 1, an asynchronous exception that has a higher group priority
		 * than any active exception results in a WFI instruction exit. If the group priority of the
		 * exception is less than or equal to the execution group priority, the exception is
		 * ignored.
		 */
	}
	void dispatch_sev(const sev instruction) override {
		_function = std::bind(&T::dispatch_sev, _target, instruction);
	}
	void dispatch_lsl_imm(const lsl_imm instruction) override {
		_function = std::bind(&T::dispatch_lsl_imm, _target, instruction);
	}
	void dispatch_lsr_imm(const lsr_imm instruction) override {
		_function = std::bind(&T::dispatch_lsr_imm, _target, instruction);
	}
	void dispatch_asr_imm(const asr_imm instruction) override {
		_function = std::bind(&T::dispatch_asr_imm, _target, instruction);
	}
	void dispatch_add_reg(const add_reg instruction) override {
		_function = std::bind(&T::dispatch_add_reg, _target, instruction);
	}
	void dispatch_subs_reg(const subs_reg instruction) override {
		_function = std::bind(&T::dispatch_subs_reg, _target, instruction);
	}
	void dispatch_add_imm(const add_imm instruction) override {
		_function = std::bind(&T::dispatch_add_imm, _target, instruction);
	}
	void dispatch_subs_imm(const subs_imm instruction) override {
		_function = std::bind(&T::dispatch_subs_imm, _target, instruction);
	}
	void dispatch_mov_imm(const mov_imm instruction) override {
		_function = std::bind(&T::dispatch_mov_imm, _target, instruction);
	}
	void dispatch_movs(const movs instruction) override {
		_function = std::bind(&T::dispatch_movs, _target, instruction);
	}
	void dispatch_cmp_imm(const cmp_imm instruction) override {
		_function = std::bind(&T::dispatch_cmp_imm, _target, instruction);
	}
	void dispatch_add_imm_t2(const add_imm_t2 instruction) override {
		_function = std::bind(&T::dispatch_add_imm_t2, _target, instruction);
	}
	void dispatch_subs_imm8(const subs_imm8 instruction) override {
		_function = std::bind(&T::dispatch_subs_imm8, _target, instruction);
	}
	void dispatch_and_reg(const and_reg instruction) override {
		_function = std::bind(&T::dispatch_and_reg, _target, instruction);
	}
	void dispatch_eor_reg(const eor_reg instruction) override {
		_function = std::bind(&T::dispatch_eor_reg, _target, instruction);
	}
	void dispatch_lsl_reg(const lsl_reg instruction) override {
		_function = std::bind(&T::dispatch_lsl_reg, _target, instruction);
	}
	void dispatch_lsr_reg(const lsr_reg instruction) override {
		_function = std::bind(&T::dispatch_lsr_reg, _target, instruction);
	}
	void dispatch_asr_reg(const asr_reg instruction) override {
		_function = std::bind(&T::dispatch_asr_reg, _target, instruction);
	}
	void dispatch_adc(const adc instruction) override {
		_function = std::bind(&T::dispatch_adc, _target, instruction);
	}
	void dispatch_sbc(const sbc instruction) override {
		_function = std::bind(&T::dispatch_sbc, _target, instruction);
	}
	void dispatch_ror_reg(const ror_reg instruction) override {
		_function = std::bind(&T::dispatch_ror_reg, _target, instruction);
	}
	void dispatch_tst_reg(const tst_reg instruction) override {
		_function = std::bind(&T::dispatch_tst_reg, _target, instruction);
	}
	void dispatch_rsb_imm(const rsb_imm instruction) override {
		_function = std::bind(&T::dispatch_rsb_imm, _target, instruction);
	}
	void dispatch_cmp_reg(const cmp_reg instruction) override {
		_function = std::bind(&T::dispatch_cmp_reg, _target, instruction);
	}
	void dispatch_cmn_reg(const cmn_reg instruction) override {
		_function = std::bind(&T::dispatch_cmn_reg, _target, instruction);
	}
	void dispatch_orr_reg(const orr_reg instruction) override {
		_function = std::bind(&T::dispatch_orr_reg, _target, instruction);
	}
	void dispatch_mul_reg(const mul_reg instruction) override {
		_function = std::bind(&T::dispatch_mul_reg, _target, instruction);
	}
	void dispatch_bic_reg(const bic_reg instruction) override {
		_function = std::bind(&T::dispatch_bic_reg, _target, instruction);
	}
	void dispatch_mvn(const mvn instruction) override {
		_function = std::bind(&T::dispatch_mvn, _target, instruction);
	}
	void dispatch_add_highreg(const add_highreg instruction) override {
		_function = std::bind(&T::dispatch_add_highreg, _target, instruction);
	}
	void dispatch_cmp_highreg(const cmp_highreg instruction) override {
		_function = std::bind(&T::dispatch_cmp_highreg, _target, instruction);
	}
	void dispatch_mov_highreg(const mov_highreg instruction) override {
		_function = std::bind(&T::dispatch_mov_highreg, _target, instruction);
	}
	void dispatch_bx(const bx instruction) override {
		_function = std::bind(&T::dispatch_bx, _target, instruction);
	}
	void dispatch_blx(const blx instruction) override {
		_function = std::bind(&T::dispatch_blx, _target, instruction);
	}
	void dispatch_ldr_literal(const ldr_literal instruction) override {
		_function = std::bind(&T::dispatch_ldr_literal, _target, instruction);
	}
	void dispatch_str_reg(const str_reg instruction) override {
		_function = std::bind(&T::dispatch_str_reg, _target, instruction);
	}
	void dispatch_strh_reg(const strh_reg instruction) override {
		_function = std::bind(&T::dispatch_strh_reg, _target, instruction);
	}
	void dispatch_strb_reg(const strb_reg instruction) override {
		_function = std::bind(&T::dispatch_strb_reg, _target, instruction);
	}
	void dispatch_ldrsb_reg(const ldrsb_reg instruction) override {
		_function = std::bind(&T::dispatch_ldrsb_reg, _target, instruction);
	}
	void dispatch_ldr_reg(const ldr_reg instruction) override {
		_function = std::bind(&T::dispatch_ldr_reg, _target, instruction);
	}
	void dispatch_ldrh_reg(const ldrh_reg instruction) override {
		_function = std::bind(&T::dispatch_ldrh_reg, _target, instruction);
	}
	void dispatch_ldrb_reg(const ldrb_reg instruction) override {
		_function = std::bind(&T::dispatch_ldrb_reg, _target, instruction);
	}
	void dispatch_ldrsh_reg(const ldrsh_reg instruction) override {
		_function = std::bind(&T::dispatch_ldrsh_reg, _target, instruction);
	}
	void dispatch_str_imm(const str_imm instruction) override {
		_function = std::bind(&T::dispatch_str_imm, _target, instruction);
	}
	void dispatch_ldr_imm(const ldr_imm instruction) override {
		_function = std::bind(&T::dispatch_ldr_imm, _target, instruction);
	}
	void dispatch_strb_imm(const strb_imm instruction) override {
		_function = std::bind(&T::dispatch_strb_imm, _target, instruction);
	}
	void dispatch_ldrb_imm(const ldrb_imm instruction) override {
		_function = std::bind(&T::dispatch_ldrb_imm, _target, instruction);
	}
	void dispatch_strh_imm(const strh_imm instruction) override {
		_function = std::bind(&T::dispatch_strh_imm, _target, instruction);
	}
	void dispatch_ldrh_imm(const ldrh_imm instruction) override {
		_function = std::bind(&T::dispatch_ldrh_imm, _target, instruction);
	}
	void dispatch_str_sp_imm(const str_sp_imm instruction) override {
		_function = std::bind(&T::dispatch_str_sp_imm, _target, instruction);
	}
	void dispatch_ldr_sp_imm(const ldr_sp_imm instruction) override {
		_function = std::bind(&T::dispatch_ldr_sp_imm, _target, instruction);
	}
	void dispatch_adr(const adr instruction) override {
		_function = std::bind(&T::dispatch_adr, _target, instruction);
	}
	void dispatch_add_sp_imm(const add_sp_imm instruction) override {
		_function = std::bind(&T::dispatch_add_sp_imm, _target, instruction);
	}
	void dispatch_add_sp_imm_t2(const add_sp_imm_t2 instruction) override {
		_function = std::bind(&T::dispatch_add_sp_imm_t2, _target, instruction);
	}
	void dispatch_sub_sp_imm(const sub_sp_imm instruction) override {
		_function = std::bind(&T::dispatch_sub_sp_imm, _target, instruction);
	}
	void dispatch_sxth(const sxth instruction) override {
		_function = std::bind(&T::dispatch_sxth, _target, instruction);
	}
	void dispatch_sxtb(const sxtb instruction) override {
		_function = std::bind(&T::dispatch_sxtb, _target, instruction);
	}
	void dispatch_uxth(const uxth instruction) override {
		_function = std::bind(&T::dispatch_uxth, _target, instruction);
	}
	void dispatch_uxtb(const uxtb instruction) override {
		_function = std::bind(&T::dispatch_uxtb, _target, instruction);
	}
	void dispatch_push(const push instruction) override {
		_function = std::bind(&T::dispatch_push, _target, instruction);
	}
	void dispatch_cps(const cps instruction) override {
		_function = std::bind(&T::dispatch_cps, _target, instruction);
	}
	void dispatch_pop(const pop instruction) override {
		_function = std::bind(&T::dispatch_pop, _target, instruction);
	}
	void dispatch_bkpt(const bkpt instruction) override {
		_function = std::bind(&T::dispatch_bkpt, _target, instruction);
	}
	void dispatch_rev_word(const rev_word instruction) override {
		_function = std::bind(&T::dispatch_rev_word, _target, instruction);
	}
	void dispatch_rev16(const rev16 instruction) override {
		_function = std::bind(&T::dispatch_rev16, _target, instruction);
	}
	void dispatch_revsh(const revsh instruction) override {
		_function = std::bind(&T::dispatch_revsh, _target, instruction);
	}
	void dispatch_branch(const branch instruction) override {
		_function = std::bind(&T::dispatch_branch, _target, instruction);
	}
	void dispatch_unconditional_branch(const unconditional_branch instruction) override {
		_function = std::bind(&T::dispatch_unconditional_branch, _target, instruction);
	}
	void dispatch_stm(const stm instruction) override {
		_function = std::bind(&T::dispatch_stm, _target, instruction);
	}
	void dispatch_ldm(const ldm instruction) override {
		_function = std::bind(&T::dispatch_ldm, _target, instruction);
	}
	void dispatch_mrs(const mrs instruction) override {
		_function = std::bind(&T::dispatch_mrs, _target, instruction);
	}
	void dispatch_msr(const msr instruction) override {
		_function = std::bind(&T::dispatch_msr, _target, instruction);
	}
	void dispatch_bl_imm(const bl_imm instruction) override {
		_function = std::bind(&T::dispatch_bl_imm, _target, instruction);
	}
	void dispatch_svc(const svc) override {
		//_exception_controller.raise_svcall();
	}
	void dispatch_dmb(const dmb instruction) override {
		_function = std::bind(&T::dispatch_dmb, _target, instruction);
	}
	void dispatch_dsb(const dsb instruction) override {
		_function = std::bind(&T::dispatch_dsb, _target, instruction);
	}
	void dispatch_isb(const isb instruction) override {
		_function = std::bind(&T::dispatch_isb, _target, instruction);
	}
	void dispatch_udf(const udf instr) override {
		// undefined instruction
		//_exception_controller.raise_hardfault();
	}
	void dispatch_udfw(const udfw) override {
		// undefined instruction
		//_exception_controller.raise_hardfault();
	}
};

}; // namespace micromachine::system
