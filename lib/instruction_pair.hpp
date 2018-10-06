#ifndef MICROMACHINE_INSTRUCTION_PAIR_HPP
#define MICROMACHINE_INSTRUCTION_PAIR_HPP

#include "types.hpp"

static
bool is_wide_thumb_encoding(const halfword& instruction) {
	return 0b111 == instruction.uint(13, 3) &&
	       0b00 !=  instruction.uint(11, 2);
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
	    : first(word.bits<16,16>())
	    , second(word.bits<0,16>()) {}


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
