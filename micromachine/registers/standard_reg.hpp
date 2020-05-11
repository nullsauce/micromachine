#pragma once

#include "registers/iregister.hpp"
#include "types/bits.hpp"

namespace micromachine::system {

/**
 * A zero-initialized 32 bit register value that can be reset to zero.
 */
class standard_reg : public iregister {
public:
	using iregister::operator=;

	standard_reg(const standard_reg& existing_state)
		: _word(existing_state) {}

	standard_reg(uint32_t value)
		: _word(value) {}

	standard_reg()
		: standard_reg(0) {}

	void reset() override {
		set(0);
	}

protected:
	template <typename bits_t>
	constexpr auto bits_ref() {
		return bits_t::of(_word);
	}

	template <typename bits_t>
	constexpr auto bits_ref() const {
		return bits_t::of(_word);
	}

	template <typename bits_t>
	constexpr bool detect_transition_of(uint32_t new_value, bool trigger_state) {
		static_assert(bits_t::length == 1, "Can only detect transition on a single bit");
		const bool bit_changed = bits_t::of(new_value) != bits_ref<bits_t>();
		const bool bit_is_in_trigger_state = ((bool)bits_t::of(new_value)) == trigger_state;
		const bool bit_transitioned_to_trigger_state = bit_changed && bit_is_in_trigger_state;
		return bit_transitioned_to_trigger_state;
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
