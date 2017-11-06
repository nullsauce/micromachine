#ifndef MICROMACHINE_INSTRUCTION_PAIR_HPP
#define MICROMACHINE_INSTRUCTION_PAIR_HPP

#include "types.hpp"

struct instruction_pair {
	instruction_pair(halfword a, halfword b)
		: first(a)
		, second(b) {}
	const halfword first;
	const halfword second;
};
#endif //MICROMACHINE_INSTRUCTION_PAIR_HPP
