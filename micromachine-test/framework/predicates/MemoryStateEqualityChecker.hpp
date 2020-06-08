
//


//

#pragma once

#include "mcu_foward_decl.hpp"
#include "memory/mapping.hpp"

#include <gtest/gtest.h>

namespace micromachine::testing {

class MemoryStateEqualityChecker {
public:
	static void check(const micromachine::system::mcu& predictedState, const micromachine::system::mcu& actualState);

private:
	static ::testing::AssertionResult assertMemoryByteEquality(const char*,
															   const char*,
															   const char*,
															   const char*,
															   const char*,
															   uint8_t expectedByte,
															   uint8_t actualByte,
															   uint32_t offset,
															   const micromachine::system::memory_mapping& actualRegion,
															   const micromachine::system::memory_mapping& expectedRegion);
};
} // namespace micromachine::testing
