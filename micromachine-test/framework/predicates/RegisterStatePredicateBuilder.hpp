
//


//

#pragma once

#include "RegisterUnsignedIncrementPredicate.hpp"
#include "RegisterUnsignedDecrementPredicate.hpp"
#include "RegisterEqualityPredicate.hpp"
#include "RegistersPushedStatePredicate.hpp"

namespace micromachine::testing {

class RegisterStatePredicateBuilder {
private:
	const reg_idx _regIdx;

public:
	RegisterStatePredicateBuilder(reg_idx regIdx)
		: _regIdx(regIdx) {}

	RegisterEqualityPredicate equals(uint32_t value) {
		return {_regIdx, value};
	}

	RegisterUnsignedIncrementPredicate wasIncrementedBy(uint32_t amount) {
		return {_regIdx, amount};
	}

	RegisterUnsignedDecrementPredicate wasDecrementedBy(uint32_t amount) {
		return {_regIdx, amount};
	}
};

}
