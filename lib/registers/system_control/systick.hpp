#ifndef MICROMACHINE_EMU_SYSTICK_HPP
#define MICROMACHINE_EMU_SYSTICK_HPP

#include "bits.hpp"
#include "types.hpp"
#include "registers/word_reg.hpp"

class systick_control_reg : public word_reg {
public:
	using word_reg::operator=;
	static constexpr size_t COUNTFLAG_BIT = 16;
	static constexpr size_t CLKSOURCE_BIT = 2;
	static constexpr size_t TICKINT_BIT = 1;
	static constexpr size_t ENABLE_BIT = 0;

	using countflag_bit = bits<COUNTFLAG_BIT>;
	using clocksource_bit = bits<CLKSOURCE_BIT>;
	using tickint_bit = bits<TICKINT_BIT>;
	using enable_bit = bits<ENABLE_BIT>;

	bool count_flag() {
		// Reading the COUNTFLAG_BIT clears it to 0
		bool flag = countflag_bit::of(_word);
		set_count_flag(0);
		return flag;
	}

	void set_count_flag(bool flag) {
		self<countflag_bit>() = flag;
	}

	bool clock_source() const {
		return self<clocksource_bit>();
	}

	void set_clock_source(bool flag) {
		self<clocksource_bit>() = flag;
	}

	bool tick_int() const {
		return self<tickint_bit>();
	}

	void set_tick_int(bool flag) {
		self<tickint_bit>() = flag;
	}

	bool enabled() const {
		return self<enable_bit>();
	}

	void set_enabled(bool flag) {
		self<enable_bit>() = flag;
	}

private:
	static constexpr uint32_t _mask = (0b1U << 16) | 0b111;

	void set(word word) override {
		_word = word & _mask;
	}

	word get() const override {
		return _word & _mask;
	}
};

class systick_reload_value_reg : public word_reg {
public:
	using word_reg::operator=;
private:
	void set(word word) override {
		bits<0,24>::of(_word) = word;
	}

	word get() const override {
		return bits<0,24>::of(_word);
	}
};

class systick_current_value_reg : public word_reg {
public:
	using word_reg::operator=;
	systick_current_value_reg(systick_control_reg& control_reg)
		: word_reg()
		, _control_reg(control_reg) {
	}
	// this setter does NOT clear the register
	void set_internal(word word) {
		bits<0,24>::of(_word) = word;
	}

	void decrement() {
		_word = _word - 1;
	}

private:
	void set(word word) override {
		// Writing to SYST_CVR clears both the register and the COUNTFLAG status bit to zero
		bits<0,24>::of(_word).clear();
		_control_reg.set_count_flag(false);
	}

	word get() const override {
		return bits<0,24>::of(_word);
	}

protected:
	word _word;
	systick_control_reg& _control_reg;
};

class systick_calib_value_reg : public word_reg {
public:
	using word_reg::operator=;
	static constexpr size_t SKEW_BIT = 30;
	static constexpr size_t NOREF_BIT = 31;

	using tenms_bits = bits<0,24>;
	using skew_bit = bits<SKEW_BIT>;
	using noref_bit = bits<NOREF_BIT>;

	word tenms() const {
		return self<tenms_bits>();
	}

	void set_tenms(uint32_t tenms) {
		self<tenms_bits>() = tenms_bits::of(tenms);
	}

	bool skew() const {
		return self<skew_bit>();
	}

	void set_skew(bool flag) {
		self<skew_bit>() = flag;
	}

	bool noref() const {
		return self<noref_bit>();
	}

	void set_noref(bool flag) {
		self<noref_bit>() = flag;
	}

private:
	static constexpr uint32_t _mask = binops::make_mask<24>() | (0b11U << 30);

	void set(word word) override {
		_word = word & _mask;
	}

	word get() const override {
		return _word & _mask;
	}
};

#endif //MICROMACHINE_EMU_SYSTICK_HPP
