#ifndef MICROMACHINE_STANDARD_REG_HPP_HPP
#define MICROMACHINE_STANDARD_REG_HPP_HPP

#include "registers/iregister.hpp"

namespace micromachine::system {

/**
 * A zero-initialized 32 bit register value that can be reset to zero.
 */
class standard_reg : public iregister {
public:
	using iregister::operator=;

	standard_reg()
		: _word(0) {}

	standard_reg(const standard_reg& existing_state)
		: _word(existing_state) {}

	void reset() override {
		set(0);
	}

private:
	void set(uint32_t word) override {
		_word = word;
	}

	uint32_t get() const override {
		return _word;
	}

protected:
	uint32_t _word;
};

} // namespace micromachine::system

#endif // MICROMACHINE_STANDARD_REG_HPP_HPP
