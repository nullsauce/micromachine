//
// Created by fla on 4/30/17.
//

#ifndef THUMBEMU_INSTRUCTIONS_HPP
#define THUMBEMU_INSTRUCTIONS_HPP

#include <cstdint>
#include <string>
#include "types.hpp"
#include "register.hpp"

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
		, rn(binops::read_uint(field, 6, 9))
	{}

	const reg_idx rd;
	const reg_idx rm;
	const reg_idx rn;
};

struct standard_rd_rm_imm3 {

	standard_rd_rm_imm3(halfword field)
			: rd  (binops::read_uint(field, 0, 3))
			, rm  (binops::read_uint(field, 3, 3))
			, imm3(binops::read_uint(field, 6, 9))
	{}

	const reg_idx rd;
	const reg_idx rm;
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
			: reg_list(binops::read_uint(field, 0, 8))
			, rn(binops::read_uint(field, 8, 3))
	{}

	bool is_set(reg_idx reg) const {
		return binops::get_bit(reg_list, reg);
	}

	size_t pop_count() const {
		return __builtin_popcount(reg_list);
	}

	const register_list_t reg_list;
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

struct asr_imm : public standard_rd_rm_imm5 {
	using standard_rd_rm_imm5::standard_rd_rm_imm5;
	imm5_t shift_offset() const {
		if(0 == imm5) return 32;
		return imm5;
	}
};

struct adds_reg : public standard_rd_rm_rn {
	using standard_rd_rm_rn::standard_rd_rm_rn;
};

struct subs_reg : public standard_rd_rm_rn {
	using standard_rd_rm_rn::standard_rd_rm_rn;
};

struct adds_imm : public standard_rd_rm_imm3 {
	using standard_rd_rm_imm3::standard_rd_rm_imm3;
};

struct subs_imm : public standard_rd_rm_imm3 {
	using standard_rd_rm_imm3::standard_rd_rm_imm3;
};

struct mov_imm : public standard_imm8_rd {
	using standard_imm8_rd::standard_imm8_rd;
};

struct cmp_imm : public standard_imm8_rn {
	using standard_imm8_rn::standard_imm8_rn;
};

struct adds_imm8 : public standard_imm8_rdn {
	// T2 version of ADD immediate
	using standard_imm8_rdn::standard_imm8_rdn;
};

struct subs_imm8 : public standard_imm8_rdn {
	// T2 version of SUB immediate
	using standard_imm8_rdn::standard_imm8_rdn;
};

struct and_reg : public standard_rdn_rm {
	using standard_rdn_rm::standard_rdn_rm;
};

struct xor_reg : public standard_rdn_rm {
	using standard_rdn_rm::standard_rdn_rm;
};

struct lsl_reg : public standard_rdn_rm {
	using standard_rdn_rm::standard_rdn_rm;
};

struct lsr_reg : public standard_rdn_rm {
	using standard_rdn_rm::standard_rdn_rm;
};

struct asr_reg : public standard_rdn_rm {
	using standard_rdn_rm::standard_rdn_rm;
};

struct add_c_reg : public standard_rdn_rm {
	using standard_rdn_rm::standard_rdn_rm;
};

struct sub_c_reg : public standard_rdn_rm {
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

struct cmp_reg : public standard_rn_rm {
	using standard_rn_rm::standard_rn_rm;
};

struct cmpn_reg : public standard_rn_rm {
	using standard_rn_rm::standard_rn_rm;
};

struct lor_reg : public standard_rdn_rm {
	using standard_rdn_rm::standard_rdn_rm;
};

struct mul_reg : public standard_rdm_rn {
	using standard_rdm_rn::standard_rdm_rn;
};

struct bic_reg : public standard_rdn_rm {
	using standard_rdn_rm::standard_rdn_rm;
};

struct not_reg : public standard_rd_rm {
	using standard_rd_rm::standard_rd_rm;
};

struct add_highreg : public standard_rdn_rm_dm {
	using standard_rdn_rm_dm::standard_rdn_rm_dm;
};

struct cmp_highreg : public standard_rn_rm_dm {
	using standard_rn_rm_dm::standard_rn_rm_dm;
};

struct mov_highreg : public standard_rd_rm_d {
	using standard_rd_rm_d::standard_rd_rm_d;
};

struct bx : public standard_rm {
	using standard_rm::standard_rm;
};

struct blx : public standard_rm {
	using standard_rm::standard_rm;
};

struct load_literal : public standard_imm8_rt {
	using standard_imm8_rt::standard_imm8_rt;
};

struct store_reg_word_reg : public standard_rt_rn_rm {
	using standard_rt_rn_rm::standard_rt_rn_rm;
};

struct store_reg_halfword_reg : public standard_rt_rn_rm {
	using standard_rt_rn_rm::standard_rt_rn_rm;
};

struct store_reg_byte_reg : public standard_rt_rn_rm {
	using standard_rt_rn_rm::standard_rt_rn_rm;
};

struct load_reg_sbyte_reg : public standard_rt_rn_rm {
	using standard_rt_rn_rm::standard_rt_rn_rm;
};

struct load_reg_word_reg : public standard_rt_rn_rm {
	using standard_rt_rn_rm::standard_rt_rn_rm;
};

struct load_reg_halfword_reg : public standard_rt_rn_rm {
	using standard_rt_rn_rm::standard_rt_rn_rm;
};

struct load_reg_byte_reg : public standard_rt_rn_rm {
	using standard_rt_rn_rm::standard_rt_rn_rm;
};

struct load_reg_shalfword_reg : public standard_rt_rn_rm {
	using standard_rt_rn_rm::standard_rt_rn_rm;
};

struct store_word_imm : public standard_rt_rn_imm5 {
	using standard_rt_rn_imm5::standard_rt_rn_imm5;
};

struct load_word_imm : public standard_rt_rn_imm5 {
	using standard_rt_rn_imm5::standard_rt_rn_imm5;
};

struct store_byte_imm : public standard_rt_rn_imm5 {
	using standard_rt_rn_imm5::standard_rt_rn_imm5;
};

struct load_byte_imm : public standard_rt_rn_imm5 {
	using standard_rt_rn_imm5::standard_rt_rn_imm5;
};

struct store_halfword_imm : public standard_rt_rn_imm5 {
	using standard_rt_rn_imm5::standard_rt_rn_imm5;
};

struct load_halfword_imm : public standard_rt_rn_imm5 {
	using standard_rt_rn_imm5::standard_rt_rn_imm5;
};

struct store_word_sp_imm : public standard_imm8_rt {
	using standard_imm8_rt::standard_imm8_rt;
};

struct load_word_sp_imm : public standard_imm8_rt {
	using standard_imm8_rt::standard_imm8_rt;
};

struct adr : public standard_imm8_rd {
	using standard_imm8_rd::standard_imm8_rd;
};

struct add_sp_imm : public standard_imm8_rd {
	using standard_imm8_rd::standard_imm8_rd;
};

struct add_sp_imm_t2 : public standard_imm7 {
	using standard_imm7::standard_imm7;
};

struct sub_sp_imm : public standard_imm7 {
	using standard_imm7::standard_imm7;
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

struct rev_packed_halfword : public standard_rd_rm {
	using standard_rd_rm::standard_rd_rm;
};

struct rev_packed_signed_halfword : public standard_rd_rm {
	using standard_rd_rm::standard_rd_rm;
};

struct pop : public standard_pop_register_list {
	using standard_pop_register_list::standard_pop_register_list;
};

struct branch : public standard_imm8_cond {
	using standard_imm8_cond::standard_imm8_cond;
};

struct stm : public standard_register_list_rn {
	using standard_register_list_rn::standard_register_list_rn;
};

struct ldm : public standard_register_list_rn {
	using standard_register_list_rn::standard_register_list_rn;
};

struct unconditional_branch : public standard_imm11 {
	using standard_imm11::standard_imm11;
};

struct svc : public standard_imm8 {
	using standard_imm8::standard_imm8;
};


struct bl_imm {

	bl_imm(halfword first, halfword second)
		: j1(second.bit(13))
		, j2(second.bit(11))
		, s(first.bit(10))
		, imm10(first.uint(0, 10))
		, imm11(second.uint(0, 11))
	{
		fprintf(stderr, "%s %s\n", first.to_string().c_str(), second.to_string().c_str());

	}


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

#endif //THUMBEMU_INSTRUCTIONS_HPP
