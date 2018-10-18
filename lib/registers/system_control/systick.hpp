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

	bool count_flag() {
		// Reading the COUNTFLAG_BIT clears it to 0
		bool flag = _word.bit(COUNTFLAG_BIT);
		set_count_flag(0);
		return flag;
	}

	void set_count_flag(bool flag) {
		_word.write_bit(COUNTFLAG_BIT, flag);
	}

	bool clock_source() const {
		return _word.bit(CLKSOURCE_BIT);
	}

	void set_clock_source(bool flag) {
		_word.write_bit(CLKSOURCE_BIT, flag);
	}

	bool tick_int() const {
		return _word.bit(TICKINT_BIT);
	}

	void set_tick_int(bool flag) {
		_word.write_bit(TICKINT_BIT, flag);
	}

	bool enabled() const {
		return _word.bit(ENABLE_BIT);
	}

	void set_enabled(bool flag) {
		_word.write_bit(ENABLE_BIT, flag);
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
		_word.bits<0, 24>() = word;
	}

	word get() const override {
		return _word.bits<0, 24>();
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
		_word.bits<0, 24>() = word;
	}

	void decrement() {
		_word = _word - 1;
	}

private:
	void set(word word) override {
		// Writing to SYST_CVR clears both the register and the COUNTFLAG status bit to zero
		_word.bits<0, 24>().clear();
		_control_reg.set_count_flag(false);
	}

	word get() const override {
		return _word.bits<0, 24>();
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

	word tenms() const {
		return _word.bits<0, 24>();
	}

	void set_tenms(uint32_t tenms) {
		bits<0, 24>::of(_word) = bits<0, 24>::of((word)tenms);
	}

	bool skew() const {
		return _word.bit(SKEW_BIT);
	}

	void set_skew(bool flag) {
		_word.write_bit(SKEW_BIT, flag);
	}

	bool noref() const {
		return _word.bit(NOREF_BIT);
	}

	void set_noref(bool flag) {
		_word.write_bit(NOREF_BIT, flag);
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
