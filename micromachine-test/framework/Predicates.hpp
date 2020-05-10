
//
// Created by fla on 29.04.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#pragma once

#include "predicates/APSRFlagsStatePredicate.hpp"
#include "predicates/EPSRFlagsStatePredicate.hpp"
#include "predicates/ExceptionStatePredicateBuilder.hpp"
#include "predicates/ExecutionModeStatePredicate.hpp"
#include "predicates/MemoryValuePredicateBuilder.hpp"
#include "predicates/MemoryStateEqualityChecker.hpp"
#include "predicates/PrimaskStateRepdicate.hpp"
#include "predicates/RegisterStatePredicateBuilder.hpp"
#include "predicates/IPSRStatePredicate.hpp"
#include "predicates/ControlRegStatePredicate.hpp"
#include "predicates/BankedStackPointerStatePredicate.hpp"

namespace micromachine::testing {
	APSRFlagsStatePredicate apsrFlagsEquals(const char apsrFlagStr[4]);
	RegisterEqualityPredicate registerValueEquals(reg_idx regIdx, uint32_t value);
	RegisterUnsignedDecrementPredicate StackGrowthOf(uint32_t amount);
	ExceptionStatePredicateBuilder exceptionStateOf(exception exception);
	RegisterStatePredicateBuilder reg(reg_idx regIdx);
	MemoryValuePredicateBuilder memoryAt(uint32_t address);
	EPSRFlagsStatePredicate thumbFlagIsNotSet();
	EPSRFlagsStatePredicate thumbFlagIsSet();
	ExecutionModeStatePredicate executionModeIs(execution_mode::mode mode);
	ExecutionModeStatePredicate executionModeIsThread();
	ExecutionModeStatePredicate executionModeIsHandler();
	PrimaskRegStatePredicate primaskIsSet();
	PrimaskRegStatePredicate primaskIsNotSet();

	uint32_t makeXPSRFlagsFrom(bool negativeFlag,
							   bool zeroFlag,
							   bool carryFlag,
							   bool overflowFlag,
							   exception ex,
							   bool thumbFlag);
	uint32_t makeXPSRFlagsFrom(const char* apsrFlagStr, exception ex, bool thumbFlag);
	uint32_t makeXPSRFlagsFromCPUState(const cpu& cpu);
	RegistersPushedStatePredicate::StackedValues captureContext(const cpu& cpu);

	static auto instruction16Executed() {
		return PredicateAggregate(reg(registers::PC).wasIncrementedBy(sizeof(uint16_t)));
	}

	static auto instruction32Executed() {
		return PredicateAggregate(reg(registers::PC).wasIncrementedBy(sizeof(uint32_t)));
	}

	static auto noExceptionIsPending() {
		return PredicateAggregate(exceptionStateOf(exception::INVALID).isNotPending(),
								  exceptionStateOf(exception::RESET).isNotPending(),
								  exceptionStateOf(exception::NMI).isNotPending(),
								  exceptionStateOf(exception::HARDFAULT).isNotPending(),
								  exceptionStateOf(exception::_RESERVED_0).isNotPending(),
								  exceptionStateOf(exception::_RESERVED_1).isNotPending(),
								  exceptionStateOf(exception::_RESERVED_2).isNotPending(),
								  exceptionStateOf(exception::_RESERVED_3).isNotPending(),
								  exceptionStateOf(exception::_RESERVED_4).isNotPending(),
								  exceptionStateOf(exception::_RESERVED_5).isNotPending(),
								  exceptionStateOf(exception::_RESERVED_6).isNotPending(),
								  exceptionStateOf(exception::SVCALL).isNotPending(),
								  exceptionStateOf(exception::_RESERVED_7).isNotPending(),
								  exceptionStateOf(exception::_RESERVED_8).isNotPending(),
								  exceptionStateOf(exception::PENDSV).isNotPending(),
								  exceptionStateOf(exception::SYSTICK).isNotPending(),
								  exceptionStateOf(exception::EXTI_00).isNotPending(),
								  exceptionStateOf(exception::EXTI_01).isNotPending(),
								  exceptionStateOf(exception::EXTI_02).isNotPending(),
								  exceptionStateOf(exception::EXTI_03).isNotPending(),
								  exceptionStateOf(exception::EXTI_04).isNotPending(),
								  exceptionStateOf(exception::EXTI_05).isNotPending(),
								  exceptionStateOf(exception::EXTI_06).isNotPending(),
								  exceptionStateOf(exception::EXTI_07).isNotPending(),
								  exceptionStateOf(exception::EXTI_08).isNotPending(),
								  exceptionStateOf(exception::EXTI_09).isNotPending(),
								  exceptionStateOf(exception::EXTI_10).isNotPending(),
								  exceptionStateOf(exception::EXTI_11).isNotPending(),
								  exceptionStateOf(exception::EXTI_12).isNotPending(),
								  exceptionStateOf(exception::EXTI_13).isNotPending(),
								  exceptionStateOf(exception::EXTI_14).isNotPending(),
								  exceptionStateOf(exception::EXTI_15).isNotPending());
	}

	static auto noExceptionIsActive() {
		return PredicateAggregate(exceptionStateOf(exception::INVALID).isNotActive(),
								  exceptionStateOf(exception::RESET).isNotActive(),
								  exceptionStateOf(exception::NMI).isNotActive(),
								  exceptionStateOf(exception::HARDFAULT).isNotActive(),
								  exceptionStateOf(exception::_RESERVED_0).isNotActive(),
								  exceptionStateOf(exception::_RESERVED_1).isNotActive(),
								  exceptionStateOf(exception::_RESERVED_2).isNotActive(),
								  exceptionStateOf(exception::_RESERVED_3).isNotActive(),
								  exceptionStateOf(exception::_RESERVED_4).isNotActive(),
								  exceptionStateOf(exception::_RESERVED_5).isNotActive(),
								  exceptionStateOf(exception::_RESERVED_6).isNotActive(),
								  exceptionStateOf(exception::SVCALL).isNotActive(),
								  exceptionStateOf(exception::_RESERVED_7).isNotActive(),
								  exceptionStateOf(exception::_RESERVED_8).isNotActive(),
								  exceptionStateOf(exception::PENDSV).isNotActive(),
								  exceptionStateOf(exception::SYSTICK).isNotActive(),
								  exceptionStateOf(exception::EXTI_00).isNotActive(),
								  exceptionStateOf(exception::EXTI_01).isNotActive(),
								  exceptionStateOf(exception::EXTI_02).isNotActive(),
								  exceptionStateOf(exception::EXTI_03).isNotActive(),
								  exceptionStateOf(exception::EXTI_04).isNotActive(),
								  exceptionStateOf(exception::EXTI_05).isNotActive(),
								  exceptionStateOf(exception::EXTI_06).isNotActive(),
								  exceptionStateOf(exception::EXTI_07).isNotActive(),
								  exceptionStateOf(exception::EXTI_08).isNotActive(),
								  exceptionStateOf(exception::EXTI_09).isNotActive(),
								  exceptionStateOf(exception::EXTI_10).isNotActive(),
								  exceptionStateOf(exception::EXTI_11).isNotActive(),
								  exceptionStateOf(exception::EXTI_12).isNotActive(),
								  exceptionStateOf(exception::EXTI_13).isNotActive(),
								  exceptionStateOf(exception::EXTI_14).isNotActive(),
								  exceptionStateOf(exception::EXTI_15).isNotActive());
	}

	static auto noExceptionIsActiveOrPending() {
		return PredicateAggregate(noExceptionIsActive(), noExceptionIsPending());
	}

	static auto
	exceptionHandlerReachedAtAddressWithContext(exception ex,
												uint32_t handlerAddress,
												RegistersPushedStatePredicate::StackedValues contextValues) {

		return PredicateAggregate(reg(registers::PC).equals(handlerAddress),
								  exceptionStateOf(ex).isActive(),
								  exceptionStateOf(ex).isNotPending(),
								  executionModeIsHandler(),
								  RegistersPushedStatePredicate(contextValues),
								  reg(registers::LR).equals(0xFFFFFFF9),
								  IPSRStatePredicate(ex),
								  StackGrowthOf(32),
								  thumbFlagIsSet());
	}
} // namespace micromachine::testing
