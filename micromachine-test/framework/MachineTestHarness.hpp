#pragma once

#include "TestHelpers.hpp"
#include "TestMachine.hpp"
#include "CodeGenerator.hpp"
#include "Predicates.hpp"

#include <gtest/gtest.h>

#include <utility>
#include <cstdint>

namespace {

template <class T, class... TT>
struct over_all {
	using next = over_all<TT...>;
	static const constexpr std::size_t size = 1 + next::size;

	template <class C>
	inline constexpr static C for_each(C cbk, T&& tval, TT&&... ttval) {
		cbk(std::forward<T>(tval));
		next::for_each(cbk, std::forward<TT>(ttval)...);
		return cbk;
	}

	template <class C>
	inline constexpr C operator()(C cbk, T&& tval, TT&&... ttval) const {
		return for_each(cbk, std::forward<T>(tval), std::forward<TT>(ttval)...);
	}
};

template <class T>
struct over_all<T> {
	static const constexpr std::size_t size = 1;

	template <class C>
	inline constexpr static C for_each(C cbk, T&& tval) {
		cbk(std::forward<T>(tval));
		return cbk;
	}

	template <class C>
	inline constexpr C operator()(C cbk, T&& tval) const {
		return for_each(cbk, std::forward<T>(tval));
	}
};

}

namespace micromachine::system {
	class mcu;
	class cpu;
}


namespace micromachine::testing {

class MachineTestHarness : public ::testing::Test {
private:
	TestMachine _machine;
	CodeGenerator _code_generator;

protected:
	virtual void SetUp() override;
	void emitInstruction16(const char* encoding, ...);
	void emitInstruction32(const char* encoding1, const char* encoding2, ...);
	void setNextEmitInstructionAddress(uint32_t address);
	void setReg(reg_idx regIdx, uint32_t value);
	uint32_t getReg(reg_idx regIdx);
	void setNegativeFlag(bool flag);
	void setZeroFlag(bool flag);
	void setCarryFlag(bool flag);
	void setOverflowFlag(bool flag);
	void memWrite32(uint32_t address, uint32_t value);
	uint32_t memRead32(uint32_t address);
	void resetMachine(uint32_t initialPc);
	void raiseHardfault();
	void raiseException(exception ex);
	void raiseExternalInterrupt(exception ex);
	void enabledAndRaiseExternalInterrupt(exception ex);
	void setExceptionPriority(exception ex, exception::priority_t priority);
	void setXPSR(uint32_t xpsr);
	void enterHandlerMode();
	void enterThreadMode();
	void setPrimaskFlag(bool flag);
	void setControl(bool nPriv, bool spSel);

	template <typename... Args>
	void CheckThat(Args... predicates) {
		// Runs these predicates check method on the current state
		over_all<Args...>::for_each([&](auto&& predicate) { predicate.check(_machine.getMCU()); },
									std::forward<Args>(predicates)...);
	}

	template <typename... Args>
	void StepAndExpectThat(Args... predicates) {

		// fork the current state into a new state
		// that serves as a prediction to what we expected
		// the reference state to become after the call
		// to step()
		TestMachine& actualMachineState = _machine;
		TestMachine predictedMachineState = actualMachineState;

		mcu& actualState = actualMachineState.getMCU();
		mcu& predictedState = predictedMachineState.getMCU();

		// apply the transforms to the predicted state
		over_all<Args...>::for_each([&](auto&& predicate) { predicate.apply(predictedState); },
									std::forward<Args>(predicates)...);

		// step the reference state
		actualState.step();

		// check that the predicted state is globally equal to the reference state
		// this will detect any difference not explicitly covered by the predicates.
		over_all<Args...>::for_each([&](const auto&& predicate) { predicate.check(actualState); },
									std::forward<Args>(predicates)...);

		// all core registers are equal
		for(reg_idx r = 0; r < core_registers::NUM_CORE_REGS; r++) {
			RegisterEqualityPredicate(r, predictedState.get_cpu().regs().get(r)).check(actualState);
		}

		// all special registers are equal
		APSRFlagsStatePredicate(predictedState).check(actualState);
		IPSRStatePredicate(predictedState).check(actualState);
		EPSRFlagsStatePredicate(predictedState).check(actualState);
		ControlRegStatePredicate(predictedState).check(actualState);
		PrimaskRegStatePredicate(predictedState).check(actualState);
		ExecutionModeStatePredicate(predictedState).check(actualState);

		// exceptions state is equal
		for(uint32_t i = 1; i < predictedState.exceptions().supported_exception_count(); ++i) {
			ExceptionActiveStatePredicate(exception::from_number(i), predictedState).check(actualState);
			ExceptionPendingStatePredicate(exception::from_number(i), predictedState).check(actualState);
			ExceptionPriorityStatePredicate(exception::from_number(i), predictedState).check(actualState);
		}

		// memory is equal
		MemoryStateEqualityChecker::check(predictedState, actualState);
	}

	RegistersPushedStatePredicate::StackedValues captureCurrentContext() const {
		return captureContext(_machine.getCpu());
	}

	auto exceptionHandlerReachedWithContext(exception ex,
											RegistersPushedStatePredicate::StackedValues contextValues) const {
		return exceptionHandlerReachedAtAddressWithContext(ex, _machine.exceptionHandlerAddress(ex), std::move(contextValues));
	}

	auto exceptionHandlerReached(exception ex) {
		return exceptionHandlerReachedWithContext(ex, captureCurrentContext());
	}

	auto hardfaultHandlerReached() {
		return exceptionHandlerReached(exception::HARDFAULT);
	}

	template <typename... Args>
	void StepAndExpectThatInstruction16IsExecutedAndThat(Args... predicates) {
		return StepAndExpectThat(std::forward<Args>(predicates)..., instruction16Executed(), thumbFlagIsSet());
	}

	template <typename... Args>
	void StepAndExpectThatInstruction32IsExecutedAndThat(Args... predicates) {
		return StepAndExpectThat(std::forward<Args>(predicates)..., instruction32Executed(), thumbFlagIsSet());
	}
};

}
