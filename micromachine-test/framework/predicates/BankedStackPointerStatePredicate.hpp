//


//

#pragma once

#include "ValueFormaters.hpp"
#include "mcu_foward_decl.hpp"

#include <gtest/gtest.h>

namespace micromachine::testing {

class BankedStackPointerStatePredicate {
private:
	const uint32_t _expectedMainSP;
	const uint32_t _expectedProcessSP;

public:
	BankedStackPointerStatePredicate(uint32_t expectedMainSP, uint32_t expectedProcessSP);
	BankedStackPointerStatePredicate(const micromachine::system::mcu& expected);
	void apply(micromachine::system::mcu& expected);
	void check(const micromachine::system::mcu& actual) const;

private:
	uint32_t getMainSPFrom(const micromachine::system::mcu& mcu) const;
	uint32_t getProcessSPFrom(const micromachine::system::mcu& mcu) const;
	::testing::AssertionResult assertEquality(const char*,
											  const char*,
											  const char*,
											  uint32_t expectedSPValue,
											  uint32_t actualSPValue,
											  const char* spName) const;
};

} // namespace micromachine::testing
