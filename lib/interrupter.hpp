/* Copyright (C) The Micromachine project - 2016 - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of The Micromachine project.

*/

#ifndef MICROMACHINE_EMU_INTERRUPTOR_HPP
#define MICROMACHINE_EMU_INTERRUPTOR_HPP

#include "exception_state.hpp"

class interrupter {
private:
	ExceptionStateVector& _exception_vector;

	template<Exception::Type Ex>
	void raise() {
		_exception_vector.interrupt_state<Ex>().set_pending();
	}

public:

	interrupter(ExceptionStateVector& exception_vector);

	void raise_nmi() {
		raise<Exception::Type::NMI>();
	}

	void raise_memory_hardfault() {
		raise<Exception::Type::HARDFAULT>();
	}

	void raise_hardfault() {
		raise<Exception::Type::HARDFAULT>();
	}

	void raise_svcall() {
		raise<Exception::Type::SVCALL>();
	}

	void raise_pendsv() {
		raise<Exception::Type::PENDSV>();
	}

	void raise_systick() {
		raise<Exception::Type::SYSTICK>();
	}

	void raise_external_interrupt(uint8_t number) {
		_exception_vector.interrupt_state(Exception::Type::EXTI_00 + number).set_pending();;
	}

};

#endif //MICROMACHINE_EMU_INTERRUPTOR_HPP
