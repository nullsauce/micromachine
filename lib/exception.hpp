//
// Created by fla on 17-5-20.
//

#ifndef MICROMACHINE_EXCEPTION_HPP
#define MICROMACHINE_EXCEPTION_HPP

#include <cstddef>

enum class exception : size_t {
	INVALID = 0,
	RESET,
	NMI,
	HARDFAULT
};

#endif //MICROMACHINE_EXCEPTION_HPP
