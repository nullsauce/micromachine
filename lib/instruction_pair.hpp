#ifndef MICROMACHINE_INSTRUCTION_PAIR_HPP
#define MICROMACHINE_INSTRUCTION_PAIR_HPP

#include "types.hpp"
#include "bits.hpp"

static
bool is_wide_thumb_encoding(const halfword& instruction) {
	return 0b111 == bits<13,3>::of(instruction) &&
	       0b00 !=  bits<11,2>::of(instruction);
}

// TODO: make natively 32 bits
struct instruction_pair {
	instruction_pair(halfword a, halfword b)
	    : first(a)
	    , second(b) {}
	halfword first;
	halfword second;

	instruction_pair()
	    : first(0)
	    , second(0) {}

	instruction_pair(word word)
	    : first(bits<0,16>::of(word))
	    , second(bits<16,16>::of(word)) {}


	bool is_wide() const {
		return is_wide_intruction(first);
	}

	static bool is_wide_intruction(halfword first) {
		return is_wide_thumb_encoding(first);
	}

	const word size() const {
		return sizeof(halfword) + (is_wide() * sizeof(halfword));
	}

};
#endif //MICROMACHINE_INSTRUCTION_PAIR_HPP
