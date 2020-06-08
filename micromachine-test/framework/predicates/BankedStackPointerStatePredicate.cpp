//


//



#include "BankedStackPointerStatePredicate.hpp"
#include "mcu.hpp"

namespace micromachine::testing {

namespace {
	uint32_t getSPFrom(const mcu& mcu, sp_reg::stack_type stackType) {
		return mcu.get_cpu().regs().sp().get_specific_banked_sp(stackType);
	}
}

using namespace micromachine::system;

BankedStackPointerStatePredicate::BankedStackPointerStatePredicate(uint32_t expectedMainSP, uint32_t expectedProcessSP)
	: _expectedMainSP(expectedMainSP)
	, _expectedProcessSP(expectedProcessSP) {}

BankedStackPointerStatePredicate::BankedStackPointerStatePredicate(const mcu& expected)
	: BankedStackPointerStatePredicate(getMainSPFrom(expected), getProcessSPFrom(expected)) {}

void BankedStackPointerStatePredicate::apply(mcu& expected) {
	expected.get_cpu().regs().sp().set_specific_banked_sp(sp_reg::stack_type::main, _expectedMainSP);
	expected.get_cpu().regs().sp().set_specific_banked_sp(sp_reg::stack_type::process, _expectedProcessSP);
}

void BankedStackPointerStatePredicate::check(const mcu& actual) const {
	EXPECT_PRED_FORMAT3(assertEquality, _expectedMainSP, getMainSPFrom(actual), "SP.Main");
	EXPECT_PRED_FORMAT3(assertEquality, _expectedProcessSP, getProcessSPFrom(actual), "SP.Process");
}

uint32_t BankedStackPointerStatePredicate::getMainSPFrom(const mcu& mcu) const {
	return getSPFrom(mcu, sp_reg::stack_type::main);
}

uint32_t BankedStackPointerStatePredicate::getProcessSPFrom(const mcu& mcu) const {
	return getSPFrom(mcu, sp_reg::stack_type::process);
}

::testing::AssertionResult BankedStackPointerStatePredicate::assertEquality(const char*,
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

} // namespace micromachine::testing
