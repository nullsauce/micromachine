#ifndef MICROMACHINE_INSTRUCTION_PAIR_HPP
#define MICROMACHINE_INSTRUCTION_PAIR_HPP

#include "types.hpp"
#include "bits.hpp"

static
bool is_wide_thumb_encoding(const uint16_t& instruction) {
	return 0b111 == bits<13,3>::of(instruction) &&
	       0b00 !=  bits<11,2>::of(instruction);
}

struct instruction_pair {

	using first_intruction_bits = bits<0, 16>;
	using second_intruction_bits = bits<16, 16>;

	instruction_pair()
		: _word(0){}

	instruction_pair(uint32_t word)
		: _word(word) {}

	instruction_pair(uint16_t a, uint16_t b){
		first_intruction_bits::of(_word) = a;
		second_intruction_bits::of(_word) = b;

	}

	first_intruction_bits::integer_slice<uint32_t> first() {
		return first_intruction_bits::of(_word);
	}

	first_intruction_bits::const_integer_slice<uint32_t> first() const {
		return first_intruction_bits::of(_word);
	}

	second_intruction_bits::integer_slice<uint32_t> second() {
		return second_intruction_bits::of(_word);
	}

	second_intruction_bits::const_integer_slice<uint32_t> second() const {
		return second_intruction_bits::of(_word);
	}

	bool is_wide() const {
		return is_wide_intruction(first());
	}

	static bool is_wide_intruction(uint16_t first) {
		return is_wide_thumb_encoding(first);
	}

	const uint32_t size() const {
		return sizeof(uint16_t) + (is_wide() * sizeof(uint16_t));
	}

private:
	uint32_t _word;

};
#endif //MICROMACHINE_INSTRUCTION_PAIR_HPP
