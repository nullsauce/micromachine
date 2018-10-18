#ifndef MICROMACHINE_EMU_SPHR2_HPP
#define MICROMACHINE_EMU_SPHR2_HPP

#include "types.hpp"
#include "registers/word_reg.hpp"

class sphr2_reg : public word_reg {
public:
	using ireg::operator=;
	using pr11_bits = bits<30, 2>;

	uint32_t pri11() const {
		return self<pr11_bits>();
	}

private:
	static constexpr uint32_t _mask = (0b11 << 30);

	void set(uint32_t word) override {
		_word = word & _mask;
	}

	uint32_t get() const override {
		return _word & _mask;
	}

protected:
	uint32_t _word;
};

#endif //MICROMACHINE_EMU_SPHR2_HPP
