#pragma once

#include "types/bits.hpp"
#include "registers/standard_reg.hpp"

namespace micromachine::system {

class control_reg : public standard_reg {
public:
	using standard_reg::operator=;

	using n_priv_bit = bits<0>;
	using sp_selection_bit = bits<1>;

	auto n_priv() const {
		return bits_ref<n_priv_bit>();
	}

	auto n_priv() {
		return bits_ref<n_priv_bit>();
	}

	auto sp_sel() const {
		return bits_ref<sp_selection_bit>();
	}

	auto sp_sel() {
		return bits_ref<sp_selection_bit>();
	}

	void set_sp_sel(bool sel) {
		sp_selection_bit::of(_word) = sel;
	}
};

} // namespace micromachine::system
