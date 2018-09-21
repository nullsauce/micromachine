#ifndef MICROMACHINE_EMU_SPHR3_HPP
#define MICROMACHINE_EMU_SPHR3_HPP

#include "types.hpp"
#include "registers/word_reg.hpp"

class sphr3_reg : public word_reg {
public:
	using ireg::operator=;

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
};

#endif //MICROMACHINE_EMU_SPHR3_HPP
