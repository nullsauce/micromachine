#ifndef MICROMACHINE_EMU_NVIC_HPP
#define MICROMACHINE_EMU_NVIC_HPP

#include "types.hpp"
#include "registers/word_reg.hpp"
#include <array>

// Interrupt Priority Registers
class nvic_ipr_reg : public word_reg {
public:
	using ireg::operator=;

	uint8_t get_priority_n(size_t index) const {
		return binops::get_bits(_word, 6 + (index * 8), 2);
	}

private:
	static constexpr uint32_t mask = 0b11000000110000001100000011000000;
	void set(uint32_t word) override {
		_word = word & mask;
	}

	uint32_t get() const override {
		return _word;
	}
};

class nvic_status_reg : public iword_reg {

public:
	nvic_status_reg(uint32_t& word) : _status(word) {}

protected:
	uint32_t& _status;
};

/*
 * On write 0: no effect
 * On write 1: enable to associated bit
 */
class nvic_enable_on_write_reg : public nvic_status_reg {
public:
	using ireg::operator=;
	using nvic_status_reg::nvic_status_reg;

private:
	void set(uint32_t word) override {
		_status |= word;
	}

	uint32_t get() const override {
		return _status;
	}
};

/*
 * On write 0: no effect
 * On write 1: disable to associated bit
 */
class nvic_disable_on_write_reg : public nvic_status_reg {
public:
	using ireg::operator=;
	using nvic_status_reg::nvic_status_reg;

private:
	void set(uint32_t word) override {
		_status = _status & ~word;
	}

	uint32_t get() const override {
		return _status;
	}
};

/* Interrupt Set-Enable Register
 * Enables, or reads the enabled state of one or more interrupts.
 */
class nvic_iser_reg : public nvic_enable_on_write_reg {
public:
	using ireg::operator=;
	using nvic_enable_on_write_reg::nvic_enable_on_write_reg;
};

/* Interrupt Clear Enable Register
 * Disables, or reads the enabled state of one or more interrupts.
 */
class nvic_icer_reg : public nvic_disable_on_write_reg {
public:
	using ireg::operator=;
	using nvic_disable_on_write_reg::nvic_disable_on_write_reg;
};

/* Interrupt Set-Pending Register
 * On writes, sets the status of one or more interrupts to pending. On reads, shows the
 * pending status of the interrupts.
 */
class nvic_ispr_reg : public nvic_enable_on_write_reg {
public:
	using ireg::operator=;
	using nvic_enable_on_write_reg::nvic_enable_on_write_reg;
};

/* Interrupt Clear-Pending Register
 * On writes, clears the status of one or more interrupts to pending. On reads, shows
 * the pending status of the interrupts.
 */
class nvic_icpr_reg : public nvic_disable_on_write_reg {
public:
	using ireg::operator=;
	using nvic_disable_on_write_reg::nvic_disable_on_write_reg;
};

class nvic {
public:


	nvic()
		: _iser_reg(_enable_status)
		, _icer_reg(_enable_status)
		, _ispr_reg(_pending_status)
		, _icpr_reg(_pending_status) {

	}

	void reset() {
		_iser_reg.reset();
		_icer_reg.reset();
		_ispr_reg.reset();
		_icpr_reg.reset();
		for(auto& reg : _priority_regs) {
			reg.reset();
		}
	}

	template<size_t index>
	nvic_ipr_reg& priority_reg() {
		return _priority_regs[index];
	}

	nvic_ipr_reg& priority_reg_at(size_t index) {
		return _priority_regs.at(index);
	}

	uint8_t external_interrupt_priority(size_t external_interrupt_number) const {
		return _priority_regs.at(external_interrupt_number / 4)
			.get_priority_n(external_interrupt_number % 4);
	}

	nvic_iser_reg _iser_reg;
	nvic_icer_reg _icer_reg;
	nvic_ispr_reg _ispr_reg;
	nvic_icpr_reg _icpr_reg;
	std::array<nvic_ipr_reg, 8> _priority_regs;

	friend class tester;
	class tester {
	public:
		tester(nvic* nvic) : _nvic(nvic) {}
		uint32_t& enable_status() {
			return _nvic->_enable_status;
		}
		uint32_t& pending_status() {
			return _nvic->_pending_status;
		}
	private:
		nvic* _nvic;
	};

private:


	uint32_t _enable_status;
	uint32_t _pending_status;
};

#endif //MICROMACHINE_EMU_NVIC_HPP
