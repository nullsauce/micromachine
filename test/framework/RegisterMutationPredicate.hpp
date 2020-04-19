/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_REGISTERMUTATIONPREDICATE_HPP
#define MICROMACHINE_REGISTERMUTATIONPREDICATE_HPP


#include "CpuMutationPredicate.hpp"

#include <gtest/gtest.h>

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
	RegisterMutationPredicate(const cpu& previous, const cpu& current, reg_idx regIdx)
	 : CpuMutationPredicate(previous, current)
	 , _regIdx(regIdx)
	{}

	RegisterMutationPredicate& Equals(uint32_t value) {
		EXPECT_EQ(value, currentRegisterValue());
		return *this;
	}

	RegisterMutationPredicate& DidNotChange() {
		EXPECT_EQ(previousRegisterValue(), currentRegisterValue());
		return *this;
	}

	RegisterMutationPredicate& Changed() {
		EXPECT_NE(previousRegisterValue(), currentRegisterValue());
		return *this;
	}
};



#endif //MICROMACHINE_REGISTERMUTATIONPREDICATE_HPP
