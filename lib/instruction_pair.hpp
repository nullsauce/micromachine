#ifndef MICROMACHINE_INSTRUCTION_PAIR_HPP
#define MICROMACHINE_INSTRUCTION_PAIR_HPP

#include "types.hpp"
#include "bits.hpp"

static
bool is_wide_thumb_encoding(const uint16_t& instruction) {
	return 0b111 == bits<13,3>::of(instruction) &&
	       0b00 !=  bits<11,2>::of(instruction);
}

// TODO: make natively 32 bits
struct instruction_pair {
	instruction_pair(uint16_t a, uint16_t b)
	    : first(a)
	    , second(b) {}
	uint16_t first;
	uint16_t second;

	instruction_pair()
	    : first(0)
	    , second(0) {}

	instruction_pair(uint32_t word)
	    : first(bits<0,16>::of(word))
	    , second(bits<16,16>::of(word)) {}


	bool is_wide() const {
		return is_wide_intruction(first);
	}

	static bool is_wide_intruction(uint16_t first) {
		return is_wide_thumb_encoding(first);
	}

	const uint32_t size() const {
		return sizeof(uint16_t) + (is_wide() * sizeof(uint16_t));
	}

};
#endif //MICROMACHINE_INSTRUCTION_PAIR_HPP
