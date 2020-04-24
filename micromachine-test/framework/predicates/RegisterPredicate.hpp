//
// Created by fla on 25.04.20.
//

#pragma once

#include "types/types.hpp"
#include "mcu.hpp"

#include <gtest/gtest.h>

#include <string>


namespace micromachine::testing {

class RegisterPredicate {
protected:
	std::string registerName(reg_idx regIdx) const {
		static const char* names[] = {
			"R0",
			"R1",
			"R2",
			"R3",
			"R4",
			"R5",
			"R6",
			"R7",
			"R8",
			"R9",
			"R10",
			"R11",
			"R12",
			"SP (R13)",
			"LR (R14)",
			"PC (R15)",
		};
		return names[regIdx & 0xf];
	}


};

}
