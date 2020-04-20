/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#include "MemoryMutationPredicate.hpp"
#include <gtest/gtest.h>

template<typename uint_type>
MemoryMutationPredicate<uint_type>::MemoryMutationPredicate(const cpu& previous, const cpu& current, uint32_t address)
	: CpuMutationPredicate(previous, current)
	, _address(address)
{}

template<typename uint_type>
MemoryMutationPredicate<uint_type>& MemoryMutationPredicate<uint_type>::Equals(uint32_t value) {
	EXPECT_EQ(value, currentValue());
	return *this;
}

template<typename uint_type>
MemoryMutationPredicate<uint_type>& MemoryMutationPredicate<uint_type>::DidNotChange() {
	EXPECT_EQ(previousValue(), currentValue());
	return *this;
}

template<typename uint_type>
MemoryMutationPredicate<uint_type>& MemoryMutationPredicate<uint_type>::Changed() {
	EXPECT_NE(previousValue(), currentValue());
	return *this;
}
