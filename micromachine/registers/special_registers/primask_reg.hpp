

#pragma once

#include "registers/standard_reg.hpp"
#include "types/bits.hpp"

namespace micromachine::system {

class primask_reg : public standard_reg {
public:
	using standard_reg::operator=;

	using primask_bit = bits<0>;

	auto pm() const {
		return bits_ref<primask_bit>();
	}

	auto pm() {
		return bits_ref<primask_bit>();
	}
};

} // namespace micromachine::system
