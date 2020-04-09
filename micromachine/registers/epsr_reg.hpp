#ifndef MICROMACHINE_EPSR_REGSITER_HPP
#define MICROMACHINE_EPSR_REGSITER_HPP

#include "types.hpp"
#include "xpsr_reg.hpp"

struct epsr_reg : public xpsr_reg {

	static const size_t THUMB_BIT = 24;
	static const size_t STACK_ALIGNMENT = 9;

	using xpsr_reg::xpsr_reg;

	using thumb_bit = bits<24>;
	using stack_align_bit = bits<9>;

	bool thumb_bit_set() const {
		return bits<24>::of(_xpsr);
	}

	void set_thumb_bit(bool set) {
		bits<24>::of(_xpsr) = set;
	}

	bool stack_alignment() const {
		return bit(STACK_ALIGNMENT);
	}
};


#endif //MICROMACHINE_EPSR_REGSITER_HPP