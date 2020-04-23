#pragma once

#include "bits.hpp"
#include "registers/iregister.hpp"
#include "registers/word_reg.hpp"
#include "types.hpp"

namespace micromachine::system {

class shpr3_reg : public memory_mapped_reg {
public:
	static constexpr uint32_t SHPR3 = 0xE000ED20;

	using iregister::operator=;

	using pr14_bits = bits<22, 2>;
	using pr15_bits = bits<30, 2>;

	pr14_bits::const_integer_slice<uint32_t> pri14() const {
		return pr14_bits::of(_word);
	}

	pr14_bits::integer_slice<uint32_t> pri14() {
		return pr14_bits::of(_word);
	}

	pr15_bits::const_integer_slice<uint32_t> pri15() const {
		return pr15_bits::of(_word);
	}

	pr15_bits::integer_slice<uint32_t> pri15() {
		return pr15_bits::of(_word);
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

} // namespace micromachine::system
