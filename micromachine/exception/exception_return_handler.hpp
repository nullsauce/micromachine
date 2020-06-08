

#pragma once

#include <cstdint>

namespace micromachine::system {

struct exception_return_handler {
	virtual void exception_return(uint32_t ret_address) = 0;
};

} // namespace micromachine::system
