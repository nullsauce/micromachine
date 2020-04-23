#pragma once

#include "../../types/bits.hpp"
#include "../../types/types.hpp"
#include "xpsr_reg.hpp"

namespace micromachine::system {

struct epsr_reg : public xpsr_reg {

	using xpsr_reg::flag_bit_ref;
	using xpsr_reg::flag_bit_const_ref;

	static const size_t FLAG_THUMB = 24;
	static const size_t STACK_ALIGNMENT = 9;

	using xpsr_reg::xpsr_reg;

	// when querying epsr bits, we consider only the thumb bit
	using epsr_bits = bits<FLAG_THUMB>;

	using stack_align_bit = bits<9>;

	flag_bit_const_ref<FLAG_THUMB> thumb_flag() const {
		return flag_bit_const_ref<FLAG_THUMB>(_xpsr);
	}

	flag_bit_ref<FLAG_THUMB> thumb_flag() {
		return flag_bit_ref<FLAG_THUMB>(_xpsr);
	}

	void set_thumb_bit(bool set) {
		bits<24>::of(_xpsr) = set;
	}

	bool stack_alignment() const {
		return bit(STACK_ALIGNMENT);
	}

	void reset() {
		set_thumb_bit(true);
	}
};

} // namespace micromachine::system
