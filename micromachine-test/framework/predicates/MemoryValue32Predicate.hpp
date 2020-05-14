//
// Created by fla on 25.04.20.
//

#pragma once


#include "MemoryValuePredicate.hpp"
#include "mcu_foward_decl.hpp"

#include <gtest/gtest.h>

#include <string>
#include <types/types.hpp>

namespace micromachine::testing {

class MemoryValue32Predicate : public MemoryValuePredicate {
protected:
	const uint32_t _expectedValue;

public:
	MemoryValue32Predicate(uint32_t address, uint32_t value);
	void apply(micromachine::system::mcu& expected);
	void check(const micromachine::system::mcu& actual) const;

private:
	static ::testing::AssertionResult assertMemoryEquality(const char*,
														   const char*,
														   const char*,
														   uint32_t actualValue,
														   uint32_t expectedValue,
														   uint32_t address);

};

} // namespace micromachine::testing
