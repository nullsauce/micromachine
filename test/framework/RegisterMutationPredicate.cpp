/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#include "RegisterMutationPredicate.hpp"
#include <gtest/gtest.h>

RegisterMutationPredicate::RegisterMutationPredicate(const cpu& previous, const cpu& current, reg_idx regIdx)
	: CpuMutationPredicate(previous, current)
	, _regIdx(regIdx) {}

RegisterMutationPredicate& RegisterMutationPredicate::Equals(uint32_t value) {
	EXPECT_EQ(value, currentRegisterValue());
	return *this;
}

RegisterMutationPredicate& RegisterMutationPredicate::DidNotChange() {
	EXPECT_EQ(previousRegisterValue(), currentRegisterValue());
	return *this;
}

RegisterMutationPredicate& RegisterMutationPredicate::Changed() {
	EXPECT_NE(previousRegisterValue(), currentRegisterValue());
	return *this;
}
