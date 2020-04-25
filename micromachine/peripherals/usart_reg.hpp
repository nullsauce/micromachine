//
// Created by joel on 15/04/2020.
//

#ifndef MICROMACHINE_USART_REG_HPP
#define MICROMACHINE_USART_REG_HPP

#include <memory>
#include "registers/memory_mapped_reg.hpp"

namespace micromachine::system {

namespace usart {
	static constexpr uint32_t USART_BASE = 0x40000000;
}

class usart_cr1_reg;

class usart_is_reg;

class usart_ic_reg;

class usart_tx_reg;

class usart_rx_reg;


class iusart_reg :public memory_mapped_reg{
public:
	using memory_mapped_reg::operator=;
	using memory_mapped_reg::operator uint32_t;

	/**
	 * Return the underlying word.
	 * @details the virtual machine (vm) should use this function instead of memory_mapped_reg::operator uint32_t
	 */
	virtual uint32_t read() = 0;

	/**
	 * Write value to the underlying word.
	 * @details the virtual machine (vm) should use this function instead of memory_mapped_reg::operator=
	 */
	virtual void write(uint32_t word) = 0;
};

/**
 * Usart Interrupt Status register
 */
class usart_is_reg : public iusart_reg {
public:
	using iusart_reg::operator=;
	using iusart_reg::operator uint32_t;

	static constexpr uint32_t USART_ISR = usart::USART_BASE + 0x04;
	static constexpr uint32_t MASK = 0b1110;

	/**
	* This bit is set by hardware when the data has been transferred to the usart_rx register. It
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
	 * This bit is set by hardware if the transmission of a frame containing data is complete
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
		return self<tx_empty_bit>();
	}

	void set_tx_empty(bool flag) {
		self<tx_empty_bit>() = flag;
	}

	bool tx_complete() {
		return self<tx_complete_bit>();
	}

	void set_tx_complete(bool flag) {
		self<tx_complete_bit>() = flag;
	}

	bool rx_not_empty() {
		return self<rx_not_empty_bit>();
	}

	void set_rx_not_empty(bool flag) {
		self<rx_not_empty_bit>() = flag;
	}

	uint32_t read() override {
		return get();
	}

	void write(uint32_t word) override {
		set(word);
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
 * Control register 1
 */
class usart_cr1_reg : public iusart_reg {
public:
	using iusart_reg::operator=;
	using iusart_reg::operator uint32_t;
	static constexpr uint32_t USART_CR1 = usart::USART_BASE + 0x00;
	static constexpr uint32_t MASK = 0b1111;

	usart_cr1_reg(usart_is_reg& isr) : _isr(isr){}
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
	 * 1: A USART interrupt is generated whenever @ref tx_empty_bit = 1 in the
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

	uint32_t read() override {
		return get();
	}

	void write(uint32_t word) override {
		set(word);
	}

private:
	uint32_t get() const override {
		return _word & MASK;
	}

	void set(uint32_t word) override {
		_word = (word & MASK);
		/*
		 * Ensure that the interrupt is set or cleared depending of txe/txc option bits in cr1.
		 */
		_isr.set_tx_empty(tx_empty_interrupt_enable());
		_isr.set_tx_complete(tx_complete_interrupt_enable());
	}

	usart_is_reg& _isr;
};

/**
 * USART interrupt clear register
 */
class usart_ic_reg : public iusart_reg {
public:
	using iusart_reg::operator=;
	using iusart_reg::operator uint32_t;
	static constexpr uint32_t USART_ICR = usart::USART_BASE + 0x08;
	static constexpr uint32_t MASK = 0b0110;

	usart_ic_reg(usart_is_reg& isr)
		: _isr(isr) {}

	using rx_not_empty_bit = bits<1>;

	/**
	 * Transmission complete clear flag.
	 * Clears @ref usart_is_reg::tx_complete_bit bit.
	 * @see tx_complete
	 * @see set_tx_complete
	 */
	using tx_complete_bit = bits<2>;

	bool rx_not_empty() {
		return self<rx_not_empty_bit>();
	}

	void set_rx_not_empty(bool flag) {
		self<rx_not_empty_bit>() = flag;
	}

	bool tx_complete() const {
		return self<tx_complete_bit>();
	}

	void set_tx_complete(bool flag) {
		self<tx_complete_bit>() = flag;
		if(flag) {
			_isr.set_tx_complete(false);
		}
	}

	uint32_t read() override {
		return get();
	}

	void write(uint32_t word) override {
		set(word);
	}

private:
	uint32_t get() const override {
		return _word & MASK;
	}

	void set(uint32_t word) override {
		_word = (word & MASK);
		if(_word & _isr.tx_complete()) {
			_isr.set_tx_complete(false);
		}
	}

	usart_is_reg& _isr;
};

/**
 * USART tx register
 */
class usart_tx_reg : public iusart_reg {
public:
	using iusart_reg::operator=;
	using iusart_reg::operator uint32_t;
	static constexpr uint32_t USART_TX = usart::USART_BASE + 0x10;
	using callback_t = std::function<void(uint8_t op, uint8_t data)>;

	usart_tx_reg(usart_is_reg& isr, usart_cr1_reg& cr1, const callback_t& callback)
		: _isr(isr)
		, _cr1(cr1)
		, _callback(callback) {}

	uint32_t read() override {
		return 0;
	}

	void write(uint32_t word) override {
		set(word);
	}

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
		_isr.set_tx_empty(false);
		_isr.set_tx_complete(false);

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
			_isr.set_tx_empty(true);
		}
		if(_cr1.tx_complete_interrupt_enable()) {
			_isr.set_tx_complete(true);
		}
	}

	usart_is_reg& _isr;
	usart_cr1_reg& _cr1;
	const callback_t& _callback;
};


/**
 * USART rx register
 */
class usart_rx_reg : public iusart_reg {
public:
	using iusart_reg::operator=;
	using iusart_reg::operator uint32_t;
	using callback_t = std::function<void(uint8_t& data)>;
	static constexpr uint32_t USART_RX = usart::USART_BASE + 0x0c;

	usart_rx_reg(usart_is_reg& isr, usart_cr1_reg& cr1, const callback_t& callback)
		: _isr(isr)
		, _cr1(cr1)
		, _callback(callback) {}


	void reset() override {
		set(0);
		// reset RXNE flag to skip the previous set
		_isr.set_rx_not_empty(false);
	}

	uint32_t read() override {
		return get();
	}
	void write(uint32_t word) override {
		set(word);
	}

private:
	uint32_t get() const override {
		if(!_cr1.enable()) {
			return 0;
		}

		if(!_isr.rx_not_empty()) {
			return 0;
		}

		uint32_t word = bits<0, 8>::of(_word);
		_isr.set_rx_not_empty(false);

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

		if(_isr.rx_not_empty()) {
			return;
		}

		bits<0, 8>::of(_word) = word;
//		if(_cr1.rx_not_empty_interrupt_enable()) {
		_isr.set_rx_not_empty(true);
//		}
	}

	usart_is_reg& _isr;
	usart_cr1_reg& _cr1;
	const callback_t& _callback;
};
}; //namespace micromachine::system

#endif // MICROMACHINE_USART_REG_HPP
