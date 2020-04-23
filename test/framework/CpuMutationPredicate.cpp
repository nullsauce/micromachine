/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#include "CpuMutationPredicate.hpp"
#include "MemoryMutationPredicate.hpp"
#include "RegisterMutationPredicate.hpp"
#include "cpu.hpp"
#include "exception/exception.hpp"
#include "instruction/instruction_pair.hpp"

#include <gtest/gtest.h>

CpuMutationPredicate::CpuMutationPredicate(const micromachine::system::cpu& previous, const micromachine::system::cpu& current)
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
	micromachine::system::instruction_pair instruction = _previous.mem().read32_unchecked(_previous.regs().get_pc());
	PcWasIncrementedBy(instruction.size());
	return *this;
}


CpuMutationPredicate& CpuMutationPredicate::APSRFlagsMatches(const std::string& apsrFlags) {
	for(char c : apsrFlags) {
		switch(c) {
			case 'n' : EXPECT_FALSE(_current.special_regs().app_status_register().negative_flag()); break;
			case 'N' : EXPECT_TRUE(_current.special_regs().app_status_register().negative_flag()); break;
			case 'z' : EXPECT_FALSE(_current.special_regs().app_status_register().zero_flag()); break;
			case 'Z' : EXPECT_TRUE(_current.special_regs().app_status_register().zero_flag()); break;
			case 'c' : EXPECT_FALSE(_current.special_regs().app_status_register().carry_flag()); break;
			case 'C' : EXPECT_TRUE(_current.special_regs().app_status_register().carry_flag()); break;
			case 'v' : EXPECT_FALSE(_current.special_regs().app_status_register().overflow_flag()); break;
			case 'V' : EXPECT_TRUE(_current.special_regs().app_status_register().overflow_flag()); break;
			default: {
				assert(false && "Invalid APSR flag given");
			}
		}
	}
	return *this;
}

CpuMutationPredicate& CpuMutationPredicate::XPSRRegisterDidNotChange() {
	EXPECT_EQ(_previous.special_regs().xpsr_register(), _current.special_regs().xpsr_register());
	return *this;
}


CpuMutationPredicate& CpuMutationPredicate::APSRFlagsDidNotChange() {
	EXPECT_EQ(_previous.special_regs().app_status_register().negative_flag(), _current.special_regs().app_status_register().negative_flag());
	EXPECT_EQ(_previous.special_regs().app_status_register().zero_flag(), _current.special_regs().app_status_register().zero_flag());
	EXPECT_EQ(_previous.special_regs().app_status_register().carry_flag(), _current.special_regs().app_status_register().carry_flag());
	EXPECT_EQ(_previous.special_regs().app_status_register().overflow_flag(), _current.special_regs().app_status_register().overflow_flag());
	return *this;
}

CpuMutationPredicate& CpuMutationPredicate::IPSRFlagsDidNotChange() {
	EXPECT_EQ(_previous.special_regs().interrupt_status_register().exception_num(), _current.special_regs().interrupt_status_register()
	.exception_num());
	return *this;
}

CpuMutationPredicate& CpuMutationPredicate::EPSRFlagsDidNotChange() {
	EXPECT_EQ(_previous.special_regs().execution_status_register().thumb_bit_set(), _current.special_regs().execution_status_register().thumb_bit_set());
	EXPECT_EQ(_previous.special_regs().execution_status_register().stack_alignment(),_current.special_regs().execution_status_register().stack_alignment());
	return *this;
}

CpuMutationPredicate& CpuMutationPredicate::RegistersDidNotChange() {
	for (reg_idx r = 0; r < micromachine::system::core_registers::NUM_GP_REGS; r++) {
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
	return IPSRExceptionNumberIs(micromachine::system::exception::INVALID);
}

CpuMutationPredicate& CpuMutationPredicate::NoInterruptIsActiveOrPending() {
	return NoInterruptIsActive().NoInterruptIsPending();
}

CpuMutationPredicate& CpuMutationPredicate::ExceptionIsPending(micromachine::system::exception::Type ex) {
	EXPECT_TRUE(_current.exceptions().at(ex).is_pending());
	return *this;
}

CpuMutationPredicate& CpuMutationPredicate::ExceptionIsActive(micromachine::system::exception::Type ex) {
	EXPECT_TRUE(_current.exceptions().at(ex).is_active());
	return *this;
}

CpuMutationPredicate& CpuMutationPredicate::ExceptionHandlerReached(micromachine::system::exception::Type ex) {
	uint32_t handler_address = _current.mem().read32(ex * sizeof(uint32_t)) & ~1;
	EXPECT_EQ(_current.regs().get_pc(), handler_address);
	return *this;
}

CpuMutationPredicate& CpuMutationPredicate::HardfaultHandlerReached() {
	return ExceptionIsActive(micromachine::system::exception::Type::HARDFAULT)
		.ExceptionHandlerReached(micromachine::system::exception::Type::HARDFAULT);
}

CpuMutationPredicate& CpuMutationPredicate::PrimaskStatusIs(bool value) {
	EXPECT_EQ(value, _current.special_regs().primask_register().pm());
	return *this;
}

CpuMutationPredicate& CpuMutationPredicate::IPSRExceptionNumberIs(micromachine::system::exception::Type ex) {
	EXPECT_EQ(ex, _current.special_regs().interrupt_status_register().exception_num());
	return *this;
}

CpuMutationPredicate& CpuMutationPredicate::ExecutionIsInHandlerMode() {
	EXPECT_EQ(execution_mode::handler, _current.get_execution_mode());
	return *this;
}

CpuMutationPredicate& CpuMutationPredicate::ExecutionIsInThreadMode() {
	EXPECT_EQ(execution_mode::thread, _current.get_execution_mode());
	return *this;
}

CpuMutationPredicate& CpuMutationPredicate::ThumbBitIsSet() {
	EXPECT_TRUE(_current.special_regs().execution_status_register().thumb_bit_set());
	return *this;
}

CpuMutationPredicate& CpuMutationPredicate::ThumbBitIsNotSet() {
	EXPECT_FALSE(_current.special_regs().execution_status_register().thumb_bit_set());
	return *this;
}

RegisterMutationPredicate CpuMutationPredicate::Register(reg_idx regIdx) {
	return {_previous, _current, regIdx};
}

template <typename uint_type>
MemoryMutationPredicate<uint_type> CpuMutationPredicate::MemoryAt(uint32_t address) {
	return MemoryMutationPredicate<uint_type>(_previous, _current, address);
}
