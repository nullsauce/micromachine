#pragma once

#include "standard_reg.hpp"
#include "types/bits.hpp"

namespace micromachine::system {

class control_reg : public standard_reg {
public:
	using standard_reg::standard_reg;
	using standard_reg::operator=;
	using n_priv_bit = bits<0>;
	using sp_selection_bit = bits<1>;

	bool n_priv() const {
		return n_priv_bit::of(_word);
	}

	void set_n_priv(bool priv) {
		n_priv_bit::of(_word) = priv;
	}

	bool sp_sel() const {
		return sp_selection_bit::of(_word);
	}

	void set_sp_sel(bool sel) {
		sp_selection_bit::of(_word) = sel;
	}
};

} // namespace micromachine::system
