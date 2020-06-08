

#include "mcu.hpp"
#include "cpu.hpp"

namespace micromachine::system {

mcu::step_result mcu::step() {
	_systick.step();
	_usart_controller.step();
	_cpu.step();

	if(_control_signals.halt) {
		return step_result::HALT;
	} else if(_control_signals.reset) {
		reset();
		return step_result::OK;
	} else {
		return step_result::OK;
	}
}

}
