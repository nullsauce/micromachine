/* Copyright (C) The Micromachine project - 2016 - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of The Micromachine project.

*/

#ifndef MICROMACHINE_IPSR_REGSITER_HPP
#define MICROMACHINE_IPSR_REGSITER_HPP

#include "types.hpp"
#include "xpsr_reg.hpp"
#include "exception.hpp"

struct ipsr_reg : public xpsr_reg {

	using xpsr_reg::xpsr_reg;

	void set_exception(exception e) {
		//write_bits(0, 0, static_cast<word>(e));
	}

	uint8_t exception() const {
		return _xpsr.uint(0, 8);
	}
};


#endif //MICROMACHINE_IPSR_REGSITER_HPP
