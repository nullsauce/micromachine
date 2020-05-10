/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#pragma once

#include "exception_controller.hpp"
#include "exception_vector.hpp"

namespace micromachine::system {

class exception_vector_controller : public exception_controller {
private:
	exception_vector _exception_vector;

	void raise(exception ex) {
		set_pending(ex, true);
	}

public:
	exception_vector_controller(nvic& nvic, shpr2_reg& shpr2, shpr3_reg& shpr3, interrupt_control_state_reg& icsr)
		: _exception_vector(nvic, shpr2, shpr3, icsr) {}

	exception_vector_controller(nvic& nvic,
						 shpr2_reg& shpr2,
						 shpr3_reg& shpr3,
						 interrupt_control_state_reg& icsr,
						 const exception_vector_controller& other)
		: _exception_vector(nvic, shpr2, shpr3, icsr, other._exception_vector) {}

	bool is_pending(exception ex) const override {
		return _exception_vector.interrupt_state(ex).is_pending();
	}

	bool is_enabled(exception ex) const override {
		return _exception_vector.interrupt_state(ex).is_enabled();
	}

	bool is_active(exception ex) const override {
		return _exception_vector.interrupt_state(ex).is_active();
	}

	exception::priority_t priority(exception ex) const override {
		return _exception_vector.interrupt_state(ex).priority();
	}

	void set_pending(exception ex, bool pending) override {
		return _exception_vector.interrupt_state(ex).set_pending(pending);
	}

	void set_enable(exception ex, bool active) override {
		return _exception_vector.interrupt_state(ex).set_enable(active);
	}

	void set_active(exception ex, bool active) override {
		return _exception_vector.interrupt_state(ex).set_active(active);
	}

	void set_priority(exception ex, exception::priority_t priority) override {
		_exception_vector.interrupt_state(ex).set_priority(priority);
	}

	size_t active_count() const override {
		return _exception_vector.active_count();
	}

	bool any_active() const override {
		return _exception_vector.any_active();
	}

	bool any_pending() const override {
		return _exception_vector.any_pending();
	}

	exception_state* top_pending() override {
		return _exception_vector.top_pending();
	}

	const exception_state* top_pending() const override {
		return _exception_vector.top_pending();
	}

	size_t highest_accepted_exception_number() const override {
		return _exception_vector.highest_accepted_exception_number();
	}

	size_t supported_exception_count() const override {
		return _exception_vector.supported_exception_count();
	}

	exception::priority_t current_execution_priority(bool primask_pm) const override {
		return _exception_vector.current_execution_priority(primask_pm);
	}

	void reset() override {
		_exception_vector.reset();
	}

	void raise_nmi() override {
		raise(exception::NMI);
	}

	void raise_memory_hardfault() override {
		raise(exception::HARDFAULT);
	}

	void raise_hardfault() override {
		raise(exception::HARDFAULT);
	}

	void raise_svcall() override {
		raise(exception::SVCALL);
	}

	void raise_pendsv() override {
		raise(exception::PENDSV);
	}

	void raise_systick() override {
		raise(exception::SYSTICK);
	}

	void raise_external_interrupt(exception ex) override {
		assert(ex >= exception::EXTI_00 && "Can't raise an interrupt lower than EXTI_00");
		_exception_vector.interrupt_state(ex).set_pending(true);
	}

};
} // namespace micromachine::system
