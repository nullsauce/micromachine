//
// Copyright (C) 2020 Joel Jungo, Flavio Roth - All Rights Reserved
//

#ifndef MICROMACHINE_USART_REG_HPP
#define MICROMACHINE_USART_REG_HPP

#include <memory>
#include "registers/standard_reg.hpp"

namespace micromachine::system {


/**
 * Usart Interrupt Status register.
 * The USART interrupt events are connected to the same interrupt vector. This
 * register indicate which interrupt are to be serviced by the software.
 */
class usart_is_reg : public standard_reg {
private:
	bool& _interrupt_event;

public:
	usart_is_reg(bool& interrupt_event)
		: _interrupt_event(interrupt_event)
	{}

	/**
	 * This bit is set when the data has been transferred to the usart_rx register. It
	 * is cleared by a read to the usart_rx_reg register.
	 *
	 * An interrupt is generated if @ref usart_cr1_reg::rx_not_empty_interrupt_enable_bit is
	 * set.
	 *
	 * @see set_rx_not_empty
	 * @see rx_not_empty
	 */
	using rx_not_empty_bit = bits<1>;

	/**
	 * Transmission complete
	 *
	 * This bit is set if the transmission of a frame containing data is complete
	 * and if @ref usart_cr1_reg::tx_empty_interrupt_enable_bit is set. An interrupt is generated if
	 * @ref usart_cr1_reg::tx_complete_interrupt_enable_bit is set. It is cleared by software
	 * writing a 1 to @ref usart_icr::tx_complete_bit or by a write to the usart_tx
	 * register.
	 *
	 * An interrupt is generated if TCIE is set
	 * 0: Transmission is not complete
	 * 1: Transmission is complete
	 *
	 * @see usart_cr1_reg::tx_complete_interrupt_enable_bit
	 * @see tx_complete
	 * @see set_tx_complete
	 */
	using tx_complete_bit = bits<2>;

	/**
	 * This bit is set when the content of @ref usart_tx register has been
	 * transferred to the VM. It is cleared by a write to the @ref usart_tx register.
	 *
	 * An interrupt is generated if the @ref usart_cr1_reg::tx_empty_interrupt_enable_bit is
	 * set.
	 *
	 * 0: data is not transferred.
	 * 1: data is transferred.
	 *
	 * @see usart_cr1_reg::tx_empty_interrupt_enable_bit
	 * @see transmit_data_register_empty
	 * @see set_tx_empty
	 */
	using tx_empty_bit = bits<3>;

	bool tx_empty() {
		return bits_ref<tx_empty_bit>();
	}

	void set_tx_empty(bool flag) {
		bits_ref<tx_empty_bit>() = flag;

		// signaling tx as empty generates an interrupt event
		if(flag) {
			_interrupt_event = true;
		}
	}

	bool rx_not_empty() {
		return bits_ref<rx_not_empty_bit>();
	}

	void set_rx_not_empty(bool flag) {
		bits_ref<rx_not_empty_bit>() = flag;

		// signaling rs as not empty generates an interrupt event
		if(flag) {
			_interrupt_event = true;
		}
	}

	void reset() override {
		_word = 0;
	}

private:
	uint32_t get() const override {
		return _word;
	}

	void set(uint32_t word) override {}
};

/**
 * Control register 1.
 * This register allows user to configure the usart peripheral and register for interrupts.
 */
class usart_cr1_reg : public standard_reg {
public:
	using standard_reg::operator=;
	using standard_reg::operator uint32_t;
	using reset_requested = std::function<void()>;

private:
	bool& _interrupt_event;
	const reset_requested _request_reset;

public:
	usart_cr1_reg(bool& interrupt_event, reset_requested request_reset)
		: _interrupt_event(interrupt_event)
		, _request_reset(std::move(request_reset))
	{}

	/**
	 * USART enable
	 *
	 * When this bit is cleared, output is stopped immediately, and
	 * current operations are discarded. The configuration of the USART is kept, but all the
	 * status flags, in the USART_ISR are set to their default values. This bit is set and
	 * cleared by software. 0: USART disabled. 1: USART enabled.
	 *
	 * @see enable
	 * @see set_enable
	 */
	using enable_bit = bits<0>;

	/**
	 * Read data register not empty interrupt enable.
	 * This bit is set and cleared by software.
	 * 0: Interrupt is inhibited
	 * 1: A USART interrupt is generated whenever @ref usart_is_reg::rx_not_empty_bit = 1 in the
	 * USART_ISR register.
	 *
	 * @see rx_not_empty_interrupt_enabled
	 * @see set_rx_not_empty_interrupt_enable
	 */
	using rx_not_empty_interrupt_enable_bit = bits<1>;

	/**
	 *
	 * transmit data register empty interrupt enable.
	 * This bit is set and cleared by software.
	 * 0: Interrupt is inhibited
	 * 1: A USART interrupt is generated whenever @ref tx_empty_bit = 1 in the
	 * USART_ISR register.
	 *
	 * @see tx_empty_interrupt_enabled
	 * @see set_tx_empty_interrupt_enable
	 */
	using tx_empty_interrupt_enable_bit = bits<3>;

	auto tx_empty_interrupt_enabled() const {
		return bits_ref<tx_empty_interrupt_enable_bit>();
	}

	auto rx_not_empty_interrupt_enabled() const {
		return bits_ref<rx_not_empty_interrupt_enable_bit>();
	}

	auto enabled() const {
		return bits_ref<enable_bit>();
	}

	void reset() override {
		_word = 0;
	}

private:
	uint32_t get() const override {
		return _word;
	}

	void set(uint32_t word) override {

		// trigger an interrupt when interrupt are enabled for these signals
		if(detect_transition_of<tx_empty_interrupt_enable_bit>(word, true)) {
			_interrupt_event = true;
		} else if(detect_transition_of<rx_not_empty_interrupt_enable_bit>(word, true)) {
			_interrupt_event = true;
		}

		// On disable, reset the controller
		if(detect_transition_of<enable_bit>(word, false)) {
			_request_reset();
		}

		_word = word;
	}
};

/**
 * USART interrupt clear register
 */
class usart_ic_reg : public standard_reg {
public:
	using standard_reg::operator=;
	using standard_reg::operator uint32_t;

	usart_ic_reg(usart_is_reg& isr)
		: _usart_is_reg(isr) {}

	/**
	 * RX register not empty clear flag.
	 * Clears @ref usart_is_reg::rx_not_empty_bit bit.
	 * @see tx_complete
	 * @see set_tx_complete
	 */
	using clear_rx_not_empty_bit = bits<1>;

	/**
	 * TX register empty clear flag.
	 * Clears @ref usart_is_reg::tx_empty_bit bit.
	 * @see tx_complete
	 * @see set_tx_complete
	 */
	using clear_tx_empty_bit = bits<2>;

private:
	uint32_t get() const override {
		return 0;
	}

	void set(uint32_t word) override {

		if(clear_rx_not_empty_bit::of(word)) {
			_usart_is_reg.set_rx_not_empty(false);
		}

		if(clear_tx_empty_bit::of(word)) {
			_usart_is_reg.set_tx_empty(false);
		}
	}

	usart_is_reg& _usart_is_reg;
};

/**
 * USART tx register
 */
class usart_tx_reg : public standard_reg {
private:
	usart_is_reg& _usart_isr_reg;

public:
	using standard_reg::operator=;
	using standard_reg::operator uint32_t;
	using data_bits = bits<0, 8>;

	usart_tx_reg(usart_is_reg& usart_is_reg)
		: _usart_isr_reg(usart_is_reg)
	{}

	/**
	 * Gets a byte from the tx register
	 * @details This function is to be used by the virtual machine.
	 */
	uint8_t read() {
		uint8_t byte = bits_ref<data_bits>();

		// Signals tx as empty which triggers an interrupt event
		// and maybe an interrupt request.
		_usart_isr_reg.set_tx_empty(true);
		return byte;
	}

	void reset() override {
		_word = 0;
		_usart_isr_reg.set_tx_empty(true);
	}

private:
	uint32_t get() const override {
		return 0;
	}

	void set(uint32_t word) override {
		bits_ref<data_bits>() = word;
		_usart_isr_reg.set_tx_empty(false);
	}
};

/**
 * USART rx register
 */
class usart_rx_reg : public standard_reg {
private:
	usart_is_reg& _usart_isr_reg;

public:
	using standard_reg::operator=;
	using standard_reg::operator uint32_t;
	using data_bits = bits<0, 8>;

	usart_rx_reg(usart_is_reg& usart_is_reg)
		: _usart_isr_reg(usart_is_reg)
	{}

	/**
	 * Write a byte to the rx register
	 * @details This function is to be used by the virtual machine.
	 */
	void write(uint8_t byte) {
		bits_ref<data_bits>() = byte;

		// Signals rx as not empty which triggers an interrupt event
		// and maybe an interrupt request.
		_usart_isr_reg.set_rx_not_empty(true);
	}

private:
	uint32_t get() const override {

		// return zero if there's no data available for reading
		if(!_usart_isr_reg.rx_not_empty()) {
			return 0;
		}

		// otherwise just return whatever data is in there and mark the register as empty
		uint32_t word = bits_ref<data_bits>();
		_usart_isr_reg.set_rx_not_empty(false);

		return word;
	}

	void set(uint32_t word) override {}

};
}; //namespace micromachine::system

#endif // MICROMACHINE_USART_REG_HPP
