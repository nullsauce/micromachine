/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_PRIMASK_REG_HPP
#define MICROMACHINE_EMU_PRIMASK_REG_HPP

#include "bits.hpp"
#include "standard_reg.hpp"
namespace micromachine::system {

class primask_reg : public standard_reg {
public:
	using standard_reg::standard_reg;
	using standard_reg::operator=;
	using pm_bit = bits<0>;

	bool pm() const {
		return pm_bit::of(_word);
	}

	void set_pm(bool pm) {
		pm_bit::of(_word) = pm;
	}
};

} // namespace micromachine::system

#endif //MICROMACHINE_EMU_PRIMASK_REG_HPP
