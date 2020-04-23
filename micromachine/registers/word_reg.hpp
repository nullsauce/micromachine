/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#pragma once

#include "bits.hpp"
#include "registers/iregister.hpp"
#include "registers/standard_reg.hpp"
#include "types.hpp"

namespace micromachine::system {

class memory_mapped_reg : public standard_reg {
public:
	using standard_reg::operator=;
	using standard_reg::operator uint32_t;
	using standard_reg::reset;
	using standard_reg::standard_reg;

	template <typename bits_t>
	constexpr auto self() {
		return bits_t::of(_word);
	}

	template <typename bits_t>
	constexpr auto self() const {
		return bits_t::of(_word);
	}
};

} // namespace micromachine::system
