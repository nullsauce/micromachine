#ifndef MICROMACHINE_EMU_SPHR2_HPP
#define MICROMACHINE_EMU_SPHR2_HPP

#include "types.hpp"
#include "registers/ireg.hpp"

class sphr2_reg : public ireg {
public:

	using ireg::operator=;
	sphr2_reg() : _word(0) {}

	word pri11() const {
		return get().bits<30, 2>();
	}

private:

	static constexpr uint32_t _mask = (0b11 << 30);

	void set(word word) override {
		_word = word & _mask;
	}

	const word get() const override {
		return _word & _mask;
	}

protected:
	word _word;
};

#endif //MICROMACHINE_EMU_SPHR2_HPP
