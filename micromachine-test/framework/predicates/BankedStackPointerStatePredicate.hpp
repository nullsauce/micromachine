//
// Created by fla on 26.04.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#pragma once

#include "ValueFormaters.hpp"
#include "mcu.hpp"

#include <gtest/gtest.h>

namespace micromachine::testing {

class BankedStackPointerStatePredicate {
private:
	const uint32_t _expectedMainSP;
	const uint32_t _expectedProcessSP;

public:
	BankedStackPointerStatePredicate(uint32_t expectedMainSP, uint32_t expectedProcessSP)
		: _expectedMainSP(expectedMainSP)
		, _expectedProcessSP(expectedProcessSP) {}

	BankedStackPointerStatePredicate(const mcu& expected)
		: BankedStackPointerStatePredicate(getMainSPFrom(expected), getProcessSPFrom(expected))
			 {}

	void apply(mcu& expected) {
		expected.get_cpu().regs().sp().set_specific_banked_sp(sp_reg::StackType::Main, _expectedMainSP);
		expected.get_cpu().regs().sp().set_specific_banked_sp(sp_reg::StackType::Process, _expectedProcessSP);
	}

	void check(const mcu& actual) const {
		EXPECT_PRED_FORMAT3(assertEquality, _expectedMainSP, getMainSPFrom(actual), "SP.Main");
		EXPECT_PRED_FORMAT3(assertEquality, _expectedProcessSP, getProcessSPFrom(actual), "SP.Process");
	}

private:

	uint32_t getSPFrom(const mcu& mcu, sp_reg::StackType stackType) const {
		return mcu.get_cpu().regs().sp().get_specific_banked_sp(stackType);
	}

	uint32_t getMainSPFrom(const mcu& mcu) const {
		return getSPFrom(mcu, sp_reg::StackType::Main);
	}

	uint32_t getProcessSPFrom(const mcu& mcu) const {
		return getSPFrom(mcu, sp_reg::StackType::Process);
	}

	::testing::AssertionResult assertEquality(const char*,
											  const char*,
											  const char*,
											  uint32_t expectedSPValue,
											  uint32_t actualSPValue,
											  const char* spName) const {

		if(expectedSPValue == actualSPValue) {
			return ::testing::AssertionSuccess();
		}

		return ::testing::AssertionFailure()
			   << "Equality check fail for banked SP value of " << spName << std::endl
			   << " * Expected: " << spName<< " value of " << uint32ToStr(expectedSPValue) << std::endl
			   << " * Actual  : " << spName<< " value is " << uint32ToStr(actualSPValue) << std::endl;
	}
};

} // namespace micromachine::testing
