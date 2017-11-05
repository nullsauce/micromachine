/* Copyright (C) MindMaze SA - 2016 - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of MindMaze SA.

*/

#ifndef MICROMACHINE_DISASM_HPP
#define MICROMACHINE_DISASM_HPP

#include "dispatcher.hpp"
#include "string_format.hpp"

class disasm : public dispatcher {

public:

	static std::string disassemble_instruction(const instruction_pair& instr) {
		exception_vector dummy;
		disasm d(dummy);
		d.dispatch_instruction(instr);
		return d.instruction_string;
	}

private:

	disasm(exception_vector& exception_vector)
	 : dispatcher(exception_vector)
	 , instruction_string(128, '\0') {
	 }

	std::string instruction_string;

	void write(const std::string& str) {
		instruction_string = str;
	}

	template<typename ... Args>
	void format(const std::string& format, Args&&... args ) {
		write(string_format(format, std::forward<Args>(args)...));
	}

	static std::string reg_list_str(const register_list_t& reg_list) {
		std::stringstream ss;
		const reg_idx end = registers::NUM_REGS-1;
		for(reg_idx rid = 0; rid < end; rid++) {
			if(binops::get_bit(reg_list, rid)) {
				ss << "r" << rid;
				if(rid < end-1)
					ss << ", ";
			}
		}
		return ss.str();
	}

	static const char* condition_string(uint8_t condition) {
		static const char* cond_names = "eqnecsccmiplvsvchilsgeltgtle";
		return cond_names + (condition * 2U);
	}

	static std::string special_register(special_reg_instr::SpecialRegister sr) {
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
			default: return "?";
		}
	}

	static std::string special_register(uint8_t val) {
		return special_register((special_reg_instr::SpecialRegister)val);
	}

	//TODO: refactor and avoid passing _regs.app_status_register() explicitely
	void dispatch(const nop& instruction) override {
		write("nop");
	}

	void dispatch(const lsl_imm& instruction) override {
		format("lsls r%d, r%d, #%d", instruction.rd, instruction.rm, instruction.imm5);
	}
	void dispatch(const lsr_imm& instruction) override {
		format("lsrs r%d, r%d, #%d", instruction.rd, instruction.rm, instruction.imm5);
	}
	void dispatch(const asr_imm& instruction) override {
		format("asrs r%d, r%d #%d", instruction.rd, instruction.rm, instruction.imm5);
	}
	void dispatch(const add_reg& instruction) override {
		format("adds r%d, r%d, r%d", instruction.rd, instruction.rn, instruction.rm);
	}
	void dispatch(const subs_reg& instruction) override {
		format("subs r%d, r%d, r%d", instruction.rd, instruction.rn, instruction.rm);
	}
	void dispatch(const add_imm& instruction) override {
		format("adds r%d, r%d, #%d", instruction.rd, instruction.rn, instruction.imm3);
	}
	void dispatch(const subs_imm& instruction) override {
		format("subs r%d, r%d, #%d", instruction.rd, instruction.rn, instruction.imm3);
	}
	void dispatch(const mov_imm& instruction) override {
		format("movs r%d, #%x", instruction.rd, instruction.imm8);
	}
	void dispatch(const movs& instruction) override {
		format("movs r%d, r%d", instruction.rd, instruction.rm);
	}
	void dispatch(const cmp_imm& instruction) override {
		format("cmp r%d, #%x", instruction.rn, instruction.imm8);
	}
	void dispatch(const add_imm_t2& instruction) override {
		format("adds r%d, #%x", instruction.rdn, instruction.imm8);
	}
	void dispatch(const subs_imm8& instruction) override {
		format("subs r%d, #%x", instruction.rdn, instruction.imm8);
	}
	void dispatch(const and_reg& instruction) override {
		format("ands r%d, r%d", instruction.rdn, instruction.rm);
	}
	void dispatch(const eor_reg& instruction) override {
		format("eors r%d, r%d", instruction.rdn, instruction.rm);
	}
	void dispatch(const lsl_reg& instruction) override {
		format("lsls r%d, r%d", instruction.rdn, instruction.rm);
	}
	void dispatch(const lsr_reg& instruction) override {
		format("lsrs r%d, r%d", instruction.rdn, instruction.rm);
	}
	void dispatch(const asr_reg& instruction) override {
		format("asrs r%d, r%d", instruction.rdn, instruction.rm);
	}
	void dispatch(const adc& instruction) override {
		format("adcs r%d, r%d", instruction.rdn, instruction.rm);
	}
	void dispatch(const sbc& instruction) override {
		format("sbcs r%d, r%d", instruction.rdn, instruction.rm);
	}
	void dispatch(const ror_reg& instruction) override {
		format("rors r%d, r%d", instruction.rdn, instruction.rm);
	}
	void dispatch(const tst_reg& instruction) override {
		format("test r%d, r%d", instruction.rn, instruction.rm);
	}
	void dispatch(const rsb_imm& instruction) override {
		format("rsbs r%d, r%d, #0", instruction.rd, instruction.rn);
	}
	void dispatch(const cmp_reg& instruction) override {
		format("cmp r%d, r%d", instruction.rn, instruction.rm);
	}
	void dispatch(const cmn_reg& instruction) override {
		format("cmn r%d, r%d", instruction.rn, instruction.rm);
	}
	void dispatch(const orr_reg& instruction) override {
		format("orrs r%d, r%d", instruction.rdn, instruction.rm);
	}
	void dispatch(const mul_reg& instruction) override {
		format("muls r%d, r%d, r%d", instruction.rdm, instruction.rn, instruction.rdm);
	}
	void dispatch(const bic_reg& instruction) override {
		format("bics r%d, r%d", instruction.rdn, instruction.rm);
	}
	void dispatch(const not_reg& instruction) override {
		format("mvns r%d, r%d", instruction.rd, instruction.rm);
	}
	void dispatch(const add_highreg& instruction) override {
		// variants of add SP plus register
		if(instruction.dm && instruction.high_rm() == 13) {
			format("add r%d, sp, r%d", instruction.rdn);
		} else if(instruction.high_rd() == 13) {
			format("add sp, r%d", instruction.high_rm());
		} else {
			format("add r%d, r%d", instruction.high_rd(), instruction.high_rm());
		}
	}
	void dispatch(const cmp_highreg& instruction) override {
		format("cmp r%d, r%d\n", instruction.high_rn(), instruction.high_rm());
	}
	void dispatch(const mov_highreg& instruction) override {
		format("mov r%d, r%d", instruction.high_rd(), instruction.high_rm());
	}
	void dispatch(const bx& instruction) override {
		format("bx %d", instruction.rm);
	}
	void dispatch(const blx& instruction) override {
		format("blx %d", instruction.rm);
	}
	void dispatch(const ldr_literal& instruction) override {
		format("ldr r%d, [pc, #%x]", instruction.rt, instruction.imm32(), instruction.imm32());
	}
	void dispatch(const str_reg& instruction) override {
		format("str r%d, [r%d, r%d]", instruction.rt, instruction.rn, instruction.rm);
	}
	void dispatch(const strh_reg& instruction) override {
		format("strh r%d, [r%d, r%d]", instruction.rt, instruction.rn, instruction.rm);
	}
	void dispatch(const strb_reg& instruction) override {
		format("strb r%d, [r%d, r%d]", instruction.rt, instruction.rn, instruction.rm);
	}
	void dispatch(const ldrsb_reg& instruction) override {
		format("ldrsb r%d, [r%d, r%d]", instruction.rt, instruction.rn, instruction.rm);
	}
	void dispatch(const ldr_reg& instruction) override {
		format("ldr r%d, [r%d, r%d]", instruction.rt, instruction.rn, instruction.rm);
	}
	void dispatch(const ldrh_reg& instruction) override {
		format("ldrh r%d, [r%d, r%d]", instruction.rt, instruction.rn, instruction.rm);
	}
	void dispatch(const ldrb_reg& instruction) override {
		format("ldrb r%d, [r%d, r%d]", instruction.rt, instruction.rn, instruction.rm);
	}
	void dispatch(const ldrsh_reg& instruction) override {
		format("ldrsh r%d, [r%d, r%d]", instruction.rt, instruction.rn, instruction.rm);
	}
	void dispatch(const str_imm& instruction) override {
		format("str r%d, [r%d, #%x]", instruction.rt, instruction.rn, instruction.imm5);
	}
	void dispatch(const ldr_imm& instruction) override {
		format("str r%d, [r%d, #%x]", instruction.rt, instruction.rn, instruction.imm5);
	}
	void dispatch(const strb_imm& instruction) override {
		format("strb r%d, [r%d, #%x]", instruction.rt, instruction.rn, instruction.imm5);
	}
	void dispatch(const ldrb_imm& instruction) override {
		format("ldrb r%d, [r%d, #%x]", instruction.rt, instruction.rn, instruction.imm5);
	}
	void dispatch(const strh_imm& instruction) override {
		format("strh r%d, [r%d, #%x]", instruction.rt, instruction.rn, instruction.imm5);
	}
	void dispatch(const ldrh_imm& instruction) override {
		format("ldrh r%d, [r%d, #%x]", instruction.rt, instruction.rn, instruction.imm5);
	}
	void dispatch(const str_sp_imm& instruction) override {
		format("str r%d, [sp, #%x]", instruction.rt, instruction.imm8);
	}
	void dispatch(const ldr_sp_imm& instruction) override {
		format("ldr r%d, [sp, #%x]", instruction.rt, instruction.imm8);
	}
	void dispatch(const adr& instruction) override {
		format("adr r%d, #%x", instruction.rd, instruction.imm32());
	}
	void dispatch(const add_sp_imm& instruction) override {
		format("add r%d, sp, #%x", instruction.rd, instruction.imm32());
	}
	void dispatch(const add_sp_imm_t2& instruction) override {
		format("add sp, sp, #%x", instruction.imm32());
	}
	void dispatch(const sub_sp_imm& instruction) override {
		format("sub sp, sp, #%x", instruction.imm7);
	}
	void dispatch(const sxth& instruction) override {
		format("sxth r%d, r%d", instruction.rd, instruction.rm);
	}
	void dispatch(const sxtb& instruction) override {
		format("sxtb r%d, r%d", instruction.rd, instruction.rm);
	}
	void dispatch(const uxth& instruction) override {
		format("uxth r%d, r%d", instruction.rd, instruction.rm);
	}
	void dispatch(const uxtb& instruction) override {
		format("uxtb r%d, r%d", instruction.rd, instruction.rm);
	}
	void dispatch(const push& instruction) override {
		format("push {%s}", reg_list_str(instruction.register_list).c_str());
	}
	void dispatch(const pop& instruction) override {
		format("pop {%s}", reg_list_str(instruction.register_list).c_str());
	}
	void dispatch(const rev_word& instruction) override {
		format("rev r%d, r%d", instruction.rd, instruction.rm);
	}
	void dispatch(const rev16& instruction) override {
		format("rev16 r%d, r%d", instruction.rd, instruction.rm);
	}
	void dispatch(const revsh& instruction) override {
		format("revsh r%d, r%d", instruction.rd, instruction.rm);
	}
	void dispatch(const branch& instruction) override {
		format("b%2.2s.n 0x%x", condition_string(instruction.cond), instruction.offset());
	}
	void dispatch(const unconditional_branch& instruction) override {
		format("b %c%i", (instruction.offset() >= 0 ? '+' : ' '), instruction.offset());
	}
	void dispatch(const stm& instruction) override {
		format("stm! r%d {%s}", instruction.rn, reg_list_str(instruction.register_list).c_str());
	}
	void dispatch(const ldm& instruction) override {
		bool list_contains_rn = binops::get_bit(instruction.register_list, instruction.rn);
		format("ldm r%d%s, {%s}"
			, instruction.rn
			, list_contains_rn ? "!" : ""
			, reg_list_str(instruction.register_list).c_str()
		);
	}
	void dispatch(const mrs& instruction) override {
		format("mrs r%d, %s", instruction.rd, special_register(instruction.sysn));
	}
	void dispatch(const msr& instruction) override {
		format("mrs %s, r%d", special_register(instruction.sysn), instruction.rn);
	}
	void dispatch(const bl_imm& instruction) override {
		format("bl #%x", instruction.offset());
	}
	void dispatch(const svc& instruction) override {
		format("svc #%x", instruction.imm8);
	}
};

#endif //MICROMACHINE_DISASM_HPP
