#ifndef MICROMACHINE_EMU_NVIC_HPP
#define MICROMACHINE_EMU_NVIC_HPP

#include "types.hpp"
#include "registers/word_reg.hpp"
#include <array>

// Interrupt Priority Registers
class nvic_ipr_reg : public word_reg {
public:
	using ireg::operator=;

	uint8_t get_priority_n(size_t index) const {
		return binops::get_bits(6 + (index * 8), 2);
	}

private:
	static constexpr uint32_t mask = 0b11000000110000001100000011000000;
	void set(uint32_t word) override {
		_word = word & mask;
	}

	uint32_t get() const override {
		return _word;
	}

	std::arry<std::reference_wrapper<slice<>

protected:
	uint32_t _word;
};

class nvic_ipr_reg_array {

	nvic_ipr_reg& at(index) {
		return _regs.at(index);
	}

	std::array<nvic_ipr_reg, 8> _regs;
};

#endif //MICROMACHINE_EMU_NVIC_HPP
