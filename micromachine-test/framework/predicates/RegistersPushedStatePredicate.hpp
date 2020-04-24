//
// Created by fla on 26.04.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#pragma once

#include "ExceptionStatePredicate.hpp"
#include "MemoryValue32Predicate.hpp"
#include "PredicateAggregate.hpp"
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
	RegistersPushedStatePredicate(StackedValues expectedValues)
		: _expectedValues(expectedValues)
		, _savedFramePtr(0)
	{}

	void apply(mcu& expected) {

		_savedFramePtr = calculateAlignedFramePtr(expected);

		PredicateAggregate(
			MemoryValue32Predicate(_savedFramePtr + 0, _expectedValues.r0),
			MemoryValue32Predicate(_savedFramePtr + 4, _expectedValues.r1),
			MemoryValue32Predicate(_savedFramePtr + 8, _expectedValues.r2),
			MemoryValue32Predicate(_savedFramePtr + 12, _expectedValues.r3),
			MemoryValue32Predicate(_savedFramePtr + 16, _expectedValues.r12),
			MemoryValue32Predicate(_savedFramePtr + 20, _expectedValues.lr),
			MemoryValue32Predicate(_savedFramePtr + 24, _expectedValues.returnAddress),
			MemoryValue32Predicate(_savedFramePtr + 28, craftXPSRValue())
		).apply(expected);
	}

	void check(const mcu& actual) const {
		const memory& mem = actual.get_memory();

		assert(_savedFramePtr != 0 && "_savedFramePtr is zero. Did you call apply() before running this check?");

		EXPECT_PRED_FORMAT4(assertStackedValue, _expectedValues.r0, mem.read32_unchecked(_savedFramePtr + 0), "R0", "(stack frame + 0)");
		EXPECT_PRED_FORMAT4(assertStackedValue, _expectedValues.r1, mem.read32_unchecked(_savedFramePtr + 4), "R1", "(stack frame + 4)");
		EXPECT_PRED_FORMAT4(assertStackedValue, _expectedValues.r2, mem.read32_unchecked(_savedFramePtr + 8), "R2", "(stack frame + 8)");
		EXPECT_PRED_FORMAT4(assertStackedValue, _expectedValues.r3, mem.read32_unchecked(_savedFramePtr + 12), "R3", "(stack frame + 12)");
		EXPECT_PRED_FORMAT4(assertStackedValue, _expectedValues.r12, mem.read32_unchecked(_savedFramePtr + 16), "R12", "(stack frame + 16)");
		EXPECT_PRED_FORMAT4(assertStackedValue,_expectedValues.lr, mem.read32_unchecked(_savedFramePtr + 20), "LR", "(stack frame + 20)");
		EXPECT_PRED_FORMAT4(assertStackedValue, _expectedValues.returnAddress, mem.read32_unchecked(_savedFramePtr + 24), "return address", "(stack frame + 24)");
		EXPECT_PRED_FORMAT4(assertStackedValue,craftXPSRValue(), mem.read32_unchecked(_savedFramePtr + 28), "XPSR", "(stack frame + 28)");

	}

private:

	uint32_t craftXPSRValue() const {
		const bool framePtrAlign = bits<2>::of<uint32_t>(_expectedValues.xpsrStatus);
		uint32_t savedXPSRValue = _expectedValues.xpsrStatus;
		bits<8>::of(savedXPSRValue) = (bool)(framePtrAlign & 1);
		return savedXPSRValue;
	}

	uint32_t calculateAlignedFramePtr(const mcu& actual) const {
		return (actual.get_cpu().regs().sp() - _stackFrameSize) & _spMask;
	}

	static ::testing::AssertionResult assertStackedValue(const char*,
														  const char*,
														  const char*,
														  const char*,
														  uint32_t expectedValue,
														  uint32_t actualValue,
														  const char* itemName,
														  const char* itemPostionIndicator) {

		if(expectedValue == actualValue) {
			return ::testing::AssertionSuccess();
		}

		return ::testing::AssertionFailure()
			   << "Equality check fail for stacked value of " << itemName << std::endl
			   << "At " << itemPostionIndicator << " :" << std::endl
			   << " * Expected: "<< itemName << " value of " << uint32ToStr(expectedValue) << std::endl
			   << " * Actual  : "<< itemName << " value is " << uint32ToStr(actualValue) << std::endl;
	}

};

}
