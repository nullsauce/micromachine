/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#pragma once

#include "exception/exception.hpp"

namespace micromachine::system {

class exception_state {
private:
	bool _active;
	const exception::Type _number;

public:
	exception_state(exception::Type number)
		: _active(false)
		, _number(number) {}

	virtual exception::priority_t priority() const = 0;
	virtual void set_priority(exception::priority_t priority) = 0;
	virtual void set_pending(bool pending) = 0;
	virtual bool is_pending() const = 0;
	virtual bool is_enabled() const = 0;
	virtual void set_enable(bool enable) = 0;

	exception::Type number() const {
		return _number;
	}

	bool is_active() const {
		return _active;
	}

	void activate() {
		_active = true;
		set_pending(false);
	}

	void deactivate() {
		_active = false;
	}

	void reset() {
		set_pending(false);
		deactivate();
		set_enable(false);
	}

	void copy_state_from(const exception_state& other) {
		set_active(other.is_active());
		set_priority(other.priority());
		set_pending(other.is_pending());
		set_enable(other.is_enabled());
	}

	void set_active(bool active) {
		_active = active;
	}
};

} // namespace micromachine::system
