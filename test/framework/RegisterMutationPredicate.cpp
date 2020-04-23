/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#include "RegisterMutationPredicate.hpp"
#include "cpu.hpp"
#include <gtest/gtest.h>

RegisterMutationPredicate::RegisterMutationPredicate(const micromachine::system::cpu& previous, const micromachine::system::cpu& current, reg_idx regIdx)
	: CpuMutationPredicate(previous, current)
	, _regIdx(regIdx)
{}

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

RegisterMutationPredicate& RegisterMutationPredicate::WasIncrementedBy(uint32_t amount) {
	uint32_t expectedValue = previousRegisterValue() + amount;
	return Equals(expectedValue);
}
