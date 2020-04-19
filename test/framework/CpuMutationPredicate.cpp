/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#include "CpuMutationPredicate.hpp"
#include "RegisterMutationPredicate.hpp"

#include <gtest/gtest.h>

CpuMutationPredicate::CpuMutationPredicate(const cpu& previous, const cpu& current)
	: _previous(previous)
	, _current(current)
{}

CpuMutationPredicate& CpuMutationPredicate::PcWasIncrementedBy(uint32_t amount) {
	uint32_t expected_pc = _previous.regs().get_pc() + amount;
	EXPECT_EQ(expected_pc, _current.regs().get_pc());
	return *this;
}

CpuMutationPredicate& CpuMutationPredicate::PcDidNotChange() {
	return PcWasIncrementedBy(0);
}

CpuMutationPredicate& CpuMutationPredicate::InstructionExecutedWithoutBranch() {
	instruction_pair instruction = _previous.fetch_instruction(_previous.regs().get_pc());
	PcWasIncrementedBy(instruction.size());
	return *this;
}


CpuMutationPredicate& CpuMutationPredicate::XPSRFlagsEquals(const std::string& apsrFlags) {
	for(char c : apsrFlags) {
		switch(c) {
			case 'n' : EXPECT_FALSE(_current.regs().app_status_register().neg_flag()); break;
			case 'N' : EXPECT_TRUE(_current.regs().app_status_register().neg_flag()); break;
			case 'z' : EXPECT_FALSE(_current.regs().app_status_register().zero_flag()); break;
			case 'Z' : EXPECT_TRUE(_current.regs().app_status_register().zero_flag()); break;
			case 'c' : EXPECT_FALSE(_current.regs().app_status_register().carry_flag()); break;
			case 'C' : EXPECT_TRUE(_current.regs().app_status_register().carry_flag()); break;
			case 'v' : EXPECT_FALSE(_current.regs().app_status_register().overflow_flag()); break;
			case 'V' : EXPECT_TRUE(_current.regs().app_status_register().overflow_flag()); break;
			case 't' : EXPECT_FALSE(_current.regs().execution_status_register().thumb_bit_set()); break;
			case 'T' : EXPECT_TRUE(_current.regs().execution_status_register().thumb_bit_set()); break;
		}
	}
	return *this;
}

CpuMutationPredicate& CpuMutationPredicate::XPSRRegisterDidNotChange() {
	EXPECT_EQ(_previous.regs().xpsr_register(), _current.regs().xpsr_register());
	return *this;
}

CpuMutationPredicate& CpuMutationPredicate::RegistersDidNotChange() {
	for (reg_idx r = 0; r < registers::NUM_GP_REGS; r++) {
		Register(r).DidNotChange();
	}
	return *this;
}


CpuMutationPredicate& CpuMutationPredicate::NoInterruptIsPending() {
	EXPECT_FALSE(_current.exceptions().any_pending());
	return *this;
}

RegisterMutationPredicate CpuMutationPredicate::Register(reg_idx regIdx) {
	return {_previous, _current, regIdx};
}