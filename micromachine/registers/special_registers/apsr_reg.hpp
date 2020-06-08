//

//

#pragma once

#include "types/bits.hpp"
#include "xpsr_reg.hpp"

namespace micromachine::system {

struct apsr_reg : public xpsr_reg {

	using negative_flag_bit = bits<31>;
	using zero_flag_bit = bits<30>;
	using carry_flag_bit = bits<29>;
	using overflow_flag_bit = bits<28>;
	using all_flags_bits = bits<28, 4>;

	using xpsr_reg::xpsr_reg;

	auto negative_flag() {
		return negative_flag_bit::of(_xpsr);
	}

	auto negative_flag() const {
		return negative_flag_bit::of(_xpsr);
	}

	auto zero_flag() {
		return zero_flag_bit::of(_xpsr);
	}

	auto zero_flag() const {
		return zero_flag_bit::of(_xpsr);
	}

	auto carry_flag() {
		return carry_flag_bit::of(_xpsr);
	}

	auto carry_flag() const {
		return carry_flag_bit::of(_xpsr);
	}

	auto overflow_flag() {
		return overflow_flag_bit::of(_xpsr);
	}

	auto overflow_flag() const {
		return overflow_flag_bit::of(_xpsr);
	}

	auto flags() {
		return all_flags_bits::integer_slice<uint32_t>(_xpsr);
	}

	auto flags() const {
		return all_flags_bits::const_integer_slice<uint32_t>(_xpsr);
	}

	void reset() {
		all_flags_bits::of(_xpsr).clear();
	}

	void apply_zero(const uint32_t& val) {
		zero_flag() = (0U == val);
	}

	void apply_neg(const uint32_t& val) {
		negative_flag() = bits<31>::of(val);
	}

	bool is_eq() const {
		return zero_flag();
	}

	bool is_ne() const {
		return !is_eq();
	}

	bool is_cs() const {
		return carry_flag();
	}

	bool is_cc() const {
		return !is_cs();
	}

	bool is_mi() const {
		return negative_flag();
	}

	bool is_pl() const {
		return !is_mi();
	}

	bool is_vs() const {
		return overflow_flag();
	}

	bool is_vc() const {
		return !is_vs();
	}

	bool is_hi() const {
		return carry_flag() && !zero_flag();
	}

	bool is_ls() const {
		return !is_hi();
	}

	bool is_ge() const {
		return negative_flag() == overflow_flag();
	}

	bool is_lt() const {
		return !is_ge();
	}

	bool is_gt() const {
		return (negative_flag() == overflow_flag()) && !zero_flag();
	}

	bool is_le() const {
		return !is_gt();
	}

	bool condition_passed(uint8_t condition) {
		switch(condition) {
			case 0b0000:
				return is_eq();
			case 0b0001:
				return is_ne();
			case 0b0010:
				return is_cs();
			case 0b0011:
				return is_cc();
			case 0b0100:
				return is_mi();
			case 0b0101:
				return is_pl();
			case 0b0110:
				return is_vs();
			case 0b0111:
				return is_vc();
			case 0b1000:
				return is_hi();
			case 0b1001:
				return is_ls();
			case 0b1010:
				return is_ge();
			case 0b1011:
				return is_lt();
			case 0b1100:
				return is_gt();
			case 0b1101:
				return is_le();
			default:
				return true;
		}
	}
};

} // namespace micromachine::system
