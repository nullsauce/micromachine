#ifndef MICROMACHINE_EMU_SPHR3_HPP
#define MICROMACHINE_EMU_SPHR3_HPP

#include "types.hpp"
#include "registers/word_reg.hpp"

class sphr3_reg : public word_reg {
public:
	using ireg::operator=;

	using pr14_bits = bits<30, 2>;
	using pr15_bits = bits<30, 2>;

	uint32_t pri14() const {
		return self<pr14_bits>();
	}

	uint32_t pri15() const {
		return self<pr15_bits>();
	}

private:
	static constexpr uint32_t _pri14_mask = (0b11 << 22);
	static constexpr uint32_t _pri15_mask = (0b11 << 30);
	static constexpr uint32_t _mask = _pri14_mask| _pri15_mask;

	void set(uint32_t word) override {
		_word = word & _mask;
	}

	uint32_t get() const override {
		return _word & _mask;
	}
};

#endif //MICROMACHINE_EMU_SPHR3_HPP
