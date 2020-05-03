#pragma once

#include "helpers/binops.hpp"
#include "instruction/instruction_decoder.hpp"
#include "instruction/instruction_pair.hpp"
#include "registers/core_registers/core_registers.hpp"
#include "string_format.hpp"

#include <numeric>

#define DISASM_FMT_HEX (1U << 0U)
#define DISASM_FMT_HEX_NO_PREFIX (1U << 1U)
#define DISASM_FMT_STD_REG_NAMES (1U << 2U)
#define DISASM_FMT_BRANCH_IS_NARROW (1U << 3U)
#define DISASM_FMT_USE_ALT_ADR (1U << 4U)

namespace micromachine::system {

class disassembler : public instruction_decoder {

public:
	static std::string disassemble_instruction(const instruction_pair instr, uint32_t address) {
		disassembler d(address, DISASM_FMT_HEX);
		d.decode_instruction(instr);
		return d.instruction_string;
	}

private:
	disassembler(size_t address, uint32_t fmt)
		: instruction_string(128, '\0')
		, _addr(address)
		, format_options(fmt) {}

	std::string instruction_string;
	uint32_t _addr;
	const uint32_t format_options;

	void write(const std::string& str) {
		instruction_string = str;
	}

	template <typename... Args>
	void format(const std::string& format, Args&&... args) {
		write(micomachine::utils::string_format(format, std::forward<Args>(args)...));
	}

	const char* const reg_names_std[16] = {"r0",
										   "r1",
										   "r2",
										   "r3",
										   "r4",
										   "r5",
										   "r6",
										   "r7",
										   "r8",
										   "r9",
										   "r10",
										   "r11",
										   "r12",
										   "sp",
										   "lr",
										   "pc"};

	const char* const reg_names_ex[16] = {"r0",
										  "r1",
										  "r2",
										  "r3",
										  "r4",
										  "r5",
										  "r6",
										  "r7",
										  "r8",
										  "r9",
										  "r10",
										  "r11",
										  "ip",
										  "sp",
										  "lr",
										  "pc"};

	const char* R(reg_idx i) {
		if(format_use_std_reg_names()) {
			return reg_names_std[i];
		} else {
			return reg_names_ex[i];
		}
	}

	bool format_use_hex() const {
		return format_options & DISASM_FMT_HEX;
	}

	bool format_hide_hex_prefix() const {
		return format_options & DISASM_FMT_HEX_NO_PREFIX;
	}

	bool format_use_std_reg_names() const {
		return format_options & DISASM_FMT_STD_REG_NAMES;
	}

	bool format_use_narrow_branch() const {
		return format_options & DISASM_FMT_BRANCH_IS_NARROW;
	}

	bool format_use_alternate_adr() const {
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

	static std::string join(const std::vector<std::string>& strings, const char* delim = ", ") {
		return std::accumulate(std::begin(strings),
							   std::end(strings),
							   std::string(),
							   [=](const std::string& l, const std::string& r) {
								   return l.empty() ? r : l + delim + r;
							   });
	}

	std::string reg_list_str(const register_list_t& reg_list) {
		std::stringstream ss;
		std::vector<std::string> register_names;
		register_names.reserve(16);
		const reg_idx end = core_registers::NUM_CORE_REGS;
		for(reg_idx rid = 0; rid < end; rid++) {
			if(binops::get_bit(reg_list, rid)) {
				register_names.emplace_back(R(rid));
			}
		}
		return join(register_names, ", ");
	}

	static const char* condition_string(uint8_t condition) {
		static const char* cond_names = "eqnecsccmiplvsvchilsgeltgtle";
		return cond_names + (condition * 2U);
	}

	static std::string special_register(special_reg_instr::SpecialRegister sr) {
		switch(sr) {
			case msr::SpecialRegister::APSR:
				return "apsr";
			case msr::SpecialRegister::IAPSR:
				return "iapsr";
			case msr::SpecialRegister::EAPSR:
				return "eapsr";
			case msr::SpecialRegister::XPSR:
				return "xpsr";
			case msr::SpecialRegister::IPSR:
				return "ipsr";
			case msr::SpecialRegister::EPSR:
				return "epsr";
			case msr::SpecialRegister::IEPSR:
				return "iepsr";
			case msr::SpecialRegister::MSP:
				return "msp";
			case msr::SpecialRegister::PSP:
				return "psp";
			case msr::SpecialRegister::PRIMASK:
				return "primask";
			case msr::SpecialRegister::CONTROL:
				return "control";
			default:
				return "?";
		}
	}

	static std::string special_register(uint8_t val) {
		return special_register((special_reg_instr::SpecialRegister)val);
	}

	void invalid_instruction(const uint16_t instr) override {
		format("<UNDEFINED> instruction: 0x%04x", (uint16_t)instr);
	}

	void invalid_instruction(const instruction_pair instr) override {
		format("<UNDEFINED> instruction: 0x%08x", instr.first() << 16U | instr.second());
	}

	// TODO: refactor and avoid passing _regs.app_status_register() explicitely
	void dispatch_nop(const nop) override {
		write("nop");
	}
	void dispatch_yield(const yield) override {
		write("yield");
	}
	void dispatch_wfe(const wfe) override {
		write("wfe");
	}
	void dispatch_wfi(const wfi) override {
		write("wfi");
	}
	void dispatch_sev(const sev) override {
		write("sev");
	}
	void dispatch_lsl_imm(const lsl_imm instruction) override {
		format("lsls %s, %s, " + IMM(),
			   R(instruction.rd()),
			   R(instruction.rm()),
			   instruction.imm5().extract());
	}
	void dispatch_lsr_imm(const lsr_imm instruction) override {
		format("lsrs %s, %s, " + IMM(),
			   R(instruction.rd()),
			   R(instruction.rm()),
			   instruction.imm5().extract());
	}
	void dispatch_asr_imm(const asr_imm instruction) override {
		format("asrs %s, %s, " + IMM(),
			   R(instruction.rd()),
			   R(instruction.rm()),
			   instruction.imm5().extract());
	}
	void dispatch_add_reg(const add_reg instruction) override {
		format("adds %s, %s, %s", R(instruction.rd()), R(instruction.rn()), R(instruction.rm()));
	}
	void dispatch_subs_reg(const subs_reg instruction) override {
		format("subs %s, %s, %s", R(instruction.rd()), R(instruction.rn()), R(instruction.rm()));
	}
	void dispatch_add_imm(const add_imm instruction) override {
		format("adds %s, %s, " + IMM(),
			   R(instruction.rd()),
			   R(instruction.rn()),
			   instruction.imm3().extract());
	}
	void dispatch_subs_imm(const subs_imm instruction) override {
		format("subs %s, %s, " + IMM(),
			   R(instruction.rd()),
			   R(instruction.rn()),
			   instruction.imm3().extract());
	}
	void dispatch_mov_imm(const mov_imm instruction) override {
		format("movs %s, " + IMM(), R(instruction.rd()), instruction.imm8().extract());
	}
	void dispatch_movs(const movs instruction) override {
		format("movs %s, %s", R(instruction.rd()), R(instruction.rm()));
	}
	void dispatch_cmp_imm(const cmp_imm instruction) override {
		format("cmp %s, " + IMM(), R(instruction.rn()), instruction.imm8().extract());
	}
	void dispatch_add_imm_t2(const add_imm_t2 instruction) override {
		format("adds %s, " + IMM(), R(instruction.rdn()), instruction.imm8().extract());
	}
	void dispatch_subs_imm8(const subs_imm8 instruction) override {
		format("subs %s, " + IMM(), R(instruction.rdn()), instruction.imm8().extract());
	}
	void dispatch_and_reg(const and_reg instruction) override {
		format("ands %s, %s", R(instruction.rdn()), R(instruction.rm()));
	}
	void dispatch_eor_reg(const eor_reg instruction) override {
		format("eors %s, %s", R(instruction.rdn()), R(instruction.rm()));
	}
	void dispatch_lsl_reg(const lsl_reg instruction) override {
		format("lsls %s, %s", R(instruction.rdn()), R(instruction.rm()));
	}
	void dispatch_lsr_reg(const lsr_reg instruction) override {
		format("lsrs %s, %s", R(instruction.rdn()), R(instruction.rm()));
	}
	void dispatch_asr_reg(const asr_reg instruction) override {
		format("asrs %s, %s", R(instruction.rdn()), R(instruction.rm()));
	}
	void dispatch_adc(const adc instruction) override {
		format("adcs %s, %s", R(instruction.rdn()), R(instruction.rm()));
	}
	void dispatch_sbc(const sbc instruction) override {
		format("sbcs %s, %s", R(instruction.rdn()), R(instruction.rm()));
	}
	void dispatch_ror_reg(const ror_reg instruction) override {
		format("rors %s, %s", R(instruction.rdn()), R(instruction.rm()));
	}
	void dispatch_tst_reg(const tst_reg instruction) override {
		format("tst %s, %s", R(instruction.rn()), R(instruction.rm()));
	}
	void dispatch_rsb_imm(const rsb_imm instruction) override {
		format("rsbs %s, %s, #0", R(instruction.rd()), R(instruction.rn()));
	}
	void dispatch_cmp_reg(const cmp_reg instruction) override {
		format("cmp %s, %s", R(instruction.rn()), R(instruction.rm()));
	}
	void dispatch_cmn_reg(const cmn_reg instruction) override {
		format("cmn %s, %s", R(instruction.rn()), R(instruction.rm()));
	}
	void dispatch_orr_reg(const orr_reg instruction) override {
		format("orrs %s, %s", R(instruction.rdn()), R(instruction.rm()));
	}
	void dispatch_mul_reg(const mul_reg instruction) override {
		format("muls %s, %s", R(instruction.rdm()), R(instruction.rn()));
	}
	void dispatch_bic_reg(const bic_reg instruction) override {
		format("bics %s, %s", R(instruction.rdn()), R(instruction.rm()));
	}
	void dispatch_mvn(const mvn instruction) override {
		format("mvns %s, %s", R(instruction.rd()), R(instruction.rm()));
	}
	void dispatch_add_highreg(const add_highreg instruction) override {
		// variants of add SP plus register
		if(instruction.dn() && (instruction.high_rm() == 13)) {
			format("add %s, sp, %s", R(instruction.rdn()));
		} else if(instruction.high_rd() == 13) {
			format("add sp, %s", R(instruction.high_rm()));
		} else {
			format("add %s, %s", R(instruction.high_rd()), R(instruction.high_rm()));
		}
	}
	void dispatch_cmp_highreg(const cmp_highreg instruction) override {
		format("cmp %s, %s", R(instruction.high_rn()), R(instruction.high_rm()));
	}
	void dispatch_mov_highreg(const mov_highreg instruction) override {
		format("mov %s, %s", R(instruction.high_rd()), R(instruction.high_rm()));
	}
	void dispatch_bx(const bx instruction) override {
		format("bx %s", R(instruction.rm()));
	}
	void dispatch_blx(const blx instruction) override {
		format("blx %s", R(instruction.rm()));
	}
	void dispatch_ldr_literal(const ldr_literal instruction) override {
		// normal syntax
		// format("ldr %s, %x", R(instruction.rt), instruction.imm32()+_addr);
		// alternative syntax
		format("ldr %s, [pc, " + IMM() + "]", R(instruction.rt()), instruction.imm32());
	}
	void dispatch_str_reg(const str_reg instruction) override {
		format("str %s, [%s, %s]", R(instruction.rt()), R(instruction.rn()), R(instruction.rm()));
	}
	void dispatch_strh_reg(const strh_reg instruction) override {
		format("strh %s, [%s, %s]", R(instruction.rt()), R(instruction.rn()), R(instruction.rm()));
	}
	void dispatch_strb_reg(const strb_reg instruction) override {
		format("strb %s, [%s, %s]", R(instruction.rt()), R(instruction.rn()), R(instruction.rm()));
	}
	void dispatch_ldrsb_reg(const ldrsb_reg instruction) override {
		format("ldrsb %s, [%s, %s]", R(instruction.rt()), R(instruction.rn()), R(instruction.rm()));
	}
	void dispatch_ldr_reg(const ldr_reg instruction) override {
		format("ldr %s, [%s, %s]", R(instruction.rt()), R(instruction.rn()), R(instruction.rm()));
	}
	void dispatch_ldrh_reg(const ldrh_reg instruction) override {
		format("ldrh %s, [%s, %s]", R(instruction.rt()), R(instruction.rn()), R(instruction.rm()));
	}
	void dispatch_ldrb_reg(const ldrb_reg instruction) override {
		format("ldrb %s, [%s, %s]", R(instruction.rt()), R(instruction.rn()), R(instruction.rm()));
	}
	void dispatch_ldrsh_reg(const ldrsh_reg instruction) override {
		format("ldrsh %s, [%s, %s]", R(instruction.rt()), R(instruction.rn()), R(instruction.rm()));
	}
	void dispatch_str_imm(const str_imm instruction) override {
		format("str %s, [%s, " + IMM() + "]",
			   R(instruction.rt()),
			   R(instruction.rn()),
			   instruction.imm32());
	}
	void dispatch_ldr_imm(const ldr_imm instruction) override {
		format("ldr %s, [%s, " + IMM() + "]",
			   R(instruction.rt()),
			   R(instruction.rn()),
			   instruction.imm32());
	}
	void dispatch_strb_imm(const strb_imm instruction) override {
		format("strb %s, [%s, " + IMM() + "]",
			   R(instruction.rt()),
			   R(instruction.rn()),
			   instruction.imm5().extract());
	}
	void dispatch_ldrb_imm(const ldrb_imm instruction) override {
		format("ldrb %s, [%s, " + IMM() + "]",
			   R(instruction.rt()),
			   R(instruction.rn()),
			   instruction.imm5().extract());
	}
	void dispatch_strh_imm(const strh_imm instruction) override {
		format("strh %s, [%s, " + IMM() + "]",
			   R(instruction.rt()),
			   R(instruction.rn()),
			   instruction.imm32());
	}
	void dispatch_ldrh_imm(const ldrh_imm instruction) override {
		format("ldrh %s, [%s, " + IMM() + "]",
			   R(instruction.rt()),
			   R(instruction.rn()),
			   instruction.imm32());
	}
	void dispatch_str_sp_imm(const str_sp_imm instruction) override {
		format("str %s, [sp, " + IMM() + "]", R(instruction.rt()), instruction.imm32());
	}
	void dispatch_ldr_sp_imm(const ldr_sp_imm instruction) override {
		format("ldr %s, [sp, " + IMM() + "]", R(instruction.rt()), instruction.imm32());
	}
	void dispatch_adr(const adr instruction) override {
		if(format_use_alternate_adr()) {
			// alternate syntax
			format("add %s, pc, " + IMM(), R(instruction.rd()), instruction.imm32());
		} else {
			// normal syntax
			format("adr %s, " + IMM(), R(instruction.rd()), instruction.imm32());
		}
	}
	void dispatch_add_sp_imm(const add_sp_imm instruction) override {
		format("add %s, sp, " + IMM(), R(instruction.rd()), instruction.imm32());
	}
	void dispatch_add_sp_imm_t2(const add_sp_imm_t2 instruction) override {
		format("add sp, " + IMM(), instruction.imm32());
	}
	void dispatch_sub_sp_imm(const sub_sp_imm instruction) override {
		format("sub sp, sp, " + IMM(), instruction.imm32());
	}
	void dispatch_sxth(const sxth instruction) override {
		format("sxth %s, %s", R(instruction.rd()), R(instruction.rm()));
	}
	void dispatch_sxtb(const sxtb instruction) override {
		format("sxtb %s, %s", R(instruction.rd()), R(instruction.rm()));
	}
	void dispatch_uxth(const uxth instruction) override {
		format("uxth %s, %s", R(instruction.rd()), R(instruction.rm()));
	}
	void dispatch_uxtb(const uxtb instruction) override {
		format("uxtb %s, %s", R(instruction.rd()), R(instruction.rm()));
	}
	void dispatch_push(const push instruction) override {
		format("push {%s}", reg_list_str(instruction.value()).c_str());
	}
	void dispatch_cps(const cps instruction) override {
		format("cps%s i", instruction.im ? "id" : "ie");
	}
	void dispatch_pop(const pop instruction) override {
		format("pop {%s}", reg_list_str(instruction.value()).c_str());
	}
	void dispatch_bkpt(const bkpt instruction) override {
		format("bkpt " + IMM(), instruction.imm8().extract());
	}
	void dispatch_rev_word(const rev_word instruction) override {
		format("rev %s, %s", R(instruction.rd()), R(instruction.rm()));
	}
	void dispatch_rev16(const rev16 instruction) override {
		format("rev16 %s, %s", R(instruction.rd()), R(instruction.rm()));
	}
	void dispatch_revsh(const revsh instruction) override {
		format("revsh %s, %s", R(instruction.rd()), R(instruction.rm()));
	}
	void dispatch_branch(const branch instruction) override {
		int32_t offset = instruction.offset();
		uint32_t label = (_addr + 4) + offset;
		std::string narrow_suffix = format_use_narrow_branch() ? ".n" : "";
		format("b%2.2s" + narrow_suffix + " " + IMM(), condition_string(instruction.cond()), label);
	}
	void dispatch_unconditional_branch(const unconditional_branch instruction) override {
		int32_t offset = instruction.offset();
		uint32_t label = (_addr + 4) + offset;
		format("b.n 0x%x", label);
	}
	void dispatch_stm(const stm instruction) override {
		format("stmia %s!, {%s}",
			   R(instruction.rn()),
			   reg_list_str(instruction.register_list()).c_str());
	}
	void dispatch_ldm(const ldm instruction) override {
		bool list_contains_rn = binops::get_bit(instruction.register_list(), instruction.rn());
		format("ldmia %s%s, {%s}",
			   R(instruction.rn()),
			   list_contains_rn ? "" : "!",
			   reg_list_str(instruction.register_list()).c_str());
	}
	void dispatch_mrs(const mrs instruction) override {
		format("mrs %s, %s", R(instruction.rd), special_register(instruction.sysn).c_str());
	}
	void dispatch_msr(const msr instruction) override {
		format("mrs %s, %s", special_register(instruction.sysn).c_str(), R(instruction.rn));
	}
	void dispatch_bl_imm(const bl_imm instruction) override {
		int32_t offset = instruction.offset();
		uint32_t label = (_addr + 4) + offset;
		format("bl 0x%x", label);
	}
	void dispatch_svc(const svc instruction) override {
		format("svc %d", instruction.imm8().extract());
	}
	void dispatch_dmb(const dmb instruction) override {
		format("dmb " + IMM(), instruction.imm4().extract());
	}
	void dispatch_dsb(const dsb instruction) override {
		format("dsb " + IMM(), instruction.imm4().extract());
	}
	void dispatch_isb(const isb instruction) override {
		format("isb " + IMM(), instruction.imm4().extract());
	}
	void dispatch_udf(const udf instr) override {
		format("udf " + IMM(), instr.imm32);
	}
	void dispatch_udfw(const udfw instr) override {
		format("udf.w " + IMM(), instr.imm32);
	}
};

} // namespace micromachine::system
