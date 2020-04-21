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
 * On write a 0 bit: no effect
 * On write a 1 bit: disable the associated bit
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
private:
	// Interrupt Priority Registers (NVIC_IPR[0-7])
	// Holds the 2-bit priority value of each of the 32 interrupts.
	// Expressed as an array of 8 NVIC_IPR registers holding 4 priority values each
	std::array<nvic_ipr_reg, 8> _priority_regs;

	// The actual data word used to store the state that can be accessed by iser and icer.
	// Holds the enable flag of each the 32 interrupts
	uint32_t _interrupt_enable_statuses;

	// The actual data word used to store the state of ispr and icpr
	// Holds the pending flag of each of the 32 interrupts
	uint32_t _interrupt_pending_statuses;

	// Interrupt set-enable register accessor
	nvic_iser_reg _iser_reg;

	// Interrupt clear enable register accessor
	nvic_icer_reg _icer_reg;

	// Interrupt set-pending register accessor
	nvic_ispr_reg _ispr_reg;

	// Interrupt clear-pending register accessor
	nvic_icpr_reg _icpr_reg;

	nvic(uint32_t interrupt_enable_status, uint32_t interrupt_pending_status)
		: _interrupt_enable_statuses(interrupt_enable_status)
		, _interrupt_pending_statuses(interrupt_pending_status)
		, _iser_reg(_interrupt_enable_statuses)
		, _icer_reg(_interrupt_enable_statuses)
		, _ispr_reg(_interrupt_pending_statuses)
		, _icpr_reg(_interrupt_pending_statuses)
	{}

public:

	static constexpr uint32_t NVIC_ISER = 0xE000E100;
	static constexpr uint32_t NVIC_ICER = 0xE000E180;
	static constexpr uint32_t NVIC_ISPR = 0xE000E200;
	static constexpr uint32_t NVIC_ICPR = 0xE000E280;
	static constexpr uint32_t NVIC_IPR0 = 0xE000E400;
	static constexpr uint32_t NVIC_IPR1 = 0xE000E404;
	static constexpr uint32_t NVIC_IPR2 = 0xE000E408;
	static constexpr uint32_t NVIC_IPR3 = 0xE000E40C;
	static constexpr uint32_t NVIC_IPR4 = 0xE000E40E;
	static constexpr uint32_t NVIC_IPR5 = 0xE000E410;
	static constexpr uint32_t NVIC_IPR6 = 0xE000E414;
	static constexpr uint32_t NVIC_IPR7 = 0xE000E418;

	nvic() : nvic(0U, 0U) {}

	nvic(const nvic& other) : nvic(other._interrupt_enable_statuses, other._interrupt_pending_statuses) {}

	// This is used for testing purposes
	static nvic with_enable_statuses(uint32_t statuses) {
		return nvic(statuses, 0U);
	}

	// This is used for testing purposes
	static nvic with_pending_statuses(uint32_t statuses) {
		return nvic(0U, statuses);
	}

	void reset() {
		_interrupt_enable_statuses = 0U;
		_interrupt_pending_statuses = 0U;
		for(auto& reg : _priority_regs) {
			reg.reset();
		}
	}

	nvic_iser_reg& iser_reg() {
		return _iser_reg;
	}

	nvic_icer_reg& icer_reg() {
		return _icer_reg;
	}

	nvic_ispr_reg& ispr_reg() {
		return _ispr_reg;
	}

	nvic_icpr_reg& icpr_reg() {
		return _icpr_reg;
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


private:

};

#endif //MICROMACHINE_EMU_NVIC_HPP
