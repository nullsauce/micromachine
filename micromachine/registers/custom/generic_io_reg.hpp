#ifndef MICROMACHINE_EMU_GENERIC_IO_REG_HPP
#define MICROMACHINE_EMU_GENERIC_IO_REG_HPP

#include "bits.hpp"
#include "registers/word_reg.hpp"
#include "types.hpp"

namespace micromachine::system {
class generic_io_reg : public word_reg {
public:
	static constexpr uint32_t GIO_IO = 0xE000EF90;

	using callback_t = std::function<void(uint8_t op, uint8_t data)>;

	generic_io_reg(const callback_t& callback)
		: _callback(callback) {
	}

private:
	void set(uint32_t val) override {
		uint8_t data = static_cast<uint8_t>(bits<0, 8>::of(val));
		uint8_t op = static_cast<uint8_t>(bits<8, 8>::of(val));
		if(_callback) _callback(op, data);
	}

	uint32_t get() const override {
		return 0U;
	}

protected:
	const callback_t& _callback;
};

} // namespace micromachine::system

#endif //MICROMACHINE_EMU_GENERIC_IO_REG_HPP
