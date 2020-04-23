/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#include "mcu.hpp"
#include "cpu.hpp"

namespace micromachine::system {

cpu::step_result mcu::step() {
	_systick.tick();
	return _cpu.step();
}

}
