#pragma once

#include "helpers/binops.hpp"
#include "registers/standard_reg.hpp"
#include "types/bits.hpp"
#include "types/types.hpp"

namespace micromachine::system {

class systick_control_reg : public standard_reg {
public:
	static constexpr uint32_t SYST_CSR = 0xE000E010;
	static constexpr uint32_t SYST_RVR = 0xE000E014;
	static constexpr uint32_t SYST_CVR = 0xE000E018;
	static constexpr uint32_t SYST_CALIB = 0xE000E01C;

	using standard_reg::operator=;

	using countflag_bit = bits<16>;
	using clocksource_bit = bits<2>;
	using tickint_bit = bits<1>;
	using enable_bit = bits<0>;

	bool count_flag() {
		// Reading the COUNTFLAG_BIT clears it
		bool flag = countflag_bit::of(_word);
		set_count_flag(0);
		return flag;
	}

	void set_count_flag(bool flag) {
		bits_ref<countflag_bit>() = flag;
	}

	auto clock_source() const {
		return bits_ref<clocksource_bit>();
	}

	auto clock_source() {
		return bits_ref<clocksource_bit>();
	}

	auto tick_int() const {
		return bits_ref<tickint_bit>();
	}

	auto tick_int() {
		return bits_ref<tickint_bit>();
	}

	auto enabled() const {
		return bits_ref<enable_bit>();
	}

	auto enabled() {
		return bits_ref<enable_bit>();
	}

private:
	static constexpr uint32_t _mask = (0b1U << 16) | 0b111;

	void set(uint32_t word) override {
		_word = word & _mask;
	}

	uint32_t get() const override {
		return _word & _mask;
	}
};

class systick_reload_value_reg : public standard_reg {
public:
	using standard_reg::operator=;
	using reload_value_bits = bits<0, 24>;

private:
	void set(uint32_t word) override {
		bits_ref<reload_value_bits>() = word;
	}

	uint32_t get() const override {
		return bits_ref<reload_value_bits>();
	}
};

class systick_current_value_reg : public standard_reg {
public:
	using standard_reg::operator=;
	using value_bits = bits<0, 24>;

	systick_current_value_reg(systick_control_reg& control_reg)
		: _control_reg(control_reg) {}
	// this setter does NOT clear the register
	void set_internal(uint32_t word) {
		bits_ref<value_bits>() = word;
	}

	void decrement() {
		_word = _word - 1;
	}

private:
	void set(uint32_t) override {
		// Writing to SYST_CVR clears both the register and the COUNTFLAG status bit to zero
		bits_ref<value_bits>().clear();
		_control_reg.set_count_flag(false);
	}

	uint32_t get() const override {
		return bits_ref<value_bits>();
	}

protected:
	systick_control_reg& _control_reg;
};

class systick_calib_value_reg : public standard_reg {
public:
	using standard_reg::operator=;
	static constexpr size_t SKEW_BIT = 30;
	static constexpr size_t NOREF_BIT = 31;

	using tenms_bits = bits<0, 24>;
	using skew_bit = bits<SKEW_BIT>;
	using noref_bit = bits<NOREF_BIT>;

	uint32_t tenms() const {
		return bits_ref<tenms_bits>();
	}

	void set_tenms(uint32_t tenms) {
		bits_ref<tenms_bits>() = tenms_bits::of(tenms);
	}

	bool skew() const {
		return bits_ref<skew_bit>();
	}

	void set_skew(bool flag) {
		bits_ref<skew_bit>() = flag;
	}

	bool noref() const {
		return bits_ref<noref_bit>();
	}

	void set_noref(bool flag) {
		bits_ref<noref_bit>() = flag;
	}

private:
	static constexpr uint32_t _mask = binops::make_mask<uint32_t>(24) | (0b11U << 30);

	void set(uint32_t word) override {
		_word = word & _mask;
	}

	uint32_t get() const override {
		return _word & _mask;
	}
};

} // namespace micromachine::system
