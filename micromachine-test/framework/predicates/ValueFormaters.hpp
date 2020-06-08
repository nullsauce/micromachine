#pragma once


#include "types/types.hpp"
#include <string>

namespace micromachine::testing {

	std::string uint8ToStr(uint8_t byte);
	std::string uint32ToStr(uint32_t value);
	std::string uint16ToStr(uint16_t value);
	std::string registerName(micromachine::system::reg_idx regIdx);

}
