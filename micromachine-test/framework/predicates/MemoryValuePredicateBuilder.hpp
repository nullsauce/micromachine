//

//

#pragma once

#include "MemoryValue32Predicate.hpp"

namespace micromachine::testing {

class MemoryValuePredicateBuilder : public MemoryValuePredicate {
public:
	MemoryValue32Predicate equals32(uint32_t value) {
		return {_address, value};
	}
};

} // namespace micromachine::testing
