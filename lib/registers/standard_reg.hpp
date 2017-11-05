/* Copyright (C) MindMaze SA - 2016 - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of MindMaze SA.

*/

#ifndef MICROMACHINE_STANDARD_REG_HPP_HPP
#define MICROMACHINE_STANDARD_REG_HPP_HPP

#include "ireg.hpp"

class standard_reg : public ireg {
public:

	using ireg::operator=;
	standard_reg() : _word(0) {}

private:

	void set(word word) override {
		_word = word;
	}

	const word get() const override {
		return _word;
	}

protected:
	word _word;
};

#endif //MICROMACHINE_STANDARD_REG_HPP_HPP
