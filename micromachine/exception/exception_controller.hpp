/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#pragma once

#include "exception/exception.hpp"
#include "exception/exception_state.hpp"

namespace micromachine::system {

class exception_controller {
public:
	virtual bool is_pending(exception::Type ex) const = 0;
	virtual bool is_enabled(exception::Type ex) const = 0;
	virtual bool is_active(exception::Type ex) const = 0;
	virtual exception::priority_t priority(exception::Type ex) const = 0;
	virtual void set_pending(exception::Type ex, bool pending) = 0;
	virtual void set_enable(exception::Type ex, bool active) = 0;
	virtual void set_active(exception::Type ex, bool active) = 0;
	virtual void set_priority(exception::Type ex, exception::priority_t priority) = 0;
	virtual size_t active_count() const = 0;
	virtual bool any_active() const = 0;
	virtual bool any_pending() const = 0;
	virtual exception_state* top_pending() = 0;
	virtual const exception_state* top_pending() const = 0;
	virtual size_t highest_accepted_exception_number() const = 0;
	virtual size_t supported_exception_count() const = 0;
	virtual exception::priority_t current_execution_priority(bool primask_pm) const = 0;
	virtual void reset() = 0;
	virtual void raise_nmi() = 0;
	virtual void raise_memory_hardfault() = 0;
	virtual void raise_hardfault() = 0;
	virtual void raise_svcall() = 0;
	virtual void raise_pendsv() = 0;
	virtual void raise_systick() = 0;
	virtual void raise_external_interrupt(exception::Type ex) = 0;
};
} // namespace micromachine::system
