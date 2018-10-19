#ifndef MICROMACHINE_DISASM_HPP
#define MICROMACHINE_DISASM_HPP

#include <numeric>
#include "dispatcher.hpp"
#include "string_format.hpp"

#define DISASM_FMT_HEX 				(1u << 0)
#define DISASM_FMT_HEX_NO_PREFIX 	(1u << 1)
#define DISASM_FMT_STD_REG_NAMES	(1u << 2)
#define DISASM_FMT_BRANCH_IS_NARROW	(1u << 3)
#define DISASM_FMT_USE_ALT_ADR		(1u << 4)
class disasm : public dispatcher {

public:

	static std::string disassemble_instruction(const instruction_pair instr, uint32_t address) {
		disasm d(address, DISASM_FMT_HEX);
		d.dispatch_instruction(instr);
		return d.instruction_string;
	}

private:

	disasm(size_t address, uint32_t fmt)
	 : instruction_string(128, '\0')
	 , _addr(address)
	 , format_options(fmt) {
	}

	std::string instruction_string;
	uint32_t _addr;
	const uint32_t format_options;

	void write(const std::string& str) {
		instruction_string = str;
	}

	template<typename ... Args>
	void format(const std::string& format, Args&&... args ) {
		write(string_format(format, std::forward<Args>(args)...));
	}


	const char* const reg_names_std[16] = {
		"r0","r1","r2","r3",
		"r4","r5","r6","r7",
		"r8","r9","r10","r11",
		"r12","sp","lr","pc"
	};

	const char* const reg_names_ex[16] = {
		"r0","r1","r2","r3",
		"r4","r5","r6","r7",
		"r8","r9","r10","r11",
		"ip","sp","lr","pc"
	};

	const char* R(reg_idx i) {
		if(format_use_std_reg_names()) {
			return reg_names_std[i];
		} else {
			return reg_names_ex[i];
		}
	}

	bool format_use_hex() {
		return format_options & DISASM_FMT_HEX;
	}

	bool format_hide_hex_prefix() {
		return format_options & DISASM_FMT_HEX_NO_PREFIX;
	}

	bool format_use_std_reg_names() {
		return format_options & DISASM_FMT_STD_REG_NAMES;
	}

	bool format_use_narrow_branch() {
		return format_options & DISASM_FMT_BRANCH_IS_NARROW;
	}

	bool format_use_alternate_adr() {
		return format_options & DISASM_FMT_USE_ALT_ADR;
	}

	std::string IMM() {
		if(format_use_hex()) {
			if(format_hide_hex_prefix()) {
				return "#%x";
			} else {
				return "#0x%x";
			}
		}
		return "#%d";
	}

	std::string join(const std::vector<std::string>& strings, const char* delim = ", ") {
		return std::accumulate(std::begin(strings), std::end(strings), std::string(), [=](const std::string &l, const std::string &r) {
			return l.empty() ? r : l + delim + r;
		});
	}

	std::string reg_list_str(const register_list_t& reg_list) {
		std::stringstream ss;
		std::vector<std::string> register_names;
		register_names.reserve(16);
		const reg_idx end = registers::NUM_REGS;
		for(reg_idx rid = 0; rid < end; rid++) {
			if(binops::get_bit(reg_list, rid)) {
				register_names.emplace_back(R(rid));
			}
		}
		return join(register_names, ", ");
	}

	const char* condition_string(uint8_t condition) {
		static const char* cond_names = "eqnecsccmiplvsvchilsgeltgtle";
		return cond_names + (condition * 2U);
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
			default: return "?";
		}
	}

	std::string special_register(uint8_t val) {
		return special_register((special_reg_instr::SpecialRegister)val);
	}

	void invalid_instruction(const uint16_t instr) override {
		format("<UNDEFINED> instruction: 0x%04x", (uint16_t)instr);
	}

	void invalid_instruction(const instruction_pair instr) override {
		format("<UNDEFINED> instruction: 0x%08x", instr.first() << 16 | instr.second());
	}

	//TODO: refactor and avoid passing _regs.app_status_register() explicitely
	void dispatch(const nop instruction) override {
		write("nop");
	}
	void dispatch(const yield instruction) override {
		write("yield");
	}
	void dispatch(const wfe instruction) override {
		write("wfe");
	}
	void dispatch(const wfi instruction) override {
		write("wfi");
	}
	void dispatch(const sev instruction) override {
		write("sev");
	}
	void dispatch(const lsl_imm instruction) override {
		format("lsls %s, %s, " + IMM(), R(instruction.rd), R(instruction.rm), instruction.imm5);
	}
	void dispatch(const lsr_imm instruction) override {
		format("lsrs %s, %s, " + IMM(), R(instruction.rd), R(instruction.rm), instruction.imm5);
	}
	void dispatch(const asr_imm instruction) override {
		format("asrs %s, %s, " + IMM(), R(instruction.rd), R(instruction.rm), instruction.imm5);
	}
	void dispatch(const add_reg instruction) override {
		format("adds %s, %s, %s", R(instruction.rd), R(instruction.rn), R(instruction.rm));
	}
	void dispatch(const subs_reg instruction) override {
		format("subs %s, %s, %s", R(instruction.rd), R(instruction.rn), R(instruction.rm));
	}
	void dispatch(const add_imm instruction) override {
		format("adds %s, %s, " + IMM(), R(instruction.rd), R(instruction.rn), instruction.imm3);
	}
	void dispatch(const subs_imm instruction) override {
		format("subs %s, %s, " + IMM(), R(instruction.rd), R(instruction.rn), instruction.imm3);
	}
	void dispatch(const mov_imm instruction) override {
		format("movs %s, " + IMM(), R(instruction.rd), instruction.imm8);
	}
	void dispatch(const movs instruction) override {
		format("movs %s, %s", R(instruction.rd), R(instruction.rm));
	}
	void dispatch(const cmp_imm instruction) override {
		format("cmp %s, " + IMM(), R(instruction.rn), instruction.imm8);
	}
	void dispatch(const add_imm_t2 instruction) override {
		format("adds %s, " + IMM(), R(instruction.rdn), instruction.imm8);
	}
	void dispatch(const subs_imm8 instruction) override {
		format("subs %s, " + IMM(), R(instruction.rdn), instruction.imm8);
	}
	void dispatch(const and_reg instruction) override {
		format("ands %s, %s", R(instruction.rdn), R(instruction.rm));
	}
	void dispatch(const eor_reg instruction) override {
		format("eors %s, %s", R(instruction.rdn), R(instruction.rm));
	}
	void dispatch(const lsl_reg instruction) override {
		format("lsls %s, %s", R(instruction.rdn), R(instruction.rm));
	}
	void dispatch(const lsr_reg instruction) override {
		format("lsrs %s, %s", R(instruction.rdn), R(instruction.rm));
	}
	void dispatch(const asr_reg instruction) override {
		format("asrs %s, %s", R(instruction.rdn), R(instruction.rm));
	}
	void dispatch(const adc instruction) override {
		format("adcs %s, %s", R(instruction.rdn), R(instruction.rm));
	}
	void dispatch(const sbc instruction) override {
		format("sbcs %s, %s", R(instruction.rdn), R(instruction.rm));
	}
	void dispatch(const ror_reg instruction) override {
		format("rors %s, %s", R(instruction.rdn), R(instruction.rm));
	}
	void dispatch(const tst_reg instruction) override {
		format("tst %s, %s", R(instruction.rn), R(instruction.rm));
	}
	void dispatch(const rsb_imm instruction) override {
		format("rsbs %s, %s, #0", R(instruction.rd), R(instruction.rn));
	}
	void dispatch(const cmp_reg instruction) override {
		format("cmp %s, %s", R(instruction.rn), R(instruction.rm));
	}
	void dispatch(const cmn_reg instruction) override {
		format("cmn %s, %s", R(instruction.rn), R(instruction.rm));
	}
	void dispatch(const orr_reg instruction) override {
		format("orrs %s, %s", R(instruction.rdn), R(instruction.rm));
	}
	void dispatch(const mul_reg instruction) override {
		format("muls %s, %s", R(instruction.rdm), R(instruction.rn));
	}
	void dispatch(const bic_reg instruction) override {
		format("bics %s, %s", R(instruction.rdn), R(instruction.rm));
	}
	void dispatch(const mvn instruction) override {
		format("mvns %s, %s", R(instruction.rd), R(instruction.rm));
	}
	void dispatch(const add_highreg instruction) override {
		// variants of add SP plus register
		if(instruction.dm && (instruction.high_rm() == 13)) {
			format("add %s, sp, %s", R(instruction.rdn));
		} else if(instruction.high_rd() == 13) {
			format("add sp, %s", R(instruction.high_rm()));
		} else {
			format("add %s, %s", R(instruction.high_rd()), R(instruction.high_rm()));
		}
	}
	void dispatch(const cmp_highreg instruction) override {
		format("cmp %s, %s", R(instruction.high_rn()), R(instruction.high_rm()));
	}
	void dispatch(const mov_highreg instruction) override {
		format("mov %s, %s", R(instruction.high_rd()), R(instruction.high_rm()));
	}
	void dispatch(const bx instruction) override {
		format("bx %s", R(instruction.rm));
	}
	void dispatch(const blx instruction) override {
		format("blx %s", R(instruction.rm));
	}
	void dispatch(const ldr_literal instruction) override {
		// normal syntax
		//format("ldr %s, %x", R(instruction.rt), instruction.imm32()+_addr);
		// alternative syntax
		format("ldr %s, [pc, " + IMM() + "]", R(instruction.rt), instruction.imm32());
	}
	void dispatch(const str_reg instruction) override {
		format("str %s, [%s, %s]", R(instruction.rt), R(instruction.rn), R(instruction.rm));
	}
	void dispatch(const strh_reg instruction) override {
		format("strh %s, [%s, %s]", R(instruction.rt), R(instruction.rn), R(instruction.rm));
	}
	void dispatch(const strb_reg instruction) override {
		format("strb %s, [%s, %s]", R(instruction.rt), R(instruction.rn), R(instruction.rm));
	}
	void dispatch(const ldrsb_reg instruction) override {
		format("ldrsb %s, [%s, %s]", R(instruction.rt), R(instruction.rn), R(instruction.rm));
	}
	void dispatch(const ldr_reg instruction) override {
		format("ldr %s, [%s, %s]", R(instruction.rt), R(instruction.rn), R(instruction.rm));
	}
	void dispatch(const ldrh_reg instruction) override {
		format("ldrh %s, [%s, %s]", R(instruction.rt), R(instruction.rn), R(instruction.rm));
	}
	void dispatch(const ldrb_reg instruction) override {
		format("ldrb %s, [%s, %s]", R(instruction.rt), R(instruction.rn), R(instruction.rm));
	}
	void dispatch(const ldrsh_reg instruction) override {
		format("ldrsh %s, [%s, %s]", R(instruction.rt), R(instruction.rn), R(instruction.rm));
	}
	void dispatch(const str_imm instruction) override {
		format("str %s, [%s, " + IMM() + "]", R(instruction.rt), R(instruction.rn), instruction.imm32());
	}
	void dispatch(const ldr_imm instruction) override {
		format("ldr %s, [%s, " + IMM() + "]", R(instruction.rt), R(instruction.rn), instruction.imm32());
	}
	void dispatch(const strb_imm instruction) override {
		format("strb %s, [%s, " + IMM() + "]", R(instruction.rt), R(instruction.rn), instruction.imm5);
	}
	void dispatch(const ldrb_imm instruction) override {
		format("ldrb %s, [%s, " + IMM() + "]", R(instruction.rt), R(instruction.rn), instruction.imm5);
	}
	void dispatch(const strh_imm instruction) override {
		format("strh %s, [%s, " + IMM() + "]", R(instruction.rt), R(instruction.rn), instruction.imm32());
	}
	void dispatch(const ldrh_imm instruction) override {
		format("ldrh %s, [%s, " + IMM() + "]", R(instruction.rt), R(instruction.rn), instruction.imm32());
	}
	void dispatch(const str_sp_imm instruction) override {
		format("str %s, [sp, " + IMM() + "]", R(instruction.rt), instruction.imm32());
	}
	void dispatch(const ldr_sp_imm instruction) override {
		format("ldr %s, [sp, " + IMM() + "]", R(instruction.rt), instruction.imm32());
	}
	void dispatch(const adr instruction) override {
		if(format_use_alternate_adr()) {
			// alternate syntax
			format("add %s, pc, " + IMM(), R(instruction.rd), instruction.imm32());
		} else {
			// normal syntax
			format("adr %s, " + IMM(), R(instruction.rd), instruction.imm32());
		}
	}
	void dispatch(const add_sp_imm instruction) override {
		format("add %s, sp, " + IMM(), R(instruction.rd), instruction.imm32());
	}
	void dispatch(const add_sp_imm_t2 instruction) override {
		format("add sp, " + IMM(), instruction.imm32());
	}
	void dispatch(const sub_sp_imm instruction) override {
		format("sub sp, sp, " + IMM(), instruction.imm32());
	}
	void dispatch(const sxth instruction) override {
		format("sxth %s, %s", R(instruction.rd), R(instruction.rm));
	}
	void dispatch(const sxtb instruction) override {
		format("sxtb %s, %s", R(instruction.rd), R(instruction.rm));
	}
	void dispatch(const uxth instruction) override {
		format("uxth %s, %s", R(instruction.rd), R(instruction.rm));
	}
	void dispatch(const uxtb instruction) override {
		format("uxtb %s, %s", R(instruction.rd), R(instruction.rm));
	}
	void dispatch(const push instruction) override {
		format("push {%s}", reg_list_str(instruction.register_list).c_str());
	}
	void dispatch(const cps instruction) override {
		format("cps%s i", instruction.im ? "id" : "ie");
	}
	void dispatch(const pop instruction) override {
		format("pop {%s}", reg_list_str(instruction.register_list).c_str());
	}
	void dispatch(const bkpt instruction) override {
		format("bkpt " + IMM(), instruction.imm8);
	}
	void dispatch(const rev_word instruction) override {
		format("rev %s, %s", R(instruction.rd), R(instruction.rm));
	}
	void dispatch(const rev16 instruction) override {
		format("rev16 %s, %s", R(instruction.rd), R(instruction.rm));
	}
	void dispatch(const revsh instruction) override {
		format("revsh %s, %s", R(instruction.rd), R(instruction.rm));
	}
	void dispatch(const branch instruction) override {
		int32_t offset = instruction.offset();
		uint32_t label = (_addr + 4) + offset;
		std::string narrow_suffix = format_use_narrow_branch() ? ".n" : "";
		format("b%2.2s"+narrow_suffix+" "+IMM(), condition_string(instruction.cond), label);
	}
	void dispatch(const unconditional_branch instruction) override {
		int32_t offset = instruction.offset();
		uint32_t label = (_addr + 4) + offset;
		format("b.n 0x%x", label);
	}
	void dispatch(const stm instruction) override {
		format("stmia %s!, {%s}"
			, R(instruction.rn)
			, reg_list_str(instruction.register_list).c_str()
		);
	}
	void dispatch(const ldm instruction) override {
		bool list_contains_rn = binops::get_bit(instruction.register_list, instruction.rn);
		format("ldmia %s%s, {%s}"
			, R(instruction.rn)
			, list_contains_rn ? "" : "!"
			, reg_list_str(instruction.register_list).c_str()
		);
	}
	void dispatch(const mrs instruction) override {
		format("mrs %s, %s", R(instruction.rd), special_register(instruction.sysn).c_str());
	}
	void dispatch(const msr instruction) override {
		format("mrs %s, %s", special_register(instruction.sysn).c_str(), R(instruction.rn));
	}
	void dispatch(const bl_imm instruction) override {
		int32_t offset = instruction.offset();
		uint32_t label = (_addr + 4) + offset;
		format("bl 0x%x", label);
	}
	void dispatch(const svc instruction) override {
		format("svc %d", instruction.imm8);
	}
	void dispatch(const udf instr) override {
		format("udf " + IMM(), instr.imm32);
	}
	void dispatch(const udfw instr) override {
		format("udf.w " + IMM(), instr.imm32);
	}
};

#endif //MICROMACHINE_DISASM_HPP
