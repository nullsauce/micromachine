#pragma once

#include "registers/iregister.hpp"
#include "registers/standard_reg.hpp"
#include "types/bits.hpp"
#include "types/types.hpp"

namespace micromachine::system {

class shpr2_reg : public standard_reg {
public:
	static constexpr uint32_t SHPR2 = 0xE000ED1C;

	using iregister::operator=;
	using pr11_bits = bits<30, 2>;

	pr11_bits::const_integer_slice<uint32_t> pri11() const {
		return bits<30, 2>::of(_word);
	}

	pr11_bits::integer_slice<uint32_t> pri11() {
		return bits<30, 2>::of(_word);
	}

private:
	static constexpr uint32_t _mask = (0b11 << 30);

	void set(uint32_t word) override {
		_word = word & _mask;
	}

	uint32_t get() const override {
		return _word & _mask;
	}
};

} // namespace micromachine::system
