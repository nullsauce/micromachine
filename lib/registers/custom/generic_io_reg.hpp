#ifndef MICROMACHINE_EMU_GENERIC_IO_REG_HPP
#define MICROMACHINE_EMU_GENERIC_IO_REG_HPP

#include "types.hpp"
#include "registers/word_reg.hpp"

class generic_io_reg : public word_reg {
public:
	using callback_t = std::function<void(uint8_t op, uint8_t data)>;

	generic_io_reg(const callback_t& callback)
		: _callback(callback) {
	}

private:
	void set(word val) override {
		uint8_t data = static_cast<uint8_t>(bits<0, 8>::of(val));
		uint8_t op = static_cast<uint8_t>(bits<8, 8>::of(val));
		if(_callback) _callback(op, data);
	}

	word get() const override {
		return 0U;
	}

protected:
	word _word;
	const callback_t& _callback;
};

#endif //MICROMACHINE_EMU_GENERIC_IO_REG_HPP
