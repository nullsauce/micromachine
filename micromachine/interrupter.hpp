/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_INTERRUPTOR_HPP
#define MICROMACHINE_EMU_INTERRUPTOR_HPP

#include "exception_state.hpp"

class interrupter {
private:
	exception_state_vector& _exception_vector;

	template<exception::Type Ex>
	void raise() {
		_exception_vector.interrupt_state<Ex>().set_pending(true);
	}

public:

	interrupter(exception_state_vector& exception_vector);

	void raise_nmi() {
		raise<exception::Type::NMI>();
	}

	void raise_memory_hardfault() {
		raise<exception::Type::HARDFAULT>();
	}

	void raise_hardfault() {
		raise<exception::Type::HARDFAULT>();
	}

	void raise_svcall() {
		raise<exception::Type::SVCALL>();
	}

	void raise_pendsv() {
		raise<exception::Type::PENDSV>();
	}

	void raise_systick() {
		raise<exception::Type::SYSTICK>();
	}

	void raise_external_interrupt(uint8_t number) {
		_exception_vector.interrupt_state(exception::Type::EXTI_00 + number).set_pending(true);
	}

	void enable_external_interrupt(uint8_t number) {
		_exception_vector.interrupt_state(exception::Type::EXTI_00 + number).set_enable(true);
	}

	void disable_external_interrupt(uint8_t number) {
		_exception_vector.interrupt_state(exception::Type::EXTI_00 + number).set_enable(false);
	}

	template<exception::Type Ex>
	void set_exception_priority(exception::priority_t priority) {
		_exception_vector.interrupt_state<Ex>().set_priority(priority);
	}

	template<exception::Type Ex>
	exception::priority_t exception_priority() {
		return _exception_vector.interrupt_state<Ex>().priority();
	}

};

#endif //MICROMACHINE_EMU_INTERRUPTOR_HPP
