#pragma once

#include "types/bits.hpp"
#include "types/types.hpp"
#include "xpsr_reg.hpp"

namespace micromachine::system {

struct epsr_reg : public xpsr_reg {


	using thumb_flag_bit = bits<24>;
	using stack_align_flag_bit = bits<9>;

	using xpsr_reg::xpsr_reg;

	// Used when building bitmask for querying epsr bits. Only the thumb flag is considered.
	using epsr_bits = thumb_flag_bit;

	auto thumb_flag() const {
		return thumb_flag_bit::of(_xpsr);
	}

	auto thumb_flag() {
		return thumb_flag_bit::of(_xpsr);
	}

	auto stack_align_flag() const {
		return stack_align_flag_bit::of(_xpsr);
	}

	void reset() {
		thumb_flag() = true;
	}
};

} // namespace micromachine::system
