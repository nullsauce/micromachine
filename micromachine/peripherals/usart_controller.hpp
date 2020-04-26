//
// Created by joel on 17/04/2020.
//

#ifndef MICROMACHINE_USART_CONTROLLER_HPP
#define MICROMACHINE_USART_CONTROLLER_HPP
#include "exception/exception_controller.hpp"
#include "peripherals/usart_reg.hpp"
#include <memory>

namespace micromachine::system {

class usart_controller {
public:
	usart_controller(exception_controller& exception_controller,
					 const usart_rx_reg::callback_t& rx_callback,
					 const usart_tx_reg::callback_t& tx_callback
					 )
		: _control_register(_interrupt_status_register)
		, _interrupt_clear_register(_interrupt_status_register)
		, _rx_register(_interrupt_status_register, _control_register, rx_callback)
		, _tx_register(_interrupt_status_register, _control_register, tx_callback)
		, _exception_controller(exception_controller)
	{}

	void run() {

		if (!_control_register.enable()) {
			return;
		}

		uint32_t isr = _interrupt_status_register.read();
		uint32_t cr1 = _control_register.read();

		// make sure exception is raised only on changed rather than on each tick
		if (_old_cr1 != cr1 || _old_isr != isr) {

			// raise interrupt if any
			if(isr & cr1) {
				// TODO find a way to map EXTI_00 to usart:
				// https://github.com/flavioroth/micromachine/projects/1#card-36544071
				_exception_controller.raise_external_interrupt(0 /*exception::Type::EXTI_00*/);
			}
			_old_isr = isr;
			_old_cr1 = cr1;
		}
	}

	const usart_cr1_reg& control_register() const {
		return _control_register;
	}

	usart_cr1_reg& control_register() {
		return _control_register;
	}

	const usart_is_reg& interrupt_status_register() const {
		return _interrupt_status_register;
	}

	usart_is_reg& interrupt_status_register() {
		return _interrupt_status_register;
	}

	const usart_ic_reg& interrupt_clear_register() const {
		return _interrupt_clear_register;
	}

	usart_ic_reg& interrupt_clear_register() {
		return _interrupt_clear_register;
	}

	const usart_rx_reg& rx_register() const {
		return _rx_register;
	}

	usart_rx_reg& rx_register() {
		return _rx_register;
	}

	const usart_tx_reg& tx_register() const {
		return _tx_register;
	}

	usart_tx_reg& tx_register() {
		return _tx_register;
	}

	void reset() {
		_control_register.reset();
		_interrupt_status_register.reset();
		_interrupt_clear_register.reset();
		_rx_register.reset();
		_tx_register.reset();
	}

	void rx_push_data(uint8_t data) {
		_rx_register = data;
	}

private:
	usart_cr1_reg _control_register;
	usart_is_reg _interrupt_status_register;
	usart_ic_reg _interrupt_clear_register;
	usart_rx_reg _rx_register;
	usart_tx_reg _tx_register;
	exception_controller& _exception_controller;

	uint32_t _old_isr{0};
	uint32_t _old_cr1{0};
};
}; //namespace micromachine::system

#endif // MICROMACHINE_USART_CONTROLLER_HPP
