/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#pragma once

#include "exception.hpp"
#include "exception_vector.hpp"

namespace micromachine::system {

class exception_controller {
private:
	exception_vector& _exception_vector;

public:
	exception_controller(exception_vector& exception_vector)
		: _exception_vector(exception_vector) {}

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

	void raise_external_interrupt(exception::Type ex) {
		assert(ex >= exception::Type::EXTI_00 && "Can't raise an interrupt lower than EXTI_00");
		_exception_vector.interrupt_state(exception::EXTI_00).set_pending(true);
	}

	template <exception::Type Ex>
	void raise_external_interrupt() {
		static_assert(Ex >= exception::EXTI_00, "Can't raise an interrupt lower than EXTI_00");
		_exception_vector.interrupt_state<Ex>().set_pending(true);
	}

	template <exception::Type Ex>
	void disable_external_interrupt() {
		static_assert(Ex >= exception::EXTI_00, "Can't raise an interrupt lower than EXTI_00");
		_exception_vector.interrupt_state<Ex>().set_enable(false);
	}

	template <exception::Type Ex>
	void enable_external_interrupt() {
		static_assert(Ex >= exception::EXTI_00, "Can't raise an interrupt lower than EXTI_00");
		_exception_vector.interrupt_state<Ex>().set_enable(true);
	}

	void enable_external_interrupt(exception::Type ex) {
		assert(ex >= exception::Type::EXTI_00 && "Can't raise an interrupt lower than EXTI_00");
		_exception_vector.interrupt_state(exception::EXTI_00).set_enable(true);
	}

	template <exception::Type Ex>
	void set_exception_priority(exception::priority_t priority) {
		_exception_vector.interrupt_state<Ex>().set_priority(priority);
	}

	template <exception::Type Ex>
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
