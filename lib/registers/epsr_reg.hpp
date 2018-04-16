#ifndef MICROMACHINE_EPSR_REGSITER_HPP
#define MICROMACHINE_EPSR_REGSITER_HPP

#include "types.hpp"
#include "xpsr_reg.hpp"
#include "exception_type.hpp"

struct epsr_reg : public xpsr_reg {

	static const size_t THUMB_BIT = 24;
	static const size_t STACK_ALIGNMENT = 9;

	using xpsr_reg::xpsr_reg;
	
	bool thumb_bit_set() const {
		bool set = bit(THUMB_BIT);
		return set;
	}

	void set_thumb_bit(bool set) {
		write_bit(THUMB_BIT, set);
	}

	bool stack_alignment() const {
		return bit(STACK_ALIGNMENT);
	}
};


#endif //MICROMACHINE_EPSR_REGSITER_HPP
