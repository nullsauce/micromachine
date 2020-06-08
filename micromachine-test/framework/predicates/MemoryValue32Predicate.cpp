//

//



#include <string>
#include <types/types.hpp>

#include "MemoryValue32Predicate.hpp"
#include "ValueFormaters.hpp"
#include "mcu.hpp"

namespace micromachine::testing {

using namespace micromachine::system;

MemoryValue32Predicate::MemoryValue32Predicate(uint32_t address, uint32_t value)
	: MemoryValuePredicate(address)
	, _expectedValue(value) {}

void MemoryValue32Predicate::apply(mcu& expected) {
	if(!expected.get_memory().write32(_address, _expectedValue)) {
		micromachine_fail("MemoryValue32Predicate applied at an unmapped address. address=%08x value=%08x",
						  _address,
						  _expectedValue);
	}
}

void MemoryValue32Predicate::check(const mcu& actual) const {
	bool memory_access_ok = false;
	uint32_t actualValue = actual.get_memory().read32(_address, memory_access_ok);
	EXPECT_TRUE(memory_access_ok);
	EXPECT_PRED_FORMAT3(assertMemoryEquality, actualValue, _expectedValue, _address);
}

::testing::AssertionResult MemoryValue32Predicate::assertMemoryEquality(const char*,
																		const char*,
																		const char*,
																		uint32_t actualValue,
																		uint32_t expectedValue,
																		uint32_t address) {

	if(expectedValue == actualValue) {
		return ::testing::AssertionSuccess();
	}

	return ::testing::AssertionFailure() << "Equality check fail for memory at address " << uint32ToStr(address)
										 << std::endl
										 << " * Expected: " << uint32ToStr(expectedValue) << std::endl
										 << " * Actual  : " << uint32ToStr(actualValue) << std::endl;
}

} // namespace micromachine::testing
