/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_WORD_REG_HPP
#define MICROMACHINE_EMU_WORD_REG_HPP

#include "types.hpp"
#include "registers/ireg.hpp"
#include "bits.hpp"

class iword_reg : public ireg {
	public:
		using ireg::operator=;
		using ireg::operator uint32_t;
		void reset() override {
			*this = 0U;
		}
};

class word_reg : public iword_reg {
public:
	using ireg::operator=;
	using ireg::operator uint32_t;
	word_reg() : _word(0) {}
	template<typename bits_t>
	auto self() {
		return bits_t::of(_word);
	}

	template<typename bits_t>
	auto self() const {
		return bits_t::of(_word);
	}
	uint32_t _word;
};


#endif //MICROMACHINE_EMU_WORD_REG_HPP
