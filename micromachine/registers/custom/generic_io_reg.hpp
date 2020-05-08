#pragma once

#include "registers/memory_mapped_reg.hpp"
#include "types/bits.hpp"
#include "types/types.hpp"

namespace micromachine::system {

class generic_io_reg : public memory_mapped_reg {
public:
	using callback_t = std::function<void(uint8_t data)>;
	static constexpr uint32_t GIO_IO = 0xE000EF90;

protected:
	uint32_t _word;
	const callback_t& _callback;

public:
	generic_io_reg(const callback_t& callback)
		: _callback(callback) {}

private:
	void set(uint32_t val) override {
		uint8_t data = bits<0, 8>::of(val);
		if(_callback) {
			_callback(data);
		}
	}

	uint32_t get() const override {
		return 0U;
	}
};

} // namespace micromachine::system
