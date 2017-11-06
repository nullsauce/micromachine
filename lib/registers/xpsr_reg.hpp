#ifndef MICROMACHINE_XPSR_REG_HPP
#define MICROMACHINE_XPSR_REG_HPP

#include "types.hpp"

/**
 * Used as a proxy for APSR IPSR AND ESPR implementeations
 */
class xpsr_reg {

public:

	xpsr_reg(word& xpsr) : _xpsr(xpsr) {}

protected:

	void write_bit(size_t offset, bool value) {
		_xpsr.write_bit(offset, value);
	}

	bool bit(size_t offset) const {
		return _xpsr.bit(offset);
	}

	word& _xpsr;
};

#endif //MICROMACHINE_XPSR_REG_HPP
