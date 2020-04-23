#pragma once

#include "helpers/binops.hpp"
#include "types/types.hpp"

namespace micromachine::system {

/**
 * Used as a proxy for APSR IPSR AND ESPR implementeations
 */
class xpsr_reg {
public:
	xpsr_reg(uint32_t& xpsr)
		: _xpsr(xpsr) {}

	template <size_t flag>
	using flag_bit_ref = typename bits<flag>::template integer_slice<uint32_t>;

	template <size_t flag>
	using flag_bit_const_ref = typename bits<flag>::template const_integer_slice<uint32_t>;

protected:
	void write_bit(size_t offset, bool value) {
		binops::write_bit(_xpsr, offset, value);
	}

	bool bit(size_t offset) const {
		return binops::get_bit(_xpsr, offset);
	}

	uint32_t& _xpsr;
};

} // namespace micromachine::system
