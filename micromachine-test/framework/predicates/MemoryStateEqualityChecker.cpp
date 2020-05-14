
//
// Created by fla on 25.04.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//



#include "MemoryStateEqualityChecker.hpp"
#include "ValueFormaters.hpp"
#include "mcu.hpp"

namespace micromachine::testing {

void MemoryStateEqualityChecker::check(const mcu& predictedState, const mcu& actualState) {

	// memory layout is identical
	EXPECT_EQ(predictedState.get_memory().regions().size(), actualState.get_memory().regions().size());

	size_t numRegions = predictedState.get_memory().regions().size();
	for(size_t i = 0; i < numRegions; i++) {
		const auto& predictedRegion = predictedState.get_memory().regions().at(i);
		const auto& actualRegion = actualState.get_memory().regions().at(i);
		EXPECT_EQ(predictedRegion.size(), actualRegion.size());
		EXPECT_EQ(predictedRegion.start(), actualRegion.start());
		EXPECT_EQ(predictedRegion.end(), actualRegion.end());
		EXPECT_NE(nullptr, predictedRegion.host_mem());
		EXPECT_NE(nullptr, actualRegion.host_mem());
		EXPECT_NE(predictedRegion.host_mem(), actualRegion.host_mem());

		for(size_t offset = 0; offset < predictedRegion.size(); offset++) {
			const uint8_t& predictedByte = predictedRegion.host_mem()[offset];
			const uint8_t& actualByte = actualRegion.host_mem()[offset];
			if(predictedByte != actualByte) {
				EXPECT_PRED_FORMAT5(assertMemoryByteEquality,
									predictedByte,
									actualByte,
									offset,
									actualRegion,
									predictedRegion);
				break;
			}
		}
	}
}

::testing::AssertionResult MemoryStateEqualityChecker::assertMemoryByteEquality(const char*,
																				const char*,
																				const char*,
																				const char*,
																				const char*,
																				uint8_t expectedByte,
																				uint8_t actualByte,
																				uint32_t offset,
																				const memory_mapping& actualRegion,
																				const memory_mapping& expectedRegion) {

	uint32_t address = actualRegion.start() + offset;
	if(expectedByte == actualByte) {
		return ::testing::AssertionSuccess();
	}

	std::stringstream msg;

	msg << "Equality check fail for memory in region" << actualRegion.name() << " at address " << uint32ToStr(address)
		<< " (offset: " << uint32ToStr(offset) << ")" << std::endl
		<< " * Expected: " << uint8ToStr(expectedByte) << std::endl
		<< " * Actual  : " << uint8ToStr(actualByte) << std::endl;

	if(address % 4 == 0) {
		uint32_t actualValue = *((uint32_t*)actualRegion.translate(address));
		uint32_t expectedValue = *((uint32_t*)expectedRegion.translate(address));
		msg << std::endl << "Info : address is 32-bit aligned" << std::endl;
		msg << "Expected 32-bit value is " << uint32ToStr(actualValue) << std::endl;
		msg << "Actual   32-bit value is " << uint32ToStr(expectedValue) << std::endl;
	}

	if(address % 2 == 0) {
		uint16_t actualValue = *((uint16_t*)actualRegion.translate(address));
		uint16_t expectedValue = *((uint16_t*)expectedRegion.translate(address));
		msg << std::endl << "Info : address is 16-bit aligned" << std::endl;
		msg << "Expected 16-bit value is " << uint16ToStr(actualValue) << std::endl;
		msg << "Actual   16-bit value is " << uint16ToStr(expectedValue) << std::endl;
	}

	return ::testing::AssertionFailure() << msg.str();
}

} // namespace micromachine::testing
