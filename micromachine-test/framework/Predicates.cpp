
//
// Created by fla on 29.04.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#include "Predicates.hpp"

namespace micromachine::testing {

	APSRFlagsStatePredicate apsrFlagsEquals(const char apsrFlagStr[4]) {
		return {apsrFlagStr};
	}

	RegisterEqualityPredicate registerValueEquals(reg_idx regIdx, uint32_t value) {
		return {regIdx, value};
	}

	RegisterUnsignedDecrementPredicate StackGrowthOf(uint32_t amount) {
		return {registers::SP, amount};
	}

	ExceptionStatePredicateBuilder exceptionStateOf(exception::Type exception) {
		return {exception};
	}

	RegisterStatePredicateBuilder reg(reg_idx regIdx) {
		return {regIdx};
	}

	MemoryValuePredicateBuilder memoryAt(uint32_t address) {
		return {address};
	}

	EPSRFlagsStatePredicate thumbFlagIsNotSet() {
		return {false};
	}

	EPSRFlagsStatePredicate thumbFlagIsSet() {
		return {true};
	}

	ExecutionModeStatePredicate executionModeIs(execution_mode::mode mode) {
		return {mode};
	}

	ExecutionModeStatePredicate executionModeIsThread() {
		return executionModeIs(execution_mode::thread);
	}

	ExecutionModeStatePredicate executionModeIsHandler() {
		return executionModeIs(execution_mode::handler);
	}

	PrimaskRegStatePredicate primaskIsSet() {
		return {true};
	}

	PrimaskRegStatePredicate primaskIsNotSet() {
		return {false};
	}

	uint32_t makeXPSRFlagsFrom(bool negativeFlag,
							   bool zeroFlag,
							   bool carryFlag,
							   bool overflowFlag,
							   exception::Type ex,
							   bool thumbFlag) {
		uint32_t xpsrValue = 0;

		apsr_reg apsrReg(xpsrValue);
		apsrReg.negative_flag() = negativeFlag;
		apsrReg.zero_flag() = zeroFlag;
		apsrReg.carry_flag() = carryFlag;
		apsrReg.overflow_flag() = overflowFlag;

		epsr_reg epsrReg(xpsrValue);
		epsrReg.set_thumb_bit(thumbFlag);

		ipsr_reg ipsrReg(xpsrValue);
		ipsrReg.set_exception_number(ex);

		return xpsrValue;
	}

	uint32_t makeXPSRFlagsFrom(const char* apsrFlagStr, exception::Type ex, bool thumbFlag) {
		const APSRFlags apsrFlags(apsrFlagStr);
		return makeXPSRFlagsFrom(apsrFlags.negativeFlag,
								 apsrFlags.zeroFlag,
								 apsrFlags.carryFlag,
								 apsrFlags.overflowFlag,
								 ex,
								 thumbFlag);
	}

	uint32_t makeXPSRFlagsFromCPUState(const cpu& cpu) {
		const apsr_reg& apsrReg = cpu.special_regs().app_status_register();
		const epsr_reg& epsrReg = cpu.special_regs().execution_status_register();
		const ipsr_reg& ipsrReg = cpu.special_regs().interrupt_status_register();
		return makeXPSRFlagsFrom(apsrReg.negative_flag(),
								 apsrReg.zero_flag(),
								 apsrReg.carry_flag(),
								 apsrReg.overflow_flag(),
								 ipsrReg.exception_num(),
								 epsrReg.thumb_flag());
	}

	RegistersPushedStatePredicate::StackedValues captureContext(const cpu& cpu) {
		return {cpu.regs().get(registers::R0),
				cpu.regs().get(registers::R1),
				cpu.regs().get(registers::R2),
				cpu.regs().get(registers::R3),
				cpu.regs().get(registers::R12),
				cpu.regs().get(registers::LR),
				cpu.regs().get(registers::PC),
				makeXPSRFlagsFromCPUState(cpu)};
	}


}
