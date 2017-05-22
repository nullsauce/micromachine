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
		// If n is 32 or more, then all the bits in the result are cleared to 0.
		// If n is 33 or more and the carry flag is updated, it is updated to 0.
		//printf("Warning: lsl_c offset is > 32 : %d\n", offset);
		//offset = offset & 31;
		const size_t binsize = binops::binsize(value);


		precond(offset > 0, "shift offset must be greater than 0");


		bool carry = false;

		if(offset <= 32) {
			carry = binops::get_bit(value, binsize - offset);
		}

		if(offset >= 32) {
			value = 0;
		} else {
			value = value << offset;
		}
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

		//If n is 32 or more, then all the bits in the result are cleared to 0.
		//If n is 33 or more and the carry flag is updated, it is updated to 0.

		//const size_t binsize = binops::binsize(value);
		//const bool msb = binops::get_bit(value, binsize - 1U);

		bool carry = false;
		if(offset <= 32) {
			carry = binops::get_bit(value, offset - 1);
		}

		value = (offset >= 32) ? 0 : value >> offset;

		//if(offset >= 33 && (carry != carry_in)) {
		//	carry = 0;
		//}

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

		bool carry = offset >= 32 ? msb : binops::get_bit(value, offset - 1U);

		// Notes:
		// If offset is 32 or more, then all the bits in the result are set to
		// the value of bit[31] of the value.
		// If offset is 32 or more and the carry flag is updated, it is updated
		// to the value of bit[31] of the value

		if(offset >= 32) {
			// all bits set to value's msb
			value = msb * binops::make_mask<uint32_t>(binsize);

			// carry flag was updated, force carry to value's msb
			if(carry_in != carry) {
				carry = msb;
			}

		} else {
			const u_type leftmost_copy = msb * (binops::make_mask<uint32_t>(offset) << (binsize - offset));
			value = value >> offset;
			value = value | leftmost_copy;
		}





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

		const uint8_t offsetLower5bits = binops::read_uint(offset, 0, 5);
		const bool msb = binops::get_bit(value, binsize - 1U);

		bool carry = msb;
		if(offsetLower5bits) {
			carry = value.bit(offsetLower5bits - 1);
		}

		value = ((value >> offset) | (value << (binsize - offset)));

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
