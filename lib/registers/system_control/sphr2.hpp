#ifndef MICROMACHINE_EMU_SPHR2_HPP
#define MICROMACHINE_EMU_SPHR2_HPP

#include "types.hpp"
#include "registers/word_reg.hpp"

class sphr2_reg : public word_reg {
public:
	using ireg::operator=;

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
