/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#pragma once


#include "CpuMutationPredicate.hpp"
#include "cpu.hpp"

class RegisterMutationPredicate : public CpuMutationPredicate {
private:
	const reg_idx _regIdx;

	uint32_t currentRegisterValue() {
		return _current.regs().get(_regIdx);
	}

	uint32_t previousRegisterValue() {
		return _previous.regs().get(_regIdx);
	}

public:
	RegisterMutationPredicate(const micromachine::system::cpu& previous, const micromachine::system::cpu& current, reg_idx regIdx);
	RegisterMutationPredicate& Equals(uint32_t value);
	RegisterMutationPredicate& DidNotChange();
	RegisterMutationPredicate& Changed();
	RegisterMutationPredicate& WasIncrementedBy(uint32_t amount);
};



