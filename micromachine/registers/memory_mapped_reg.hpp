/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#pragma once

#include "registers/iregister.hpp"
#include "registers/standard_reg.hpp"
#include "types/bits.hpp"
#include "types/types.hpp"

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
	constexpr bool detect_transition_of(uint32_t new_value, bool trigger_state) {
		static_assert(bits_t::length == 1, "Can only detect transition on a single bit");
		const bool bit_changed = bits_t::of(new_value) != self<bits_t>();
		const bool bit_is_in_trigger_state = ((bool)bits_t::of(new_value)) == trigger_state;
		const bool bit_transitioned_to_trigger_state = bit_changed && bit_is_in_trigger_state;
		return bit_transitioned_to_trigger_state;
	}

	template <typename bits_t>
	constexpr auto self() const {
		return bits_t::of(_word);
	}
};

} // namespace micromachine::system
