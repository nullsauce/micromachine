/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#include "CpuMutationPredicate.hpp"
#include "RegisterMutationPredicate.hpp"
#include "MemoryMutationPredicate.hpp"

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


CpuMutationPredicate& CpuMutationPredicate::APSRFlagsMatches(const std::string& apsrFlags) {
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
			default: {
				assert(false && "Invalid APSR flag given");
			}
		}
	}
	return *this;
}

CpuMutationPredicate& CpuMutationPredicate::XPSRRegisterDidNotChange() {
	EXPECT_EQ(_previous.regs().xpsr_register(), _current.regs().xpsr_register());
	return *this;
}


CpuMutationPredicate& CpuMutationPredicate::APSRFlagsDidNotChange() {
	EXPECT_EQ(_previous.regs().app_status_register().neg_flag(), _current.regs().app_status_register().neg_flag());
	EXPECT_EQ(_previous.regs().app_status_register().zero_flag(), _current.regs().app_status_register().zero_flag());
	EXPECT_EQ(_previous.regs().app_status_register().carry_flag(), _current.regs().app_status_register().carry_flag());
	EXPECT_EQ(_previous.regs().app_status_register().overflow_flag(), _current.regs().app_status_register().overflow_flag());
	return *this;
}

CpuMutationPredicate& CpuMutationPredicate::IPSRFlagsDidNotChange() {
	EXPECT_EQ(_previous.regs().interrupt_status_register().exception_num(), _current.regs().interrupt_status_register()
	.exception_num());
	return *this;
}

CpuMutationPredicate& CpuMutationPredicate::EPSRFlagsDidNotChange() {
	EXPECT_EQ(_previous.regs().execution_status_register().thumb_bit_set(), _current.regs().execution_status_register().thumb_bit_set());
	EXPECT_EQ(_previous.regs().execution_status_register().stack_alignment(),_current.regs().execution_status_register().stack_alignment());
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

CpuMutationPredicate& CpuMutationPredicate::NoInterruptIsActive() {
	EXPECT_FALSE(_current.exceptions().any_active());
	return IPSRExceptionNumberIs(exception::INVALID);
}

CpuMutationPredicate& CpuMutationPredicate::NoInterruptIsActiveOrPending() {
	return NoInterruptIsActive().NoInterruptIsPending();
}

CpuMutationPredicate& CpuMutationPredicate::ExceptionIsPending(exception::Type ex) {
	EXPECT_TRUE(_current.exceptions().at(ex).is_pending());
	return *this;
}

CpuMutationPredicate& CpuMutationPredicate::ExceptionIsActive(exception::Type ex) {
	EXPECT_TRUE(_current.exceptions().at(ex).is_active());
	return *this;
}

CpuMutationPredicate& CpuMutationPredicate::ExceptionHandlerReached(exception::Type ex) {
	uint32_t handler_address = _current.mem().read32(ex * sizeof(uint32_t)) & ~1;
	EXPECT_EQ(_current.regs().get_pc(), handler_address);
	return *this;
}

CpuMutationPredicate& CpuMutationPredicate::HardfaultHandlerReached() {
	return ExceptionIsActive(exception::Type::HARDFAULT)
		.ExceptionHandlerReached(exception::Type::HARDFAULT);
}

CpuMutationPredicate& CpuMutationPredicate::PrimaskStatusIs(bool value) {
	EXPECT_EQ(value, _current.regs().primask_register().pm());
	return *this;
}

CpuMutationPredicate& CpuMutationPredicate::IPSRExceptionNumberIs(exception::Type ex) {
	EXPECT_EQ(ex, _current.regs().interrupt_status_register().exception_num());
	return *this;
}

CpuMutationPredicate& CpuMutationPredicate::ExecutionIsInHandlerMode() {
	EXPECT_TRUE(_current.regs().exec_mode_register().is_handler_mode());
	return *this;
}

CpuMutationPredicate& CpuMutationPredicate::ExecutionIsInThreadMode() {
	EXPECT_TRUE(_current.regs().exec_mode_register().is_thread_mode());
	return *this;
}

CpuMutationPredicate& CpuMutationPredicate::ThumbBitIsSet() {
	EXPECT_TRUE(_current.regs().execution_status_register().thumb_bit_set());
	return *this;
}

CpuMutationPredicate& CpuMutationPredicate::ThumbBitIsNotSet() {
	EXPECT_FALSE(_current.regs().execution_status_register().thumb_bit_set());
	return *this;
}

RegisterMutationPredicate CpuMutationPredicate::Register(reg_idx regIdx) {
	return {_previous, _current, regIdx};
}

template <typename uint_type>
MemoryMutationPredicate<uint_type> CpuMutationPredicate::MemoryAt(uint32_t address) {
	return MemoryMutationPredicate<uint_type>(_previous, _current, address);
}
