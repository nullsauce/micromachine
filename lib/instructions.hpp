//
// Created by fla on 4/30/17.
//

#ifndef THUMBEMU_INSTRUCTIONS_HPP
#define THUMBEMU_INSTRUCTIONS_HPP

#include <cstdint>
#include <string>
#include <sstream>
#include "types.hpp"
#include "registers/registers.hpp"
#include "string_format.hpp"

struct standard_rd_rm_imm5 {

	standard_rd_rm_imm5(halfword field)
			: rd  (binops::read_uint(field, 0, 3))
			, rm  (binops::read_uint(field, 3, 3))
			, imm5(binops::read_uint(field, 6, 5))
	{}

	const reg_idx rd;
	const reg_idx rm;
	const imm5_t  imm5;
};


struct standard_rd_rm_rn {

	standard_rd_rm_rn(halfword field)
		: rd(binops::read_uint(field, 0, 3))
		, rm(binops::read_uint(field, 3, 3))
		, rn(binops::read_uint(field, 6, 3))
	{}

	const reg_idx rd;
	const reg_idx rm;
	const reg_idx rn;
};

struct standard_rd_rn_rm {

	standard_rd_rn_rm(halfword field)
			: rd(binops::read_uint(field, 0, 3))
			, rn(binops::read_uint(field, 3, 3))
			, rm(binops::read_uint(field, 6, 3))
	{}

	const reg_idx rd;
	const reg_idx rn;
	const reg_idx rm;
};

struct standard_rd_rm_imm3 {

	standard_rd_rm_imm3(halfword field)
			: rd  (binops::read_uint(field, 0, 3))
			, rm  (binops::read_uint(field, 3, 3))
			, imm3(binops::read_uint(field, 6, 3))
	{}

	const reg_idx rd;
	const reg_idx rm;
	const imm3_t  imm3;
};


struct standard_rd_rn_imm3 {

	standard_rd_rn_imm3(halfword field)
			: rd  (binops::read_uint(field, 0, 3))
			, rn  (binops::read_uint(field, 3, 3))
			, imm3(binops::read_uint(field, 6, 3))
	{}

	const reg_idx rd;
	const reg_idx rn;
	const imm3_t  imm3;
};


struct standard_imm8_rd {

	standard_imm8_rd(halfword field)
		: imm8(binops::read_uint(field, 0, 8))
		, rd  (binops::read_uint(field, 8, 3))
	{}

	const imm8_t  imm8;
	const reg_idx rd;
};

struct standard_imm8_rn {

	standard_imm8_rn(halfword field)
			: imm8(binops::read_uint(field, 0, 8))
			, rn  (binops::read_uint(field, 8, 3))
	{}

	const imm8_t  imm8;
	const reg_idx rn;
};

struct standard_rdn_rm {

	standard_rdn_rm(halfword field)
		: rdn(binops::read_uint(field, 0, 3))
		, rm (binops::read_uint(field, 3, 3))
	{}

	const reg_idx rdn;
	const reg_idx rm;
};

struct standard_rdm_rn {

	standard_rdm_rn(halfword field)
			: rdm(binops::read_uint(field, 0, 3))
			, rn (binops::read_uint(field, 3, 3))
	{}

	const reg_idx rdm;
	const reg_idx rn;
};

// rdn can be a high register if dm is set
struct standard_rdn_rm_dm {

	standard_rdn_rm_dm(halfword field)
		: rdn(binops::read_uint(field, 0, 3))
		, rm (binops::read_uint(field, 3, 4))
		, dm (binops::get_bit(field, 7))
	{}

	// returns the possibly high register index
	reg_idx high_rd() const {
		return (dm * 8) + rdn;
	}

	// returns the possibly high register index
	reg_idx high_rm() const {
		return rm;
	}

	const reg_idx rdn;
	const reg_idx rm;
	const bool dm;
};

// rd can be a high register if dm is set
struct standard_rn_rm_dm {

	standard_rn_rm_dm(halfword field)
			: rn(binops::read_uint(field, 0, 3))
			, rm(binops::read_uint(field, 3, 4))
			, dm(binops::get_bit(field, 7))
	{}

	// returns the possibly high register index
	reg_idx high_rn() const {
		return (dm * 8) + rn;
	}

	// returns the possibly high register index
	reg_idx high_rm() const {
		return rm;
	}

	const reg_idx rn;
	const reg_idx rm;
	const bool dm;
};


// rd can be a high register if dm is set
struct standard_rd_rm_d {

	standard_rd_rm_d(halfword field)
		: rd(binops::read_uint(field, 0, 3))
		, rm(binops::read_uint(field, 3, 4))
		, d (binops::get_bit(field, 7))
	{}

	// returns the possibly high register index
	reg_idx high_rd() const {
		return (d * 8) + rd;
	}

	// returns the possibly high register index
	reg_idx high_rm() const {
		return rm;
	}

	const reg_idx rd;
	const reg_idx rm;
	const bool d;
};

struct standard_rn_rm {

	standard_rn_rm(halfword field)
			: rn(binops::read_uint(field, 0, 3))
			, rm(binops::read_uint(field, 3, 3))
	{}

	const reg_idx rn;
	const reg_idx rm;
};

struct standard_rd_rn {

	standard_rd_rn(halfword field)
		: rd(binops::read_uint(field, 0, 3))
		, rn(binops::read_uint(field, 3, 3))
	{}

	const reg_idx rd;
	const reg_idx rn;
};


struct standard_rm {

	standard_rm(halfword field)
		: rm(binops::read_uint(field, 3, 4))
	{}

	const reg_idx rm;
};

struct standard_rd_rm {

	standard_rd_rm(halfword field)
		: rd(binops::read_uint(field, 0, 3))
		, rm(binops::read_uint(field, 3, 3))
	{}

	const reg_idx rd;
	const reg_idx rm;
};

struct standard_imm8_rdn {

	standard_imm8_rdn(halfword field)
		: imm8(binops::read_uint(field, 0, 8))
		, rdn (binops::read_uint(field, 8, 3))
	{}

	const imm8_t  imm8;
	const reg_idx rdn;
};


struct standard_rt_rn_rm {

	standard_rt_rn_rm(halfword field)
		: rt(binops::read_uint(field, 0, 3))
		, rn(binops::read_uint(field, 3, 3))
		, rm(binops::read_uint(field, 6, 3))
	{}

	const reg_idx rt;
	const reg_idx rn;
	const reg_idx rm;
};

struct standard_rt_rn_imm5 {

	standard_rt_rn_imm5(halfword field)
		: rt  (binops::read_uint(field, 0, 3))
		, rn  (binops::read_uint(field, 3, 3))
		, imm5(binops::read_uint(field, 6, 5))
	{}

	const reg_idx rt;
	const reg_idx rn;
	const imm5_t  imm5;
};

struct standard_imm8_rt {

	standard_imm8_rt(halfword field)
		: imm8(binops::read_uint(field, 0, 8))
		, rt  (binops::read_uint(field, 8, 3))
	{}

	const imm8_t  imm8;
	const reg_idx rt;
};

struct standard_imm7 {

	standard_imm7(halfword field)
		: imm7(binops::read_uint(field, 0, 7))
	{}

	const imm7_t  imm7;
};

struct standard_imm8 {

	standard_imm8(halfword field)
		: imm8(binops::read_uint(field, 0, 8))
	{}

	const imm8_t imm8;
};

struct standard_imm11 {

	standard_imm11(halfword field)
		: imm11(binops::read_uint(field, 0, 11))
	{}

	const imm11_t imm11;
};

struct standard_push_register_list {

	standard_push_register_list(halfword field)
		: register_list(
			(binops::read_uint(field, 0, 8)) |
			(binops::get_bit(field, 8) << 14)
		)
	{}

	bool is_set(reg_idx reg) const {
		return binops::get_bit(register_list, reg);
	}

    // TODO: rename to push_count
	uint32_t pop_count() const {
		return __builtin_popcount(register_list);
	}

	const register_list_t register_list;
};


struct standard_pop_register_list {

	standard_pop_register_list(halfword field)
			: register_list(
			(binops::read_uint(field, 0, 8)) |
			(binops::get_bit(field, 8) << 15)
	)
	{}

	bool is_set(reg_idx reg) const {
		return binops::get_bit(register_list, reg);
	}

	uint32_t pop_count() const {
		return __builtin_popcount(register_list);
	}

	const register_list_t register_list;
};

struct standard_register_list_rm {
	standard_register_list_rm(halfword field)
		: reg_list(binops::read_uint(field, 0, 8))
		, rm(binops::read_uint(field, 8, 3))
	{}

	bool is_set(reg_idx reg) const {
		return binops::get_bit(reg_list, reg);
	}

	uint32_t pop_count() const {
		return __builtin_popcount(reg_list);
	}

	const register_list_t reg_list;
	const reg_idx rm;
};

struct standard_register_list_rn {
	standard_register_list_rn(halfword field)
			: register_list(binops::read_uint(field, 0, 8))
			, rn(binops::read_uint(field, 8, 3))
	{}

	bool is_set(reg_idx reg) const {
		return binops::get_bit(register_list, reg);
	}

	size_t pop_count() const {
		return __builtin_popcount(register_list);
	}

	const register_list_t register_list;
	const reg_idx rn;
};

struct standard_imm8_cond {

	standard_imm8_cond(halfword field)
		: imm8(binops::read_uint(field, 0, 8))
		, cond(binops::read_uint(field, 8, 4))
	{}

	const imm8_t imm8;
	const uint8_t cond;
};

struct nop {

};

struct adc : public standard_rdn_rm {
	using standard_rdn_rm::standard_rdn_rm;

};

struct add_imm : public standard_rd_rn_imm3 {
	using standard_rd_rn_imm3::standard_rd_rn_imm3;

};

struct add_imm_t2 : public standard_imm8_rdn {
	// T2 version of ADD immediate
	using standard_imm8_rdn::standard_imm8_rdn;

};

struct add_reg : public standard_rd_rn_rm {
	using standard_rd_rn_rm::standard_rd_rn_rm;

};

// TODO: should herit fromstandard_rdn_rm_dN
struct add_highreg : public standard_rdn_rm_dm {
	// Note: encoding t2 of add reg
	using standard_rdn_rm_dm::standard_rdn_rm_dm;
};

struct add_sp_imm : public standard_imm8_rd {
	using standard_imm8_rd::standard_imm8_rd;

	word imm32() const {
		return imm8 << 2;
	}
};

struct add_sp_imm_t2 : public standard_imm7 {
	using standard_imm7::standard_imm7;
	// encoding t2
	word imm32() const {
		return imm7 << 2;
	}

};

struct adr : public standard_imm8_rd {
	using standard_imm8_rd::standard_imm8_rd;

	word imm32() const {
		return imm8 << 2;
	}
};

struct and_reg : public standard_rdn_rm {
	using standard_rdn_rm::standard_rdn_rm;
};

struct asr_imm : public standard_rd_rm_imm5 {
	using standard_rd_rm_imm5::standard_rd_rm_imm5;
	imm5_t shift_offset() const {
		if(0 == imm5) return 32;
		return imm5;
	}
};

struct asr_reg : public standard_rdn_rm {
	using standard_rdn_rm::standard_rdn_rm;
};

static
const char* condition_string(uint8_t condition) {
	static const char* cond_names = "eqnecsccmiplvsvchilsgeltgtle";
	return cond_names + (condition * 2U);
}

struct branch : public standard_imm8_cond {
	using standard_imm8_cond::standard_imm8_cond;

	word imm32() const {
		return imm8 << 1;
	}

	int32_t offset() const {
		uint32_t signed_offset = imm32();
		return binops::sign<int32_t>(signed_offset, 9);
	}
};

struct unconditional_branch : public standard_imm11 {
	using standard_imm11::standard_imm11;
	// t2 encoding of B
	word imm32() const {
		return imm11 << 1;
	}

	int32_t offset() const {
		uint32_t signed_offset = imm32();
		return binops::sign<int32_t>(signed_offset, 12);
	}
};

struct bic_reg : public standard_rdn_rm {
	using standard_rdn_rm::standard_rdn_rm;
};

struct bl_imm {

	bl_imm(const instruction_pair& instr)
			: j1(instr.second.bit(13))
			, j2(instr.second.bit(11))
			, s(instr.first.bit(10))
			, imm10(instr.first.uint(0, 10))
			, imm11(instr.second.uint(0, 11))
	{}

	int32_t offset() const {
		const bool i1 = !((!j1) != (!s)); // logical xor i1 = not(j1 xor s)
		const bool i2 = !((!j2) != (!s)); // logical xor i2 = not(j2 xor s)
		uint32_t uint25_offset =
				(imm11 | (imm10 << 11) | (i2 << 21) | (i1 << 22) | (s << 23)) << 1;
		return binops::sign<int32_t>(uint25_offset, 25);
	}

	const bool j1;
	const bool j2;
	const bool s;
	const halfword imm10;
	const halfword imm11;
};

struct bx : public standard_rm {
	using standard_rm::standard_rm;
};

struct blx : public standard_rm {
	using standard_rm::standard_rm;
};

struct cmn_reg : public standard_rn_rm {
	using standard_rn_rm::standard_rn_rm;
};

struct cmp_imm : public standard_imm8_rn {
	using standard_imm8_rn::standard_imm8_rn;
};

struct cmp_reg : public standard_rn_rm {
	using standard_rn_rm::standard_rn_rm;
};

struct eor_reg : public standard_rdn_rm {
	using standard_rdn_rm::standard_rdn_rm;
};

static
std::string reglist(uint8_t mask) {
	static const char* reg_names[] = {
		"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12",
		"sp", "lr", "pc"
	};
	std::stringstream ss;
	ss << "{";
	for(reg_idx i = 0; i < 16; i++) {
		if (mask & (1 << i)) {
			ss << reg_names[i] << " ";
		}
	}
	ss << "}";
	return ss.str();
}

struct ldm : public standard_register_list_rn {
	using standard_register_list_rn::standard_register_list_rn;
};

struct ldr_imm : public standard_rt_rn_imm5 {
	using standard_rt_rn_imm5::standard_rt_rn_imm5;

	uint32_t imm32() const {
		return imm5 << 2;
	}
};


struct ldr_sp_imm : public standard_imm8_rt {
	using standard_imm8_rt::standard_imm8_rt;
	// encoding t2 of ldr (imm)
	uint32_t imm32() const {
		return imm8 << 2;
	}
};

struct ldr_literal : public standard_imm8_rt {
	using standard_imm8_rt::standard_imm8_rt;

	uint32_t imm32() const {
		return imm8 << 2;
	}
};

struct ldr_reg : public standard_rt_rn_rm {
	using standard_rt_rn_rm::standard_rt_rn_rm;
};


struct lsl_imm : public standard_rd_rm_imm5 {
	using standard_rd_rm_imm5::standard_rd_rm_imm5;
	imm5_t shift_offset() const {
		return imm5;
	}
};

struct lsr_imm : public standard_rd_rm_imm5 {
	using standard_rd_rm_imm5::standard_rd_rm_imm5;
	imm5_t shift_offset() const {
		if(0 == imm5) return 32;
		return imm5;
	}
};
struct subs_reg : public standard_rd_rn_rm {
	using standard_rd_rn_rm::standard_rd_rn_rm;
};


struct subs_imm : public standard_rd_rn_imm3 {
	using standard_rd_rn_imm3::standard_rd_rn_imm3;
};

struct mov_imm : public standard_imm8_rd {
	using standard_imm8_rd::standard_imm8_rd;
};

struct subs_imm8 : public standard_imm8_rdn {
	// T2 version of SUB immediate
	using standard_imm8_rdn::standard_imm8_rdn;
};

struct lsl_reg : public standard_rdn_rm {
	using standard_rdn_rm::standard_rdn_rm;
};

struct lsr_reg : public standard_rdn_rm {
	using standard_rdn_rm::standard_rdn_rm;
};

struct sbc : public standard_rdn_rm {
	using standard_rdn_rm::standard_rdn_rm;
};

struct ror_reg : public standard_rdn_rm {
	using standard_rdn_rm::standard_rdn_rm;
};

struct tst_reg : public standard_rn_rm {
	using standard_rn_rm::standard_rn_rm;
};

struct rsb_imm : public standard_rd_rn {
	using standard_rd_rn::standard_rd_rn;
};

struct orr_reg : public standard_rdn_rm {
	using standard_rdn_rm::standard_rdn_rm;
};

struct mul_reg : public standard_rdm_rn {
	using standard_rdm_rn::standard_rdm_rn;
};

struct not_reg : public standard_rd_rm {
	using standard_rd_rm::standard_rd_rm;
};

struct cmp_highreg : public standard_rn_rm_dm {
	using standard_rn_rm_dm::standard_rn_rm_dm;
};

struct mov_highreg : public standard_rd_rm_d {
	// encoding t1 of mov register
	using standard_rd_rm_d::standard_rd_rm_d;
};

struct movs: public standard_rd_rm {
	// encoding t2 of mov register
	// lsl imm with imm = 0 redirects here
	using standard_rd_rm::standard_rd_rm;
};

struct str_reg : public standard_rt_rn_rm {
	using standard_rt_rn_rm::standard_rt_rn_rm;
};

struct strh_reg : public standard_rt_rn_rm {
	using standard_rt_rn_rm::standard_rt_rn_rm;
};

struct strb_reg : public standard_rt_rn_rm {
	using standard_rt_rn_rm::standard_rt_rn_rm;
};

struct ldrsb_reg : public standard_rt_rn_rm {
	using standard_rt_rn_rm::standard_rt_rn_rm;
};

struct ldrh_reg : public standard_rt_rn_rm {
	using standard_rt_rn_rm::standard_rt_rn_rm;
};

struct ldrb_reg : public standard_rt_rn_rm {
	using standard_rt_rn_rm::standard_rt_rn_rm;
};

struct ldrsh_reg : public standard_rt_rn_rm {
	using standard_rt_rn_rm::standard_rt_rn_rm;
};

struct str_imm : public standard_rt_rn_imm5 {
	using standard_rt_rn_imm5::standard_rt_rn_imm5;
	// encoding t1
	uint32_t imm32() const {
		return imm5 << 2;
	}
};

struct strb_imm : public standard_rt_rn_imm5 {
	using standard_rt_rn_imm5::standard_rt_rn_imm5;
};

struct ldrb_imm : public standard_rt_rn_imm5 {
	using standard_rt_rn_imm5::standard_rt_rn_imm5;
};

struct strh_imm : public standard_rt_rn_imm5 {
	using standard_rt_rn_imm5::standard_rt_rn_imm5;
	uint32_t imm32() const {
		return imm5 << 1;
	}
};

struct ldrh_imm : public standard_rt_rn_imm5 {
	using standard_rt_rn_imm5::standard_rt_rn_imm5;
	uint32_t imm32() const {
		return imm5 << 1;
	}
};

struct str_sp_imm : public standard_imm8_rt {
	// T2 encoding of str (immediate)
	using standard_imm8_rt::standard_imm8_rt;

	uint32_t imm32() const {
		return imm8 << 2;
	}
};

struct sub_sp_imm : public standard_imm7 {
	using standard_imm7::standard_imm7;

	uint32_t imm32() const {
		return imm7 << 2;
	}
};


struct sxth : public standard_rd_rm {
	using standard_rd_rm::standard_rd_rm;
};

struct sxtb : public standard_rd_rm {
	using standard_rd_rm::standard_rd_rm;
};

struct uxth : public standard_rd_rm {
	using standard_rd_rm::standard_rd_rm;
};

struct uxtb : public standard_rd_rm {
	using standard_rd_rm::standard_rd_rm;
};

struct push : public standard_push_register_list {
	using standard_push_register_list::standard_push_register_list;
};

struct rev_word : public standard_rd_rm {
	using standard_rd_rm::standard_rd_rm;
};

struct rev16 : public standard_rd_rm {
	using standard_rd_rm::standard_rd_rm;
};

struct revsh : public standard_rd_rm {
	using standard_rd_rm::standard_rd_rm;
};

struct pop : public standard_pop_register_list {
	using standard_pop_register_list::standard_pop_register_list;
};

struct stm : public standard_register_list_rn {
	using standard_register_list_rn::standard_register_list_rn;
};

struct svc : public standard_imm8 {
	using standard_imm8::standard_imm8;
};

struct special_reg_instr {
	enum class SpecialRegister : imm8_t {
		APSR 	= 0b00000000,
		IAPSR 	= 0b00000001,
		EAPSR 	= 0b00000010,
		XPSR 	= 0b00000011,
		IPSR 	= 0b00000101,
		EPSR 	= 0b00000110,
		IEPSR 	= 0b00000111,
		MSP 	= 0b00001000,
		PSP 	= 0b00001001,
		PRIMASK	= 0b00010000,
		CONTROL	= 0b00010100,
	};
};

struct msr : special_reg_instr{
	msr(const instruction_pair& instr)
		: rn(instr.first.uint(0, 4))
		, sysn((SpecialRegister)instr.second.uint(0, 8)) {
	}
	const reg_idx rn;
	const SpecialRegister sysn;
};

struct mrs : special_reg_instr{
	mrs(const instruction_pair& instr)
		: rd(instr.second.uint(8, 4))
		, sysn(instr.second.uint(0, 8)) {
	}
	const reg_idx rd;
	const byte sysn;
};


struct udf {
	udf(const halfword& instruction)
		: imm32(instruction.uint(0, 8)) {
	}
	const uint32_t imm32;
};

struct udfw {
	udfw(const instruction_pair& instr)
		: imm32(
			(instr.first.uint(0, 4) << 12) |
			(instr.second.uint(0, 12))
		){
	}
	const uint32_t imm32;
};

struct cps {
	cps(halfword instruction)
	 : im(instruction.bit(4)){}
	const bool im;
};

struct bkpt : standard_imm8 {
	using standard_imm8::standard_imm8;
};
#endif //THUMBEMU_INSTRUCTIONS_HPP
