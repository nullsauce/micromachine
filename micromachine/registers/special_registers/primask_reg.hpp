/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

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
