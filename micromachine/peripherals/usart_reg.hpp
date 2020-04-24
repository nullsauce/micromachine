//
// Created by joel on 15/04/2020.
//

#ifndef MICROMACHINE_USART_REG_HPP
#define MICROMACHINE_USART_REG_HPP

#include "registers/memory_mapped_reg.hpp"
#include <memory>
namespace micromachine::system {

namespace usart {
	static constexpr uint32_t USART_BASE = 0x40000000;
}

class usart_cr1_reg;

class usart_is_reg;

class usart_ic_reg;

class usart_tx_reg;

class usart_rx_reg;

/**
 * Control register 1
 */
class usart_cr1_reg : public memory_mapped_reg {
public:
	using memory_mapped_reg::operator=;
	using memory_mapped_reg::operator uint32_t;
	static constexpr uint32_t USART_CR1 = usart::USART_BASE + 0x00;
	static constexpr uint32_t MASK = 0b1111;

	/**
	 * USART enable
	 *
	 * When this bit is cleared, output is stopped immediately, and
	 * current operations are discarded. The configuration of the USART is kept, but all the
	 * status flags, in the USART_ISR are set to their default values. This bit is set and
	 * cleared by software. 0: USART prescaler and outputs disabled. 1: USART enabled.
	 *
	 * @see enable
	 * @see set_enable
	 */
	using enable_bit = bits<0>;

	/**
	 * Read data register not empty interrupt enable.
	 * This bit is set and cleared by software.
	 * 0: Interrupt is inhibited
	 * 1: A USART interrupt is generated whenever @ref rx_not_empty_int_status = 1 in the
	 * USART_ISR register.
	 *
	 * @see rx_not_empty_interrupt_enable
	 * @see set_rx_not_empty_interrupt_enable
	 */
	using rx_not_empty_interrupt_enable_bit = bits<1>;

	/**
	 * transmission complete interrupt enable.
	 * This bit is set and cleared by software.
	 * 0: Interrupt is inhibited
	 * 1: A USART interrupt is generated whenever @ref tx_complete_int_status = 1 in the
	 * USART_ISR register.
	 *
	 * @see tx_complete_interrupt_enable
	 * @see set_tx_complete_interrupt_enable
	 */
	using tx_complete_interrupt_enable_bit = bits<2>;

	/**
	 *
	 * transmit data register empty interrupt enable.
	 * This bit is set and cleared by software.
	 * 0: Interrupt is inhibited
	 * 1: A USART interrupt is generated whenever @ref transmit_data_register_empty_bit = 1 in the
	 * USART_ISR register.
	 *
	 * @see tx_empty_interrupt_enable
	 * @see set_tx_empty_interrupt_enable
	 */
	using tx_empty_interrupt_enable_bit = bits<3>;

	bool tx_empty_interrupt_enable() const {
		return self<tx_empty_interrupt_enable_bit>();
	}

	void set_tx_empty_interrupt_enable(bool flag) {
		self<tx_empty_interrupt_enable_bit>() = flag;
	}

	bool tx_complete_interrupt_enable() const {
		return self<tx_complete_interrupt_enable_bit>();
	}

	void set_tx_complete_interrupt_enable(bool flag) {
		self<tx_complete_interrupt_enable_bit>() = flag;
	}

	bool rx_not_empty_interrupt_enable() const {
		return self<rx_not_empty_interrupt_enable_bit>();
	}

	void set_rx_not_empty_interrupt_enable(bool flag) {
		self<rx_not_empty_interrupt_enable_bit>() = flag;
	}

	bool enable() const {
		return self<enable_bit>();
	}

	void set_enable(bool flag) {
		self<enable_bit>() = flag;
	}

private:
	uint32_t get() const override {
		return _word & MASK;
	}

	void set(uint32_t word) override {
		_word = (word & MASK);
	}
};

/**
 * Usart Interrupt Status register
 */
class usart_is_reg : public memory_mapped_reg {
public:
	using memory_mapped_reg::operator=;
	using memory_mapped_reg::operator uint32_t;

	static constexpr uint32_t USART_ISR = usart::USART_BASE + 0x04;
	static constexpr uint32_t MASK = 0b1110;


	usart_is_reg(usart_cr1_reg& control_register)
		: _control_register(control_register) {}


	/**
	* This bit is set by hardware when the data has been transferred to the usart_rx register. It
	* is cleared by a read to the usart_rx_reg register.
	*
	* An interrupt is generated if @ref usart_cr1_reg::rx_not_empty_interrupt_enable_bit is
	* set.
	*
	* @see set_read_data_register_not_empty
	* @see read_data_register_not_empty
	*/
	using read_data_register_not_empty_bit = bits<1>;

	/**
	 * Transmission complete
	 *
	 * This bit is set by hardware if the transmission of a frame containing data is complete
	 * and if @ref usart_cr1_reg::tx_empty_interrupt_enable_bit is set. An interrupt is generated if
	 * @ref usart_cr1_reg::tx_complete_interrupt_enable_bit is set. It is cleared by software
	 * writing a 1 to @ref usart_icr::transmission_complete_bit or by a write to the usart_tx
	 * register.
	 *
	 * An interrupt is generated if TCIE is set
	 * 0: Transmission is not complete
	 * 1: Transmission is complete
	 *
	 * @see usart_cr1_reg::tx_complete_interrupt_enable_bit
	 * @see transmission_complete
	 * @see set_transmission_complete
	 */
	using transmission_complete_bit = bits<2>;

	/**
	 * This bit is set when the content of @ref usart_tx register has been
	 * transferred to the client. It is cleared by a write to the @ref usart_tx register.
	 *
	 * An interrupt is generated if the @ref usart_cr1_reg::tx_empty_interrupt_enable_bit is
	 * set.
	 *
	 * 0: data is not transferred.
	 * 1: data is transferred.
	 *
	 * @see usart_cr1_reg::tx_empty_interrupt_enable_bit
	 * @see transmit_data_register_empty
	 * @see set_transmit_data_register_empty
	 */
	using transmit_data_register_empty_bit = bits<3>;


	bool transmit_data_register_empty() {
		return self<transmit_data_register_empty_bit>();
	}

	void set_transmit_data_register_empty(bool flag) {
		self<transmit_data_register_empty_bit>() = flag;
	}

	bool transmission_complete() {
		return self<transmission_complete_bit>();
	}

	void set_transmission_complete(bool flag) {
		self<transmission_complete_bit>() = flag;
	}

	bool read_data_register_not_empty() {
		return self<read_data_register_not_empty_bit>();
	}

	void set_read_data_register_not_empty(bool flag) {
		self<read_data_register_not_empty_bit>() = flag;
	}

private:
	uint32_t get() const override {
		return _word & MASK;
	}

	void set(uint32_t word) override {
		_word = (word & MASK);
	}

	usart_cr1_reg& _control_register;
};

/**
 * USART interrupt clear register
 */
class usart_ic_reg : public memory_mapped_reg {
public:
	using memory_mapped_reg::operator=;
	using memory_mapped_reg::operator uint32_t;
	static constexpr uint32_t USART_ICR = usart::USART_BASE + 0x08;
	static constexpr uint32_t MASK = 0b0110;

	usart_ic_reg(usart_is_reg& isr)
		: _isr(isr) {}

	using read_data_register_not_empty_bit = bits<1>;

	/**
	 * Transmission complete clear flag.
	 * Clears @ref usart_is_reg::transmission_complete_bit bit.
	 * @see transmission_complete
	 * @see set_transmission_complete
	 */
	using transmission_complete_bit = bits<2>;

	bool read_data_register_not_empty() {
		return self<read_data_register_not_empty_bit>();
	}

	void set_read_data_register_not_empty(bool flag) {
		self<read_data_register_not_empty_bit>() = flag;
	}

	bool transmission_complete() const {
		return self<transmission_complete_bit>();
	}

	void set_transmission_complete(bool flag) {
		self<transmission_complete_bit>() = flag;
		if(flag) {
			_isr.set_transmission_complete(false);
		}
	}

private:
	uint32_t get() const override {
		return _word & MASK;
	}

	void set(uint32_t word) override {
		_word = (word & MASK);
		if(_word & _isr.transmission_complete()) {
			_isr.set_transmission_complete(false);
		}
	}

	usart_is_reg& _isr;
};

/**
 * USART tx register
 */
class usart_tx_reg : public memory_mapped_reg {
public:
	using memory_mapped_reg::operator=;
	using memory_mapped_reg::operator uint32_t;
	static constexpr uint32_t USART_TX = usart::USART_BASE + 0x10;
	using callback_t = std::function<void(uint8_t op, uint8_t data)>;

	usart_tx_reg(usart_is_reg& isr, usart_cr1_reg& cr1, const callback_t& callback)
		: _isr(isr)
		, _cr1(cr1)
		, _callback(callback) {}


private:
	uint32_t get() const override {
		return 0;
	}

	void set(uint32_t word) override {

		if(!_cr1.enable()) {
			return;
		}

		// consume the byte to send
		bits<0, 8>::of(_word) = word;

		// clear txc and txe
		_isr.set_transmit_data_register_empty(false);
		_isr.set_transmission_complete(false);

		// transmit data
		uint8_t data = static_cast<uint8_t>(bits<0, 8>::of(_word));
		uint8_t op = static_cast<uint8_t>(bits<8, 8>::of(_word));
		if(_callback) {
			_callback(op, data);
		}

		// assume data is transmitted so clear the register
		bits<0, 8>::of(_word) = 0;

		// set interrupt if needed
		if(_cr1.tx_empty_interrupt_enable()) {
			_isr.set_transmit_data_register_empty(true);
		}
		if(_cr1.tx_complete_interrupt_enable()) {
			_isr.set_transmission_complete(true);
		};
	}

	usart_is_reg& _isr;
	usart_cr1_reg& _cr1;
	const callback_t& _callback;
};


/**
 * USART rx register
 */
class usart_rx_reg : public memory_mapped_reg {
public:
	using memory_mapped_reg::operator=;
	using memory_mapped_reg::operator uint32_t;
	using callback_t = std::function<void(uint8_t& data)>;
	static constexpr uint32_t USART_RX = usart::USART_BASE + 0x0c;

	usart_rx_reg(usart_is_reg& isr, usart_cr1_reg& cr1, callback_t& callback)
		: _isr(isr)
		, _cr1(cr1)
		, _callback(callback) {}


	void reset() override {
		set(0);
		// reset RXNE flag to skip the previous set
		_isr.set_read_data_register_not_empty(false);
	}

private:
	uint32_t get() const override {
		if(!_cr1.enable()) {
			return 0;
		}

		if(!_isr.read_data_register_not_empty()) {
			return 0;
		}

		uint32_t word = bits<0, 8>::of(_word);
		_isr.set_read_data_register_not_empty(false);

		if(_callback) {
			// set next data to _word
			// next get call will receive this value
			uint8_t next_data = 0;
			_callback(next_data);
		}

		return word;
	}


	void set(uint32_t word) override {
//		if(!_cr1.enable()) {
//			return;
//		}

		if(_isr.read_data_register_not_empty()) {
			return;
		}

		bits<0, 8>::of(_word) = word;
//		if(_cr1.rx_not_empty_interrupt_enable()) {
		_isr.set_read_data_register_not_empty(true);
//		}
	}

	usart_is_reg& _isr;
	usart_cr1_reg& _cr1;
	const callback_t& _callback;
};
}; //namespace micromachine::system

#endif // MICROMACHINE_USART_REG_HPP
