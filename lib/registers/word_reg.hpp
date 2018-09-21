/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_WORD_REG_HPP
#define MICROMACHINE_EMU_WORD_REG_HPP

#include "types.hpp"
#include "registers/ireg.hpp"

class word_reg : public ireg {
public:
	using ireg::operator=;
	using ireg::operator word;
	using ireg::operator uint32_t;
	word_reg() : _word(0) {}
	void reset() override {
		set(0);
	}
private:
	virtual void set(word word) = 0;
protected:
	word _word;
};


#endif //MICROMACHINE_EMU_WORD_REG_HPP
