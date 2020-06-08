//


//

#pragma once

#include "ExceptionStatePredicate.hpp"
#include "MemoryValue32Predicate.hpp"
#include "PredicateAggregate.hpp"
#include "mcu_foward_decl.hpp"

#include <gtest/gtest.h>

namespace micromachine::testing {

class RegistersPushedStatePredicate {
public:
	struct StackedValues {
		uint32_t r0;
		uint32_t r1;
		uint32_t r2;
		uint32_t r3;
		uint32_t r12;
		uint32_t lr;
		uint32_t returnAddress;
		uint32_t xpsrStatus;
	};

private:
	const StackedValues _expectedValues;
	uint32_t _savedFramePtr;
	static constexpr size_t _stackFrameSize = 32;
	static constexpr uint32_t _spMask = ~((uint32_t)0b100);

public:
	RegistersPushedStatePredicate(StackedValues expectedValues);
	void apply(micromachine::system::mcu& expected);
	void check(const micromachine::system::mcu& actual) const;

private:
	uint32_t craftXPSRValue() const;
	uint32_t calculateAlignedFramePtr(const micromachine::system::mcu& actual) const;
	static ::testing::AssertionResult assertStackedValue(const char*,
														 const char*,
														 const char*,
														 const char*,
														 uint32_t expectedValue,
														 uint32_t actualValue,
														 const char* itemName,
														 const char* itemPostionIndicator);
};

} // namespace micromachine::testing
