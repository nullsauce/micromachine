#ifndef MICROMACHINE_EMU_SPHR3_HPP
#define MICROMACHINE_EMU_SPHR3_HPP

#include "types.hpp"
#include "registers/ireg.hpp"

class sphr3_reg : public ireg {
public:

	using ireg::operator=;
	sphr3_reg() : _word(0) {}

	word pri14() const {
		return get().bits<22, 2>();
	}

	word pri15() const {
		return get().bits<30, 2>();
	}

private:

	static constexpr uint32_t _pri14_mask = (0b11 << 22);
	static constexpr uint32_t _pri15_mask = (0b11 << 30);
	static constexpr uint32_t _mask = _pri14_mask| _pri15_mask;

	void set(word word) override {
		_word = word & _mask;
	}

	const word get() const override {
		return _word & _mask;
	}

protected:
	word _word;
};

#endif //MICROMACHINE_EMU_SPHR3_HPP
