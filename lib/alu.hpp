//
// Created by fla on 17-5-8.
//

#ifndef MICROMACHINE_ALU_HPP_HPP
#define MICROMACHINE_ALU_HPP_HPP

#include "types.hpp"

namespace alu {

	template <typename u_type>
	bool lsl_c(u_type& value, uint32_t offset, bool carry_in) {
		if(0 == offset) return carry_in;
		precond(offset > 0, "shift offset must be greater than 0");
		bool carry = binops::get_bit(value, offset);
		value = value << offset;
		return carry;
	}

	template <typename u_type>
	bool lsl(u_type& value, uint32_t offset) {
		return lsl_c(value, offset, false);
	}

	template <typename u_type>
	bool lsr_c(u_type& value, uint32_t offset, bool carry_in) {
		if(0 == offset) return carry_in;
		precond(offset > 0, "shift offset must be greater than 0");
		const size_t binsize = binops::binsize(value);
		bool carry = binops::get_bit(value, binsize - offset - 1);
		value = value >> offset;
		return carry;
	}

	template <typename u_type>
	bool lsr(u_type& value, uint32_t offset) {
		return lsr_c(value, offset, false);
	}

	template <typename u_type>
	bool asr_c(u_type& value, uint32_t offset, bool carry_in) {
		if(0 == offset) return carry_in;
		precond(offset > 0, "shift offset must be greater than 0");
		const size_t binsize = binops::binsize(value);
		const bool msb = binops::get_bit(value, binsize - 1U);
		const bool carry = binops::get_bit(value, offset - 1U);
		const u_type leftmost_copy = msb * (binops::make_mask<uint32_t>(offset) << (binsize - offset));
		value = value >> offset;
		value = value | leftmost_copy;
		return carry;
	}

	template <typename u_type>
	bool asr(u_type& value, uint32_t offset) {
		return asr_c(value, offset, false);
	}

	template <typename u_type>
	bool ror_c(u_type& value, uint32_t offset, bool carry_in) {
		if(0 == offset) return carry_in;
		const size_t binsize = binops::binsize(value);
		precond(offset <= binsize, "shift offset too large");
		value = ((value >> offset) | (value << (binsize - offset)));
		const bool carry = binops::get_bit(value, binsize - 1U);
		return carry;
	}

	template <typename u_type>
	bool ror(u_type& value, uint32_t offset) {
		return ror_c(value, offset, false);
	}

	static inline word add_with_carry(
			const word& a,
			const word& b,
			const bool& carry_in,
			bool& carry_out,
			bool& overflow_out) {

		const uint64_t bigval = (uint64_t)a + (uint64_t)b + (uint64_t)carry_in;
		const word lower32bits = binops::make_mask<word>(binops::binsize<word>());
		const word res = (word)(bigval & lower32bits);

		const word ab = a ^ b;
		const word ares = a ^ res;

		carry_out = (bigval >> 32) & 1U;
		overflow_out = !ab.sign_bit() && ares.sign_bit();

		return res;
	}

	static inline word add_with_carry(
			const word& a,
			const word& b,
			const bool& carry_in) {
		bool ignored_carry;
		bool ignored_overflow;
		return add_with_carry(a, b, carry_in, ignored_carry, ignored_overflow);
	}
}


#endif //MICROMACHINE_ALU_HPP_HPP
