/* Copyright (C) MindMaze SA - 2016 - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of MindMaze SA.

*/

#ifndef MICROMACHINE_EPSR_REGSITER_HPP
#define MICROMACHINE_EPSR_REGSITER_HPP

#include "types.hpp"
#include "xpsr_reg.hpp"
#include "exception.hpp"

struct epsr_reg : public xpsr_reg {

	static const size_t THUMB_BIT = 24;
	static const size_t STACK_ALIGNMENT = 9;

	using xpsr_reg::xpsr_reg;
	
	bool thumb_bit_set() const {
		return bit(THUMB_BIT);
	}

	void set_thumb_bit(bool set) {
		write_bit(THUMB_BIT, set);
	}

	bool stack_alignment() const {
		return bit(STACK_ALIGNMENT);
	}
};


#endif //MICROMACHINE_EPSR_REGSITER_HPP
