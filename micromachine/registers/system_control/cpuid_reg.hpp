/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#pragma once

#include "registers/word_reg.hpp"
#include "types.hpp"

namespace micromachine::system {

class cpuid_reg	: public memory_mapped_reg {
public:
	static constexpr uint32_t CPUID = 0xE000ED00;

private:
	static constexpr uint8_t _value[4] = {'M', 'M', 0b01111100, 'a'};

	void set(uint32_t word) override {
		// writes to this register are ignored
	}

	uint32_t get() const override {
		return *((uint32_t*)_value);
	}

};

} // namespace micromachine::system
