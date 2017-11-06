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
