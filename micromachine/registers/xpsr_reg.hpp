#ifndef MICROMACHINE_XPSR_REG_HPP
#define MICROMACHINE_XPSR_REG_HPP

#include "types.hpp"

/**
 * Used as a proxy for APSR IPSR AND ESPR implementeations
 */
class xpsr_reg {

public:

	xpsr_reg(uint32_t& xpsr) : _xpsr(xpsr) {}

protected:

	void write_bit(size_t offset, bool value) {
		binops::write_bit(_xpsr, offset, value);
	}

	bool bit(size_t offset) const {
		return binops::get_bit(_xpsr, offset);
	}

	uint32_t& _xpsr;
};

#endif //MICROMACHINE_XPSR_REG_HPP
