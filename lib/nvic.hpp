#ifndef MICROMACHINE_EMU_NVIC_HPP
#define MICROMACHINE_EMU_NVIC_HPP

#include "types.hpp"
#include "registers/word_reg.hpp"
#include <array>

// Interrupt Priority Registers
class nvic_ipr_reg : public word_reg {
public:
	using ireg::operator=;

	template <size_t index>
	using bits_for = bits<6 + (index * 8), 2>;

	template<size_t index>
	typename bits_for<index>::template const_integer_slice<uint32_t> priority_bits_for_nth_interrupt() const {
		return bits_for<index>::of(_word);
	}

	template<size_t index>
	typename bits_for<index>::template integer_slice<uint32_t> priority_bits_for_nth_interrupt() {
		return bits_for<index>::of(_word);
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
 * Enables, or reads the enabled state of one or more exceptions.
 */
class nvic_iser_reg : public nvic_enable_on_write_reg {
public:
	using ireg::operator=;
	using nvic_enable_on_write_reg::nvic_enable_on_write_reg;
};

/* Interrupt Clear Enable Register
 * Disables, or reads the enabled state of one or more exceptions.
 */
class nvic_icer_reg : public nvic_disable_on_write_reg {
public:
	using ireg::operator=;
	using nvic_disable_on_write_reg::nvic_disable_on_write_reg;
};

/* Interrupt Set-Pending Register
 * On writes, sets the status of one or more exceptions to pending. On reads, shows the
 * pending status of the exceptions.
 */
class nvic_ispr_reg : public nvic_enable_on_write_reg {
public:
	using ireg::operator=;
	using nvic_enable_on_write_reg::nvic_enable_on_write_reg;
};

/* Interrupt Clear-Pending Register
 * On writes, clears the status of one or more exceptions to pending. On reads, shows
 * the pending status of the exceptions.
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

	/// Gets a mutable reference to the two priority bits for a given external interrupt
	/// \tparam exti_number External interrupter number [0 - 15]
	/// \return a mutable reference to the two priority bits of this external interrupt
	template<size_t exti_number>
	typename nvic_ipr_reg::bits_for<exti_number % 4>::template integer_slice<uint32_t> priority_bits_for() {
		return priority_reg<exti_number / 4>().template priority_bits_for_nth_interrupt<exti_number % 4>();
	}

	/// Gets an immutable reference to the two priority bits for a given external interrupt
	/// \tparam exti_number External interrupter number [0 - 15]
	/// \return an immutable reference to the two priority bits of this external interrupt
	template<size_t exti_number>
	typename nvic_ipr_reg::bits_for<exti_number % 4>::template const_integer_slice<uint32_t> priority_bits_for() const {
		return priority_reg<exti_number / 4>().template priority_bits_for_nth_interrupt<exti_number % 4>();
	}

	/// Gets a mutable reference to the nth interrupt priority register
	/// \tparam index register index
	/// \return a mutable reference to this interrupt priority register
	template<size_t index>
	nvic_ipr_reg& priority_reg() {
		return _priority_regs[index];
	}

	/// Gets an immutable reference to the nth interrupt priority register
	/// \tparam index register index
	/// \return an immutable reference to this interrupt priority register
	template<size_t index>
	const nvic_ipr_reg& priority_reg() const {
		return _priority_regs[index];
	}

// TODO: make this protected or private
public:
	nvic_iser_reg _iser_reg;
	nvic_icer_reg _icer_reg;
	nvic_ispr_reg _ispr_reg;
	nvic_icpr_reg _icpr_reg;
	std::array<nvic_ipr_reg, 8> _priority_regs;

	// used for unit tests
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
