#pragma once

namespace micromachine::testing {

class MemoryValuePredicate {
protected:
	const uint32_t _address;
public:
	MemoryValuePredicate(uint32_t address)
		: _address(address) {}
};

} // namespace micromachine::testing
