#pragma once

#include "../../helpers/binops.hpp"
#include "../../types/types.hpp"

namespace micromachine::system {

/**
 * Used as a proxy for APSR IPSR AND ESPR implementeations
 */
class xpsr_reg {
public:
	xpsr_reg(uint32_t& xpsr)
		: _xpsr(xpsr) {}

protected:
	uint32_t& _xpsr;
};

} // namespace micromachine::system
