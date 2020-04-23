/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_INTERRUPTOR_HPP
#define MICROMACHINE_EMU_INTERRUPTOR_HPP

#include "exception_defs.hpp"
#include "exception_vector.hpp"

namespace micromachine::system {

class exception_controller {
private:
	exception_vector& _exception_vector;

public:
	exception_controller(exception_vector& exception_vector)
		: _exception_vector(exception_vector)
	{}

	void raise_nmi() {
		raise<exception::NMI>();
	}

	void raise_memory_hardfault() {
		raise<exception::HARDFAULT>();
	}

	void raise_hardfault() {
		raise<exception::HARDFAULT>();
	}

	void raise_svcall() {
		raise<exception::SVCALL>();
	}

	void raise_pendsv() {
		raise<exception::PENDSV>();
	}

	void raise_systick() {
		raise<exception::SYSTICK>();
	}

	void raise_external_interrupt(uint8_t number) {
		_exception_vector.interrupt_state(exception::EXTI_00 + number).set_pending(true);
	}

	void enable_external_interrupt(uint8_t number) {
		_exception_vector.interrupt_state(exception::EXTI_00 + number).set_enable(true);
	}

	void disable_external_interrupt(uint8_t number) {
		_exception_vector.interrupt_state(exception::EXTI_00 + number).set_enable(false);
	}

	template<exception::Type Ex>
	void set_exception_priority(exception::priority_t priority) {
		_exception_vector.interrupt_state<Ex>().set_priority(priority);
	}

	template<exception::Type Ex>
	exception::priority_t exception_priority() {
		return _exception_vector.interrupt_state<Ex>().priority();
	}

private:
	template <exception::Type Ex>
	void raise() {
		_exception_vector.interrupt_state<Ex>().set_pending(true);
	}

};
} // namespace micromachine::system

#endif //MICROMACHINE_EMU_INTERRUPTOR_HPP
