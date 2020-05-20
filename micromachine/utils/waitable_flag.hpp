
//
// Created by fla on 04.05.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#pragma once

#include "waitable_value.hpp"

namespace micromachine::system {

/**
 * A waitable flag is a waitable boolean value on which
 * threads can wait for it to become true or false.
 * Initial value can be either true or false.
 */
class waitable_flag : public waitable_value<bool> {
	using waitable_value::waitable_value;
};

}
