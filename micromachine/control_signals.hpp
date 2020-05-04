
//
// Created by fla on 04.05.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#pragma once

#include "utils/signal.hpp"

namespace micromachine::system {

struct control_signals {
	signal halt;
	signal enter_low_power;
	signal reset;
};

}
