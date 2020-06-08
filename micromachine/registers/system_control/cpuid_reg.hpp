

#pragma once

#include "registers/standard_reg.hpp"
#include "types/types.hpp"

namespace micromachine::system {

class cpuid_reg : public standard_reg {
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
