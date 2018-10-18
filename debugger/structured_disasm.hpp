#ifndef MICROMACHINE_STRUCTURED_DISASM_HPP
#define MICROMACHINE_STRUCTURED_DISASM_HPP

#include <numeric>
#include "dispatcher.hpp"
#include "string_format.hpp"

#define DISASM_FMT_HEX 				(1u << 0)
#define DISASM_FMT_HEX_NO_PREFIX 	(1u << 1)
#define DISASM_FMT_STD_REG_NAMES	(1u << 2)
#define DISASM_FMT_BRANCH_IS_NARROW	(1u << 3)
#define DISASM_FMT_USE_ALT_ADR		(1u << 4)

#include "InstructionDetails.hpp"

class structured_disasm : public dispatcher {

public:
	structured_disasm()
		: _addr(0) {
	}

	void setAddress(uint32_t address) {
		_addr = address;
	}

protected:
	uint32_t _addr;

private:
	virtual void emit_name(const std::string& name) = 0;
	virtual void emit_reg(uint8_t reg_id) = 0;
	virtual void emit_special_reg(const std::string& reg_name) = 0;
	virtual void emit_immediate(uint32_t immediate) = 0;
	virtual void emit_regs(const std::vector<uint8_t>& regs) = 0;
	virtual void emit_condition_flag(const std::string& name) = 0;
	virtual void emit_label_address(uint32_t label_address) = 0;
	virtual void emit_begin_deref() = 0;
	virtual void emit_end_deref() = 0;

	std::vector<uint8_t> reg_list_str(const register_list_t& reg_list) {
		std::vector<uint8_t> list;
		list.reserve(16);
		const reg_idx end = registers::NUM_REGS;
		for(reg_idx rid = 0; rid < end; rid++) {
			if(binops::get_bit(reg_list, rid)) {
				list.emplace_back(rid);
			}
		}
		return list;
	}

	const std::string condition_string(uint8_t condition) {
		static const char* cond_names = "eqnecsccmiplvsvchilsgeltgtle";
		return std::string(cond_names + (condition * 2U), 2);
	}

	std::string special_register(special_reg_instr::SpecialRegister sr) {
		switch(sr) {
			case msr::SpecialRegister::APSR: 	return "apsr";
			case msr::SpecialRegister::IAPSR: 	return "iapsr";
			case msr::SpecialRegister::EAPSR: 	return "eapsr";
			case msr::SpecialRegister::XPSR: 	return "xpsr";
			case msr::SpecialRegister::IPSR: 	return "ipsr";
			case msr::SpecialRegister::EPSR: 	return "epsr";
			case msr::SpecialRegister::IEPSR: 	return "iepsr";
			case msr::SpecialRegister::MSP: 	return "msp";
			case msr::SpecialRegister::PSP: 	return "psp";
			case msr::SpecialRegister::PRIMASK:	return "primask";
			case msr::SpecialRegister::CONTROL:	return "control";
		}
	}

	std::string special_register(uint8_t val) {
		return special_register(static_cast<special_reg_instr::SpecialRegister>(val));
	}

	void invalid_instruction(const uint16_t instr) override {
		//format("<UNDEFINED> instruction: 0x%04x", (uint16_t)instr);
		emit_name("UNDEFINED");
		emit_immediate(instr);
	}

	void invalid_instruction(const instruction_pair instr) override {
		//format("<UNDEFINED> instruction: 0x%08x", instr.first << 16 | instr.second);
		emit_name("UNDEFINED");
		emit_immediate(instr.first << 16 | instr.second);
	}

	//TODO: refactor and avoid passing _regs.app_status_register() explicitely
	void dispatch(const nop instruction) override {
		emit_name("nop");
	}
	void dispatch(const yield instruction) override {
		emit_name("yield");
	}
	void dispatch(const wfe instruction) override {
		emit_name("wfe");
	}
	void dispatch(const wfi instruction) override {
		emit_name("wfi");
	}
	void dispatch(const sev instruction) override {
		emit_name("sev");
	}
	void dispatch(const lsl_imm instruction) override {
		//format("lsls %s, %s, " + IMM(), R(instruction.rd), R(instruction.rm), instruction.imm5);
		emit_name("lsls");
		emit_reg(instruction.rd);
		emit_reg(instruction.rm);
		emit_immediate(instruction.imm5);
	}
	void dispatch(const lsr_imm instruction) override {
		//format("lsrs %s, %s, " + IMM(), R(instruction.rd), R(instruction.rm), instruction.imm5);
		emit_name("lsrs");
		emit_reg(instruction.rd);
		emit_reg(instruction.rm);
		emit_immediate(instruction.imm5);
	}
	void dispatch(const asr_imm instruction) override {
		//format("asrs %s, %s, " + IMM(), R(instruction.rd), R(instruction.rm), instruction.imm5);
		emit_name("asrs");
		emit_reg(instruction.rd);
		emit_reg(instruction.rm);
		emit_immediate(instruction.imm5);
	}
	void dispatch(const add_reg instruction) override {
		//format("adds %s, %s, %s", R(instruction.rd), R(instruction.rn), R(instruction.rm));
		emit_name("adds");
		emit_reg(instruction.rd);
		emit_reg(instruction.rn);
		emit_reg(instruction.rm);
	}
	void dispatch(const subs_reg instruction) override {
		//format("subs %s, %s, %s", R(instruction.rd), R(instruction.rn), R(instruction.rm));
		emit_name("subs");
		emit_reg(instruction.rd);
		emit_reg(instruction.rn);
		emit_reg(instruction.rm);
	}
	void dispatch(const add_imm instruction) override {
		//format("adds %s, %s, " + IMM(), R(instruction.rd), R(instruction.rn), instruction.imm3);
		emit_name("adds");
		emit_reg(instruction.rd);
		emit_reg(instruction.rn);
		emit_immediate(instruction.imm3);
	}
	void dispatch(const subs_imm instruction) override {
		//format("subs %s, %s, " + IMM(), R(instruction.rd), R(instruction.rn), instruction.imm3);
		emit_name("subs");
		emit_reg(instruction.rd);
		emit_reg(instruction.rn);
		emit_immediate(instruction.imm3);
	}
	void dispatch(const mov_imm instruction) override {
		//format("movs %s, " + IMM(), R(instruction.rd), instruction.imm8);
		emit_name("movs");
		emit_reg(instruction.rd);
		emit_immediate(instruction.imm8);
	}
	void dispatch(const movs instruction) override {
		//format("movs %s, %s", R(instruction.rd), R(instruction.rm));
		emit_name("subs");
		emit_reg(instruction.rd);
		emit_reg(instruction.rm);;
	}
	void dispatch(const cmp_imm instruction) override {
		//format("cmp %s, " + IMM(), R(instruction.rn), instruction.imm8);
		emit_name("cmp");
		emit_reg(instruction.rn);
		emit_immediate(instruction.imm8);
	}
	void dispatch(const add_imm_t2 instruction) override {
		//format("adds %s, " + IMM(), R(instruction.rdn), instruction.imm8);
		emit_name("adds");
		emit_reg(instruction.rdn);
		emit_immediate(instruction.imm8);
	}
	void dispatch(const subs_imm8 instruction) override {
		//format("subs %s, " + IMM(), R(instruction.rdn), instruction.imm8);
		emit_name("subs");
		emit_reg(instruction.rdn);
		emit_immediate(instruction.imm8);
	}
	void dispatch(const and_reg instruction) override {
		//format("ands %s, %s", R(instruction.rdn), R(instruction.rm));
		emit_name("ands");
		emit_reg(instruction.rdn);
		emit_reg(instruction.rm);
	}
	void dispatch(const eor_reg instruction) override {
		//format("eors %s, %s", R(instruction.rdn), R(instruction.rm));
		emit_name("eors");
		emit_reg(instruction.rdn);
		emit_reg(instruction.rm);
	}
	void dispatch(const lsl_reg instruction) override {
		//format("lsls %s, %s", R(instruction.rdn), R(instruction.rm));
		emit_name("lsls");
		emit_reg(instruction.rdn);
		emit_reg(instruction.rm);
	}
	void dispatch(const lsr_reg instruction) override {
		//format("lsrs %s, %s", R(instruction.rdn), R(instruction.rm));
		emit_name("lsrs");
		emit_reg(instruction.rdn);
		emit_reg(instruction.rm);
	}
	void dispatch(const asr_reg instruction) override {
		//format("asrs %s, %s", R(instruction.rdn), R(instruction.rm));
		emit_name("asrs");
		emit_reg(instruction.rdn);
		emit_reg(instruction.rm);
	}
	void dispatch(const adc instruction) override {
		//format("adcs %s, %s", R(instruction.rdn), R(instruction.rm));
		emit_name("adcs");
		emit_reg(instruction.rdn);
		emit_reg(instruction.rm);
	}
	void dispatch(const sbc instruction) override {
		//format("sbcs %s, %s", R(instruction.rdn), R(instruction.rm));
		emit_name("sbcs");
		emit_reg(instruction.rdn);
		emit_reg(instruction.rm);
	}
	void dispatch(const ror_reg instruction) override {
		//format("rors %s, %s", R(instruction.rdn), R(instruction.rm));
		emit_name("rors");
		emit_reg(instruction.rdn);
		emit_reg(instruction.rm);
	}
	void dispatch(const tst_reg instruction) override {
		//format("tst %s, %s", R(instruction.rn), R(instruction.rm));
		emit_name("tst");
		emit_reg(instruction.rn);
		emit_reg(instruction.rm);
	}
	void dispatch(const rsb_imm instruction) override {
		//format("rsbs %s, %s, #0", R(instruction.rd), R(instruction.rn));
		emit_name("rsbs");
		emit_reg(instruction.rd);
		emit_reg(instruction.rn);
	}
	void dispatch(const cmp_reg instruction) override {
		//format("cmp %s, %s", R(instruction.rn), R(instruction.rm));
		emit_name("cmp");
		emit_reg(instruction.rn);
		emit_reg(instruction.rm);
	}
	void dispatch(const cmn_reg instruction) override {
		//format("cmn %s, %s", R(instruction.rn), R(instruction.rm));
		emit_name("cmn");
		emit_reg(instruction.rn);
		emit_reg(instruction.rm);
	}
	void dispatch(const orr_reg instruction) override {
		//format("orrs %s, %s", R(instruction.rdn), R(instruction.rm));
		emit_name("orrs");
		emit_reg(instruction.rdn);
		emit_reg(instruction.rm);
	}
	void dispatch(const mul_reg instruction) override {
		//format("muls %s, %s", R(instruction.rdm), R(instruction.rn));
		emit_name("muls");
		emit_reg(instruction.rdm);
		emit_reg(instruction.rn);
	}
	void dispatch(const bic_reg instruction) override {
		//format("bics %s, %s", R(instruction.rdn), R(instruction.rm));
		emit_name("bics");
		emit_reg(instruction.rdn);
		emit_reg(instruction.rm);
	}
	void dispatch(const not_reg instruction) override {
		//format("mvns %s, %s", R(instruction.rd), R(instruction.rm));
		emit_name("mvns");
		emit_reg(instruction.rd);
		emit_reg(instruction.rm);
	}
	void dispatch(const add_highreg instruction) override {
		// variants of add SP plus register
		if(instruction.dm && (instruction.high_rm() == 13)) {
			//format("add %s, sp, %s", R(instruction.rdn));
			emit_name("add");
			emit_reg(instruction.rdn);
			emit_reg(13);
			emit_reg(instruction.rdn);
		} else if(instruction.high_rd() == 13) {
			//format("add sp, %s", R(instruction.high_rm()));
			emit_name("add");
			emit_reg(13);
			emit_reg(instruction.high_rm());
		} else {
			//format("add %s, %s", R(instruction.high_rd()), R(instruction.high_rm()));
			emit_name("add");
			emit_reg(instruction.high_rd());
			emit_reg(instruction.high_rm());
		}
	}
	void dispatch(const cmp_highreg instruction) override {
		//format("cmp %s, %s", R(instruction.high_rn()), R(instruction.high_rm()));
		emit_name("cmp");
		emit_reg(instruction.high_rn());
		emit_reg(instruction.high_rm());
	}
	void dispatch(const mov_highreg instruction) override {
		//format("mov %s, %s", R(instruction.high_rd()), R(instruction.high_rm()));
		emit_name("mov");
		emit_reg(instruction.high_rd());
		emit_reg(instruction.high_rm());
	}
	void dispatch(const bx instruction) override {
		//format("bx %s", R(instruction.rm));
		emit_name("bx");
		emit_reg(instruction.rm);
	}
	void dispatch(const blx instruction) override {
		//format("blx %s", R(instruction.rm));
		emit_name("blx");
		emit_reg(instruction.rm);
	}
	void dispatch(const ldr_literal instruction) override {
		// normal syntax
		//format("ldr %s, %x", R(instruction.rt), instruction.imm32()+_addr);
		// alternative syntax
		//format("ldr %s, [pc, " + IMM() + "]", R(instruction.rt), instruction.imm32());
		emit_name("ldr");
		emit_reg(instruction.rt);
		emit_begin_deref();
		emit_reg(15);
		emit_immediate(instruction.imm32());
		emit_end_deref();;
	}
	void dispatch(const str_reg instruction) override {
		//format("str %s, [%s, %s]", R(instruction.rt), R(instruction.rn), R(instruction.rm));
		emit_name("str");
		emit_reg(instruction.rt);
		emit_begin_deref();
		emit_reg(instruction.rn);
		emit_reg(instruction.rm);
		emit_end_deref();;
	}
	void dispatch(const strh_reg instruction) override {
		//format("strh %s, [%s, %s]", R(instruction.rt), R(instruction.rn), R(instruction.rm));
		emit_name("strh");
		emit_reg(instruction.rt);
		emit_begin_deref();
		emit_reg(instruction.rn);
		emit_reg(instruction.rm);
		emit_end_deref();;
	}
	void dispatch(const strb_reg instruction) override {
		//format("strb %s, [%s, %s]", R(instruction.rt), R(instruction.rn), R(instruction.rm));
		emit_name("strb");
		emit_reg(instruction.rt);
		emit_begin_deref();
		emit_reg(instruction.rn);
		emit_reg(instruction.rm);
		emit_end_deref();;
	}
	void dispatch(const ldrsb_reg instruction) override {
		//format("ldrsb %s, [%s, %s]", R(instruction.rt), R(instruction.rn), R(instruction.rm));
		emit_name("ldrsb");
		emit_reg(instruction.rt);
		emit_begin_deref();
		emit_reg(instruction.rn);
		emit_reg(instruction.rm);
		emit_end_deref();;
	}
	void dispatch(const ldr_reg instruction) override {
		//format("ldr %s, [%s, %s]", R(instruction.rt), R(instruction.rn), R(instruction.rm));
		emit_name("ldrsb");
		emit_reg(instruction.rt);
		emit_begin_deref();
		emit_reg(instruction.rn);
		emit_reg(instruction.rm);
		emit_end_deref();;
	}
	void dispatch(const ldrh_reg instruction) override {
		//format("ldrh %s, [%s, %s]", R(instruction.rt), R(instruction.rn), R(instruction.rm));
		emit_name("ldrsb");
		emit_reg(instruction.rt);
		emit_begin_deref();
		emit_reg(instruction.rn);
		emit_reg(instruction.rm);
		emit_end_deref();;
	}
	void dispatch(const ldrb_reg instruction) override {
		//format("ldrb %s, [%s, %s]", R(instruction.rt), R(instruction.rn), R(instruction.rm));
		emit_name("ldrb");
		emit_reg(instruction.rt);
		emit_begin_deref();
		emit_reg(instruction.rn);
		emit_reg(instruction.rm);
		emit_end_deref();;
	}
	void dispatch(const ldrsh_reg instruction) override {
		//format("ldrsh %s, [%s, %s]", R(instruction.rt), R(instruction.rn), R(instruction.rm));
		emit_name("ldrsh");
		emit_reg(instruction.rt);
		emit_begin_deref();
		emit_reg(instruction.rn);
		emit_reg(instruction.rm);
		emit_end_deref();;
	}
	void dispatch(const str_imm instruction) override {
		//format("str %s, [%s, " + IMM() + "]", R(instruction.rt), R(instruction.rn), instruction.imm32());
		emit_name("str");
		emit_reg(instruction.rt);
		emit_begin_deref();
		emit_reg(instruction.rn);
		emit_immediate(instruction.imm32());
		emit_end_deref();;
	}
	void dispatch(const ldr_imm instruction) override {
		//format("ldr %s, [%s, " + IMM() + "]", R(instruction.rt), R(instruction.rn), instruction.imm32());
		emit_name("ldr");
		emit_reg(instruction.rt);
		emit_begin_deref();
		emit_reg(instruction.rn);
		emit_immediate(instruction.imm32());
		emit_end_deref();;
	}
	void dispatch(const strb_imm instruction) override {
		//format("strb %s, [%s, " + IMM() + "]", R(instruction.rt), R(instruction.rn), instruction.imm5);
		emit_name("strb");
		emit_reg(instruction.rt);
		emit_begin_deref();
		emit_reg(instruction.rn);
		emit_immediate(instruction.imm5);
		emit_end_deref();;
	}
	void dispatch(const ldrb_imm instruction) override {
		//format("ldrb %s, [%s, " + IMM() + "]", R(instruction.rt), R(instruction.rn), instruction.imm5);
		emit_name("ldrb");
		emit_reg(instruction.rt);
		emit_begin_deref();
		emit_reg(instruction.rn);
		emit_immediate(instruction.imm5);
		emit_end_deref();;
	}
	void dispatch(const strh_imm instruction) override {
		//format("strh %s, [%s, " + IMM() + "]", R(instruction.rt), R(instruction.rn), instruction.imm32());
		emit_name("strh");
		emit_reg(instruction.rt);
		emit_begin_deref();
		emit_reg(instruction.rn);
		emit_immediate(instruction.imm32());
		emit_end_deref();;
	}
	void dispatch(const ldrh_imm instruction) override {
		//format("ldrh %s, [%s, " + IMM() + "]", R(instruction.rt), R(instruction.rn), instruction.imm32());
		emit_name("ldrh");
		emit_reg(instruction.rt);
		emit_begin_deref();
		emit_reg(instruction.rn);
		emit_immediate(instruction.imm32());
		emit_end_deref();;
	}
	void dispatch(const str_sp_imm instruction) override {
		//format("str %s, [sp, " + IMM() + "]", R(instruction.rt), instruction.imm32());
		emit_name("str");
		emit_reg(instruction.rt);
		emit_begin_deref();
		emit_reg(13);
		emit_immediate(instruction.imm32());
		emit_end_deref();;
	}
	void dispatch(const ldr_sp_imm instruction) override {
		//format("ldr %s, [sp, " + IMM() + "]", R(instruction.rt), instruction.imm32());
		emit_name("ldr");
		emit_reg(instruction.rt);
		emit_begin_deref();
		emit_reg(13);
		emit_immediate(instruction.imm32());
		emit_end_deref();
	}
	void dispatch(const adr instruction) override {
		// normal syntax
		//format("adr %s, " + IMM(), R(instruction.rd), instruction.imm32());
		emit_name("adr");
		emit_reg(instruction.rd);
		emit_immediate(instruction.imm32());
	}
	void dispatch(const add_sp_imm instruction) override {
		//format("add %s, sp, " + IMM(), R(instruction.rd), instruction.imm32());
		emit_name("add");
		emit_reg(instruction.rd);
		emit_reg(13);
		emit_immediate(instruction.imm32());
	}
	void dispatch(const add_sp_imm_t2 instruction) override {
		//format("add sp, " + IMM(), instruction.imm32());
		emit_name("add");
		emit_reg(13);
		emit_immediate(instruction.imm32());
	}
	void dispatch(const sub_sp_imm instruction) override {
		//format("sub sp, sp, " + IMM(), instruction.imm32());
		emit_name("sub");
		emit_reg(13);
		emit_reg(13);
		emit_immediate(instruction.imm32());
	}
	void dispatch(const sxth instruction) override {
		//format("sxth %s, %s", R(instruction.rd), R(instruction.rm));
		emit_name("sxth");
		emit_reg(instruction.rd);
		emit_reg(instruction.rm);
	}
	void dispatch(const sxtb instruction) override {
		//format("sxtb %s, %s", R(instruction.rd), R(instruction.rm));
		emit_name("sxtb");
		emit_reg(instruction.rd);
		emit_reg(instruction.rm);
	}
	void dispatch(const uxth instruction) override {
		//format("uxth %s, %s", R(instruction.rd), R(instruction.rm));
		emit_name("uxth");
		emit_reg(instruction.rd);
		emit_reg(instruction.rm);
	}
	void dispatch(const uxtb instruction) override {
		//format("uxtb %s, %s", R(instruction.rd), R(instruction.rm));
		emit_name("uxtb");
		emit_reg(instruction.rd);
		emit_reg(instruction.rm);
	}
	void dispatch(const push instruction) override {
		//format("push {%s}", reg_list_str(instruction.register_list).c_str());
		emit_name("push");
		emit_regs(reg_list_str(instruction.register_list));
	}
	void dispatch(const cps instruction) override {
		//format("cps%s i", instruction.im ? "id" : "ie");
		if(instruction.im) {
			emit_name("cpsid");
		} else {
			emit_name("cpsie");
		}
	}
	void dispatch(const pop instruction) override {
		//format("pop {%s}", reg_list_str(instruction.register_list).c_str());
		emit_name("pop");
		emit_regs(reg_list_str(instruction.register_list));
	}
	void dispatch(const bkpt instruction) override {
		//format("bkpt " + IMM(), instruction.imm8);
		emit_name("bkpt");
		emit_immediate(instruction.imm8);
	}
	void dispatch(const rev_word instruction) override {
		//format("rev %s, %s", R(instruction.rd), R(instruction.rm));
		emit_name("rev");
		emit_reg(instruction.rd);
		emit_reg(instruction.rm);
	}
	void dispatch(const rev16 instruction) override {
		//format("rev16 %s, %s", R(instruction.rd), R(instruction.rm));
		emit_name("rev16");
		emit_reg(instruction.rd);
		emit_reg(instruction.rm);
	}
	void dispatch(const revsh instruction) override {
		//format("revsh %s, %s", R(instruction.rd), R(instruction.rm));
		emit_name("revsh");
		emit_reg(instruction.rd);
		emit_reg(instruction.rm);
	}
	void dispatch(const branch instruction) override {
		int32_t offset = instruction.offset();
		int32_t label = ((int32_t)_addr + 4) + offset;
		//std::string narrow_suffix = format_use_narrow_branch() ? ".n" : "";
		//format("b%2.2s"+narrow_suffix+" "+IMM(), condition_string(instruction.cond), label);
		emit_name("b");
		emit_condition_flag(condition_string(instruction.cond));
		emit_label_address(label);
	}
	void dispatch(const unconditional_branch instruction) override {
		int32_t offset = instruction.offset();
		int32_t label = ((int32_t)_addr + 4) + offset;
		//format("b.n 0x%x", label);
		emit_name("b.n");
		emit_label_address(label);
	}
	void dispatch(const stm instruction) override {
		/*
		format("stmia %s!, {%s}"
			, R(instruction.rn)
			, reg_list_str(instruction.register_list).c_str()
		);*/
		emit_name("stmia");
		emit_regs(reg_list_str(instruction.register_list));
	}
	void dispatch(const ldm instruction) override {
		bool list_contains_rn = binops::get_bit(instruction.register_list, instruction.rn);
		/*
		format("ldmia %s%s, {%s}"
			, R(instruction.rn)
			, list_contains_rn ? "" : "!"
			, reg_list_str(instruction.register_list).c_str()
		);*/
		emit_name("ldmia");
		emit_reg(instruction.rn);
		emit_regs(reg_list_str(instruction.register_list));
	}
	void dispatch(const mrs instruction) override {
		//format("mrs %s, %s", R(instruction.rd), special_register(instruction.sysn).c_str());
		emit_name("mrs");
		emit_reg(instruction.rd);
		emit_special_reg(special_register(instruction.sysn));
	}
	void dispatch(const msr instruction) override {
		//format("msr %s, %s", special_register(instruction.sysn).c_str(), R(instruction.rn));
		emit_name("msr");
		emit_special_reg(special_register(instruction.sysn));
		emit_reg(instruction.rn);
	}
	void dispatch(const bl_imm instruction) override {
		int32_t offset = instruction.offset();
		uint32_t label = (_addr + 4) + offset;
		//format("bl 0x%x", label);
		emit_name("bl");
		emit_label_address(label);
	}
	void dispatch(const svc instruction) override {
		//format("svc %d", instruction.imm8);
		emit_name("svc");
		emit_immediate(instruction.imm8);
	}
	void dispatch(const udf instr) override {
		//format("udf " + IMM(), instr.imm32);
		emit_name("udf");
		emit_immediate(instr.imm32);
	}
	void dispatch(const udfw instr) override {
		//format("udf.w " + IMM(), instr.imm32);
		emit_name("udf.w");
		emit_immediate(instr.imm32);
	}
};

#endif //MICROMACHINE_STRUCTURED_DISASM_HPP
