/* Copyright (C) MindMaze SA - 2016 - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of MindMaze SA.

*/

#ifndef MICROMACHINE_EMU_INTERRUPTOR_HPP
#define MICROMACHINE_EMU_INTERRUPTOR_HPP

#include "exception_state.hpp"

class interrupter {
private:
	ExceptionStateVector& _exception_vector;

	template<Exception::Type Ex>
	void raise() {
		_exception_vector.interrupt_state<Ex>()->set_pending();
	}

public:

	interrupter(ExceptionStateVector& exception_vector);

	void raise_nmi() {
		_exception_vector.interrupt_state(Exception::Type::NMI)->set_pending();
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

	void raise_systick() {
		raise<Exception::Type::SYSTICK>();
	}

};

#endif //MICROMACHINE_EMU_INTERRUPTOR_HPP
