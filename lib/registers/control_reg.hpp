#ifndef MICROMACHINE_CONTROL_REG_HPP
#define MICROMACHINE_CONTROL_REG_HPP

#include "standard_reg.hpp"

class control_reg : public standard_reg {
public:

	using standard_reg::standard_reg;
	using standard_reg::operator=;

	bool n_priv() const {
		return _word.bit(0);
	}

	void set_n_priv(bool priv) {
		_word.write_bit(0, priv);
	}

	bool sp_sel() const {
		return _word.bit(1);
	}

	void set_sp_sel(bool sel) {
		_word.write_bit(1, sel);
	}

};

#endif //MICROMACHINE_CONTROL_REG_HPP
