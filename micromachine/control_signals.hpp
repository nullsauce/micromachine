#pragma once

#include "utils/signal.hpp"

namespace micromachine::system {

struct control_signals {
	signal halt;
	signal enter_low_power;
	signal reset;
};

}
