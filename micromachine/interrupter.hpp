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
	exception_state_vector& _exception_vector;

	template<exception::Type Ex>
	void raise() {
		_exception_vector.interrupt_state<Ex>().set_pending();
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
		_exception_vector.interrupt_state(exception::Type::EXTI_00 + number).set_pending();
	}

};

#endif //MICROMACHINE_EMU_INTERRUPTOR_HPP
