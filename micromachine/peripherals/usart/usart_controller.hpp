//
// Copyright (C) 2020 Joel Jungo, Flavio Roth - All Rights Reserved
//

#ifndef MICROMACHINE_USART_CONTROLLER_HPP
#define MICROMACHINE_USART_CONTROLLER_HPP

#include "exception/exception_controller.hpp"
#include "peripherals/iodev.hpp"
#include "peripherals/usart/usart_reg.hpp"
#include "utils/blocking_queue.hpp"

#include <deque>
#include <memory>
#include <thread>

namespace micromachine::system {

class usart_controller : public iodev {
private:
	using data_channel = blocking_bounded_channel<uint8_t, 1024>;

	bool _interrupt_event;
	exception_controller& _exception_controller;
	usart_is_reg _interrupt_status_register;
	usart_ic_reg _interrupt_clear_register;
	usart_cr1_reg _control_register;
	usart_rx_reg _rx_register;
	usart_tx_reg _tx_register;
	data_channel _rx_buffer;
	data_channel _tx_buffer;
	const exception _external_interrupt;


public:
	static constexpr uint32_t USART_BASE = 0x40000000;
	static constexpr uint32_t USART_CR1 = USART_BASE + 0x00;
	static constexpr uint32_t USART_ISR = USART_BASE + 0x04;
	static constexpr uint32_t USART_ICR = USART_BASE + 0x08;
	static constexpr uint32_t USART_RX = USART_BASE + 0x0c;
	static constexpr uint32_t USART_TX = USART_BASE + 0x10;

	usart_controller(exception_controller& exception_controller, exception external_interrupt)
		: _interrupt_event(false)
		, _exception_controller(exception_controller)
		, _interrupt_status_register(_interrupt_event)
		, _interrupt_clear_register(_interrupt_status_register)
		, _control_register(_interrupt_event, std::bind(&usart_controller::reset, this))
		, _rx_register(_interrupt_status_register)
		, _tx_register(_interrupt_status_register)
		, _external_interrupt(external_interrupt)
	{}

	void reset() {
		clear_interrupt_event();
		_control_register.reset();
		_interrupt_status_register.reset();
		_interrupt_clear_register.reset();
		_rx_register.reset();
		_tx_register.reset();
	}

	void step() {

		if (!_control_register.enabled()) {
			return;
		}

		// do we have data to sent to the application ?
		if(_rx_buffer.size()) {
			// send it if the rx register is empty
			if(!_interrupt_status_register.rx_not_empty()) {
				// commit the data to the register
				uint8_t byte = 0;
				if(data_channel::success == _rx_buffer.pop(byte)) {
					_rx_register.write(byte);
				}
			}
		}

		// is there data to be read from the application ?
		if(!_interrupt_status_register.tx_empty()) {
			_tx_buffer.force_push(_tx_register.read());
		}

		if(_interrupt_event) {
			// check if any interrupt should be fired.

			bool should_interrupt_because_tx_empty = _interrupt_status_register.tx_empty() && _control_register.tx_empty_interrupt_enabled();
			bool should_interrupt_because_rx_not_empty = _interrupt_status_register.rx_not_empty() && _control_register.rx_not_empty_interrupt_enabled();
			bool should_interrupt = should_interrupt_because_tx_empty || should_interrupt_because_rx_not_empty;

			if(should_interrupt) {
				_exception_controller.raise_external_interrupt(_external_interrupt);
				clear_interrupt_event();
			}
		}
	}

	/**
	 * Send a byte to the MCU
	 * @param byte
	 */
	void send(uint8_t byte) override {
		_rx_buffer.force_push(byte);
	}

	/** Receieves a byte from the MCU
	 * @return
	 */
	bool receive(uint8_t& out) override {
		if(data_channel::success == _tx_buffer.pop(out)) {
			return true;
		} else {
			return false;
		}
	}

	size_t bytes_available() const override {
		return _tx_buffer.size();
	}

	void shutdown() override {
		_rx_buffer.abort();
		_tx_buffer.abort();
	}

	const usart_cr1_reg& control_register() const {
		return _control_register;
	}

	size_t rx_buffer_size() const {
		return _rx_buffer.size();
	}

	size_t tx_buffer_size() const {
		return _tx_buffer.size();
	}

	usart_cr1_reg& control_register() {
		return _control_register;
	}

	usart_is_reg& interrupt_status_register() {
		return _interrupt_status_register;
	}

	usart_ic_reg& interrupt_clear_register() {
		return _interrupt_clear_register;
	}

	usart_rx_reg& rx_register() {
		return _rx_register;
	}

	usart_tx_reg& tx_register() {
		return _tx_register;
	}

private:
	void clear_interrupt_event() {
		_interrupt_event = false;
	}

};
}; //namespace micromachine::system

#endif // MICROMACHINE_USART_CONTROLLER_HPP
