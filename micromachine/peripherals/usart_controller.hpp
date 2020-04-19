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
	usart_controller(usart_cr1_reg& control_register,
					 usart_is_reg& interrupt_status_register,
					 usart_ic_reg& interrupt_clear_register,
					 usart_rx_reg& rx_register,
					 usart_tx_reg& tx_register,
					 exception_controller& exception_controller)
		: _control_register(control_register)
		, _interrupt_status_register(interrupt_status_register)
		, _interrupt_clear_register(interrupt_clear_register)
		, _rx_register(rx_register)
		, _tx_register(tx_register)
		, _exception_controller(exception_controller) {}

	void run() {
		// TODO JJO totally wip

		if (!_control_register.enable()) {
			return;
		}

		// consume the current data
		uint32_t rx_data = _rx_register;

		// clear or set isr if any
		_interrupt_status_register.set_transmit_data_register_empty(
			_control_register.tx_empty_interrupt_enable());

		_interrupt_status_register.set_transmission_complete(
			_control_register.tx_complete_interrupt_enable()
		);

		// raise interrupt if any
		uint32_t isr = _interrupt_status_register;
		if(isr) {
			// TODO find a way to map EXTI_00 to usart:
			// https://github.com/flavioroth/micromachine/projects/1#card-36544071
			_exception_controller.raise_external_interrupt(0 /*exception::Type::EXTI_00*/);
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

private:
	usart_cr1_reg& _control_register;
	usart_is_reg& _interrupt_status_register;
	usart_ic_reg& _interrupt_clear_register;
	usart_rx_reg& _rx_register;
	usart_tx_reg& _tx_register;

	exception_controller& _exception_controller;
};
}; //namespace micromachine::system

#endif // MICROMACHINE_USART_CONTROLLER_HPP
