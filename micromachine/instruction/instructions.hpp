#pragma once

#include "helpers/binops.hpp"
#include "helpers/string_format.hpp"
#include "instruction_pair.hpp"
#include "registers/core_registers/core_registers.hpp"
#include "types/bits.hpp"
#include "types/types.hpp"
#include <cstdint>
#include <sstream>
#include <string>

namespace micromachine::system {

template <typename uint_type>
class generic_instruction {
public:
	template <typename T>
	using slice_of = typename T::template integer_slice<uint_type>;

	template <typename T>
	using const_slice_of = typename T::template const_integer_slice<uint_type>;

	generic_instruction(uint_type word)
		: _word(word) {}

	uint_type value() const {
		return _word;
	}

protected:
	uint_type _word;
};

/// Standard 1-field instruction layout
/// \tparam f0 offset (in bits) of the first field
/// \tparam f1 size (in bits) of the first field
template <typename uint_type, size_t f0, size_t f1>
struct standard_1_fields : public generic_instruction<uint_type> {
	using generic_instruction<uint_type>::generic_instruction;
	using field0_bits = bits<f0, f1>;
	using generic_instruction<uint_type>::_word;

	typename generic_instruction<uint_type>::template slice_of<field0_bits> field0() {
		return field0_bits::of(generic_instruction<uint_type>::_word);
	}

	typename generic_instruction<uint_type>::template const_slice_of<field0_bits> field0() const {
		return field0_bits::of(generic_instruction<uint_type>::_word);
	}
};

/// Standard 2-fields instruction layout
/// \tparam f0 offset (in bits) of the first field
/// \tparam f1 size (in bits) of the first field
/// \tparam f2 offset (in bits) of the second field
/// \tparam f3 size (in bits) of the second field
template <typename uint_type, size_t f0, size_t f1, size_t f2, size_t f3>
struct standard_2_fields : public standard_1_fields<uint_type, f0, f1> {
	using standard_1_fields<uint_type, f0, f1>::standard_1_fields;
	using field1_bits = bits<f2, f3>;

	typename generic_instruction<uint_type>::template slice_of<field1_bits> field1() {
		return field1_bits::of(generic_instruction<uint_type>::_word);
	}

	typename generic_instruction<uint_type>::template const_slice_of<field1_bits> field1() const {
		return field1_bits::of(generic_instruction<uint_type>::_word);
	}
};

/// Standard 3-fields instruction layout
/// \tparam f0 offset (in bits) of the first field
/// \tparam f1 size (in bits) of the first field
/// \tparam f2 offset (in bits) of the second field
/// \tparam f3 size (in bits) of the second field
/// \tparam f4 offset (in bits) of the third field
/// \tparam f5 size (in bits) of the third field
template <typename uint_type, size_t f0, size_t f1, size_t f2, size_t f3, size_t f4, size_t f5>
struct standard_3_fields : public standard_2_fields<uint_type, f0, f1, f2, f3> {
	using standard_2_fields<uint_type, f0, f1, f2, f3>::standard_2_fields;
	using field2_bits = bits<f4, f5>;

	typename generic_instruction<uint_type>::template slice_of<field2_bits> field2() {
		return field2_bits::of(generic_instruction<uint_type>::_word);
	}

	typename generic_instruction<uint_type>::template const_slice_of<field2_bits> field2() const {
		return field2_bits::of(generic_instruction<uint_type>::_word);
	}
};
/// Standard 3-fields instruction layout
/// \tparam f0 offset (in bits) of the first field
/// \tparam f1 size (in bits) of the first field
/// \tparam f2 offset (in bits) of the second field
/// \tparam f3 size (in bits) of the second field
/// \tparam f4 offset (in bits) of the third field
/// \tparam f5 size (in bits) of the third field
/// \tparam f6 offset (in bits) of the fourth field
/// \tparam f7 size (in bits) of the fourth field
template <typename uint_type, size_t f0, size_t f1, size_t f2, size_t f3, size_t f4, size_t f5, size_t f6, size_t f7>
struct standard_4_fields : public standard_3_fields<uint_type, f0, f1, f2, f3, f4, f5> {
	using standard_3_fields<uint_type, f0, f1, f2, f3, f4, f5>::standard_3_fields;
	using field3_bits = bits<f6, f7>;

	typename generic_instruction<uint_type>::template slice_of<field3_bits> field3() {
		return field3_bits::of(generic_instruction<uint_type>::_word);
	}

	typename generic_instruction<uint_type>::template const_slice_of<field3_bits> field3() const {
		return field3_bits::of(generic_instruction<uint_type>::_word);
	}
};

/// Helper macro to declare mutable and const accessor to a standard field
/// \param name the name of the field
/// \param field_no the number of the field (zero-based)
#define define_instruction_field(name, field_no)                                                                       \
	using name##_bits = field##field_no##_bits;                                                                        \
	slice_of<name##_bits> name() {                                                                                     \
		return field##field_no();                                                                                      \
	}                                                                                                                  \
	const_slice_of<name##_bits> name() const {                                                                         \
		return field##field_no();                                                                                      \
	}

// standard instruction binary layouts re-used across instructions
using layout_16_03_33_65 = standard_3_fields<uint16_t, 0, 3, 3, 3, 6, 5>;
using layout_16_03_33_63 = standard_3_fields<uint16_t, 0, 3, 3, 3, 6, 3>;
using layout_16_03_34_71 = standard_3_fields<uint16_t, 0, 3, 3, 4, 7, 1>;
using layout_16_08_83 = standard_2_fields<uint16_t, 0, 8, 8, 3>;
using layout_16_08_84 = standard_2_fields<uint16_t, 0, 8, 8, 4>;
using layout_16_03_33 = standard_2_fields<uint16_t, 0, 3, 3, 3>;
using layout_16_04_44 = standard_2_fields<uint16_t, 0, 4, 4, 4>;
using layout_16_34 = standard_1_fields<uint16_t, 3, 4>;
using layout_16_07 = standard_1_fields<uint16_t, 0, 7>;
using layout_16_08 = standard_1_fields<uint16_t, 0, 8>;
using layout_16_011 = standard_1_fields<uint16_t, 0, 11>;
using layout_16_04 = standard_1_fields<uint16_t, 0, 4>;

struct standard_rd_rm_imm5 : public layout_16_03_33_65 {
	using layout_16_03_33_65::layout_16_03_33_65;

	define_instruction_field(rd, 0);
	define_instruction_field(rm, 1);
	define_instruction_field(imm5, 2);
};

struct standard_rd_rn_rm : public layout_16_03_33_63 {
	using layout_16_03_33_63::layout_16_03_33_63;

	define_instruction_field(rd, 0);
	define_instruction_field(rn, 1);
	define_instruction_field(rm, 2);
};

struct standard_rd_rn_imm3 : public layout_16_03_33_63 {
	using layout_16_03_33_63::layout_16_03_33_63;

	define_instruction_field(rd, 0);
	define_instruction_field(rn, 1);
	define_instruction_field(imm3, 2);
};

struct standard_imm8_rd : public layout_16_08_83 {
	using layout_16_08_83::layout_16_08_83;

	define_instruction_field(imm8, 0);
	define_instruction_field(rd, 1);
};

struct standard_imm8_rn : public layout_16_08_83 {
	using layout_16_08_83::layout_16_08_83;

	define_instruction_field(imm8, 0);
	define_instruction_field(rn, 1);
};

struct standard_rdn_rm : public layout_16_03_33 {
	using layout_16_03_33::layout_16_03_33;

	define_instruction_field(rdn, 0);
	define_instruction_field(rm, 1);
};

struct standard_rdm_rn : public layout_16_03_33 {
	using layout_16_03_33::layout_16_03_33;

	define_instruction_field(rdm, 0);
	define_instruction_field(rn, 1);
};

struct standard_rdn_rm_dm : public layout_16_03_34_71 {
	using layout_16_03_34_71::layout_16_03_34_71;

	define_instruction_field(rdn, 0);
	define_instruction_field(rm, 1);
	define_instruction_field(dm, 2);

	// returns the possibly high register index
	reg_idx high_rd() const {
		// rdn can be a high register if dm is set
		return ((bool)dm() * 8U) + (reg_idx)rdn();
	}

	// returns the possibly high register index
	reg_idx high_rm() const {
		return rm();
	}
};

// This layout is similar to standard_rdn_rm_dm
// Except from the fact that the last field is named
// 'dn' instead of 'dm'.
struct standard_rdn_rm_dn : public layout_16_03_34_71 {
	using layout_16_03_34_71::layout_16_03_34_71;

	define_instruction_field(rdn, 0);
	define_instruction_field(rm, 1);
	define_instruction_field(dn, 2);

	// returns the possibly high register index
	reg_idx high_rd() const {
		// rdn can be a high register if dm is set
		return ((bool)dn() * 8U) + (reg_idx)rdn();
	}

	// returns the possibly high register index
	reg_idx high_rm() const {
		return rm();
	}
};

struct standard_rn_rm_dm : public layout_16_03_34_71 {
	using layout_16_03_34_71::layout_16_03_34_71;

	define_instruction_field(rn, 0);
	define_instruction_field(rm, 1);
	define_instruction_field(dm, 2);

	// returns the possibly high register index
	reg_idx high_rn() const {
		// rd can be a high register if dm is set
		return ((bool)dm() * 8) + (reg_idx)rn();
	}

	// returns the possibly high register index
	reg_idx high_rm() const {
		return rm();
	}
};

struct standard_rd_rm_d : public layout_16_03_34_71 {
	using layout_16_03_34_71::layout_16_03_34_71;

	define_instruction_field(rd, 0);
	define_instruction_field(rm, 1);
	define_instruction_field(d, 2);

	// returns the possibly high register index
	reg_idx high_rd() const {
		// rd can be a high register if dm is set
		return ((bool)d() * 8) + (reg_idx)rd();
	}

	// returns the possibly high register index
	reg_idx high_rm() const {
		return rm();
	}
};

struct standard_rn_rm : public layout_16_03_33 {
	using layout_16_03_33::layout_16_03_33;

	define_instruction_field(rn, 0);
	define_instruction_field(rm, 1);
};

struct standard_rd_rn : public layout_16_03_33 {
	using layout_16_03_33::layout_16_03_33;

	define_instruction_field(rd, 0);
	define_instruction_field(rn, 1);
};

struct standard_rm : public layout_16_34 {
	using layout_16_34::layout_16_34;

	define_instruction_field(rm, 0);
};

struct standard_rd_rm : public layout_16_03_33 {
	using layout_16_03_33::layout_16_03_33;

	define_instruction_field(rd, 0);
	define_instruction_field(rm, 1);
};

struct standard_imm8_rdn : public layout_16_08_83 {
	using layout_16_08_83::layout_16_08_83;

	define_instruction_field(imm8, 0);
	define_instruction_field(rdn, 1);
};

struct standard_rt_rn_rm : public layout_16_03_33_63 {
	using layout_16_03_33_63::layout_16_03_33_63;

	define_instruction_field(rt, 0);
	define_instruction_field(rn, 1);
	define_instruction_field(rm, 2);
};

struct standard_rt_rn_imm5 : public layout_16_03_33_65 {
	using layout_16_03_33_65::layout_16_03_33_65;

	define_instruction_field(rt, 0);
	define_instruction_field(rn, 1);
	define_instruction_field(imm5, 2);
};

struct standard_imm8_rt : public layout_16_08_83 {
	using layout_16_08_83::layout_16_08_83;

	define_instruction_field(imm8, 0);
	define_instruction_field(rt, 1);
};

struct standard_imm4 : public layout_16_04 {
	using layout_16_04::layout_16_04;

	define_instruction_field(imm4, 0);
};

struct standard_imm7 : public layout_16_07 {
	using layout_16_07::layout_16_07;

	define_instruction_field(imm7, 0);
};

struct standard_imm8 : public layout_16_08 {
	using layout_16_08::layout_16_08;

	define_instruction_field(imm8, 0);
};

struct standard_imm11 : public layout_16_011 {
	using layout_16_011::layout_16_011;

	define_instruction_field(imm11, 0);
};

struct standard_push_register_list : public generic_instruction<uint16_t> {

	standard_push_register_list(uint16_t field)
		: generic_instruction((binops::read_uint(field, 0, 8)) | (binops::get_bit(field, 8) << 14)) {}

	bool is_set(reg_idx reg) const {
		return binops::get_bit(_word, reg);
	}

	uint32_t pushed_registers_count() const {
		// Here popcount stands for population count. (nothing to do with push/pop)
		return __builtin_popcount(_word);
	}
};

struct standard_pop_register_list : public generic_instruction<uint16_t> {

	standard_pop_register_list(uint16_t field)
		: generic_instruction((binops::read_uint(field, 0, 8)) | (binops::get_bit(field, 8) << 15)) {}

	bool is_set(reg_idx reg) const {
		return binops::get_bit(_word, reg);
	}

	uint32_t pop_count() const {
		return __builtin_popcount(_word);
	}
};

struct standard_register_list_rn : public layout_16_08_83 {
	using layout_16_08_83::layout_16_08_83;

	define_instruction_field(register_list, 0);
	define_instruction_field(rn, 1);

	bool is_set(reg_idx reg) const {
		return binops::get_bit(register_list(), reg);
	}

	size_t pop_count() const {
		return __builtin_popcount(register_list());
	}
};

struct standard_imm8_cond : public layout_16_08_84 {
	using layout_16_08_84::layout_16_08_84;

	define_instruction_field(imm8, 0);
	define_instruction_field(cond, 1);
};

struct nop {};
struct wfe {};
struct yield {};
struct wfi {};

struct sev : public generic_instruction<uint16_t> {
	using generic_instruction<uint16_t>::generic_instruction;
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

struct add_highreg : public standard_rdn_rm_dn {
	// Note: encoding t2 of add reg
	using standard_rdn_rm_dn::standard_rdn_rm_dn;
};

struct add_sp_imm : public standard_imm8_rd {
	using standard_imm8_rd::standard_imm8_rd;

	uint32_t imm32() const {
		return imm8() << 2;
	}
};

struct add_sp_imm_t2 : public standard_imm7 {
	using standard_imm7::standard_imm7;
	// encoding t2
	uint32_t imm32() const {
		return imm7() << 2;
	}
};

struct adr : public standard_imm8_rd {
	using standard_imm8_rd::standard_imm8_rd;

	uint32_t imm32() const {
		return imm8() << 2;
	}
};

struct and_reg : public standard_rdn_rm {
	using standard_rdn_rm::standard_rdn_rm;
};

struct asr_imm : public standard_rd_rm_imm5 {
	using standard_rd_rm_imm5::standard_rd_rm_imm5;

	imm5_bits::const_integer_slice<uint16_t> shift_offset() const {
		return imm5();
	}
};

struct asr_reg : public standard_rdn_rm {
	using standard_rdn_rm::standard_rdn_rm;
};

static const char* condition_string(uint8_t condition) {
	static const char* cond_names = "eqnecsccmiplvsvchilsgeltgtle";
	return cond_names + (condition * 2U);
}

struct branch : public standard_imm8_cond {
	using standard_imm8_cond::standard_imm8_cond;

	uint32_t imm32() const {
		return imm8() << 1;
	}

	int32_t offset() const {
		uint32_t signed_offset = imm32();
		return binops::sign<int32_t>(signed_offset, 9);
	}
};

struct unconditional_branch : public standard_imm11 {
	using standard_imm11::standard_imm11;

	unconditional_branch()
		: standard_imm11(0b1110000000000000) {}

	// t2 encoding of B
	uint32_t imm32() const {
		return imm11() << 1;
	}

	int32_t offset() const {
		uint32_t signed_offset = imm32();
		return binops::sign<int32_t>(signed_offset, 12); // TODO: seems like 12 should be 11 ?
	}

	void set_offset(int16_t offset) {
		imm11() = (offset >> 1U);
	}
};

struct bic_reg : public standard_rdn_rm {
	using standard_rdn_rm::standard_rdn_rm;
};

struct bl_imm {

	bl_imm(const instruction_pair& instr)
		: j1(bits<13>::of(instr.second()))
		, j2(bits<11>::of(instr.second()))
		, s(bits<10>::of(instr.first()))
		, imm10(bits<0, 10>::of(instr.first()))
		, imm11(bits<0, 11>::of(instr.second())) {}

	int32_t offset() const {
		const bool i1 = !((!j1) != (!s)); // logical xor i1 = not(j1 xor s)
		const bool i2 = !((!j2) != (!s)); // logical xor i2 = not(j2 xor s)
		uint32_t uint25_offset = (imm11 | (imm10 << 11) | (i2 << 21) | (i1 << 22) | (s << 23)) << 1;
		return binops::sign<int32_t>(uint25_offset, 25);
	}

	const bool j1;
	const bool j2;
	const bool s;
	const uint16_t imm10;
	const uint16_t imm11;
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

static std::string reglist(uint8_t mask) {
	static const char* reg_names[] =
		{"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "sp", "lr", "pc"};
	std::stringstream ss;
	ss << "{";
	for(reg_idx i = 0; i < 16; i++) {
		if(mask & (1 << i)) {
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
		return imm5() << 2;
	}
};

struct ldr_sp_imm : public standard_imm8_rt {
	using standard_imm8_rt::standard_imm8_rt;
	// encoding t2 of ldr (imm)
	uint32_t imm32() const {
		return imm8() << 2;
	}
};

struct ldr_literal : public standard_imm8_rt {
	using standard_imm8_rt::standard_imm8_rt;

	uint32_t imm32() const {
		return imm8() << 2;
	}
};

struct ldr_reg : public standard_rt_rn_rm {
	using standard_rt_rn_rm::standard_rt_rn_rm;
};

struct lsl_imm : public standard_rd_rm_imm5 {
	using standard_rd_rm_imm5::standard_rd_rm_imm5;
	imm5_bits::const_integer_slice<uint16_t> shift_offset() const {
		return imm5();
	}
};

struct lsr_imm : public standard_rd_rm_imm5 {
	using standard_rd_rm_imm5::standard_rd_rm_imm5;
	imm5_bits::const_integer_slice<uint16_t> shift_offset() const {
		return imm5();
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

struct mvn : public standard_rd_rm {
	using standard_rd_rm::standard_rd_rm;
};

struct cmp_highreg : public standard_rn_rm_dm {
	using standard_rn_rm_dm::standard_rn_rm_dm;
};

struct mov_highreg : public standard_rd_rm_d {
	// encoding t1 of mov register
	using standard_rd_rm_d::standard_rd_rm_d;
};

struct movs : public standard_rd_rm {
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
		return imm5() << 2;
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
		return imm5() << 1;
	}
};

struct ldrh_imm : public standard_rt_rn_imm5 {
	using standard_rt_rn_imm5::standard_rt_rn_imm5;
	uint32_t imm32() const {
		return imm5() << 1;
	}
};

struct str_sp_imm : public standard_imm8_rt {
	// T2 encoding of str (immediate)
	using standard_imm8_rt::standard_imm8_rt;

	uint32_t imm32() const {
		return imm8() << 2;
	}
};

struct sub_sp_imm : public standard_imm7 {
	using standard_imm7::standard_imm7;

	uint32_t imm32() const {
		return imm7() << 2;
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
	enum class special_register : imm8_t {
		apsr = 0b00000000,
		iapsr = 0b00000001,
		eapsr = 0b00000010,
		xpsr = 0b00000011,
		ipsr = 0b00000101,
		epsr = 0b00000110,
		iepsr = 0b00000111,
		msp = 0b00001000,
		psp = 0b00001001,
		primask = 0b00010000,
		control = 0b00010100,
	};
};

struct msr : special_reg_instr {
	msr(const instruction_pair& instr)
		: rn(bits<0, 4>::of(instr.first()))
		, sysn((special_register)(uint8_t)bits<0, 8>::of(instr.second())) {}
	const reg_idx rn;
	const special_register sysn;
};

struct mrs : special_reg_instr {
	mrs(const instruction_pair& instr)
		: rd(bits<8, 4>::of(instr.second()))
		, sysn(bits<0, 8>::of(instr.second())) {}
	const reg_idx rd;
	const uint8_t sysn;
};

struct udf {
	udf(const uint16_t& instruction)
		: imm32(bits<0, 8>::of(instruction)) {}
	const uint32_t imm32;
};

struct udfw {
	udfw(const instruction_pair& instr)
		: imm32((bits<0, 4>::of(instr.first()) << 12) | (bits<0, 12>::of(instr.second()))) {}
	const uint32_t imm32;
};

struct cps {
	cps(uint16_t instruction)
		: im(bits<4>::of(instruction)) {}
	const bool im;
};

struct bkpt : standard_imm8 {
	using standard_imm8::standard_imm8;
};

struct hint : public layout_16_04_44 {
	using layout_16_04_44::layout_16_04_44;

	define_instruction_field(opa, 0);
	define_instruction_field(opb, 1);
};

struct dsb : public standard_imm4 {
	using standard_imm4::standard_imm4;
};

struct dmb : public standard_imm4 {
	using standard_imm4::standard_imm4;
};

struct isb : public standard_imm4 {
	using standard_imm4::standard_imm4;
};

} // namespace micromachine::system
