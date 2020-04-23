/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#pragma once

namespace micromachine::system {

/** Used to signal event. Not accessible by software
* The Event Register is a single bit register for each processor
* in a multiprocessor system. When set, an Event Register
* indicates that an event has occurred, since the register was
* last cleared, that might prevent the processor having to suspend
* operation on issuing a WFE instruction. The following conditions
* apply to the Event Register
*/
class event_register {
private:
	bool _flag;

public:
	event_register()
		: _flag(false)
	{}

	void clear() {
		_flag = false;
	}

	void set() {
		_flag = true;
	}

	bool is_set() const {
		return  _flag;
	}

};

} // namespace micromachine::system
