/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#pragma once

#include "cpu.hpp"
#include "exception/exception_defs.hpp"

class RegisterMutationPredicate;

template<typename uint_type>
class MemoryMutationPredicate;

template<>
class MemoryMutationPredicate<uint32_t>;

template<>
class MemoryMutationPredicate<uint16_t>;

template<>
class MemoryMutationPredicate<uint8_t>;


class CpuMutationPredicate {
protected:
	const micromachine::system::cpu& _previous;
	const micromachine::system::cpu& _current;

public:
	CpuMutationPredicate(const micromachine::system::cpu& previous, const micromachine::system::cpu& current);
	CpuMutationPredicate& PcWasIncrementedBy(uint32_t amount);
	CpuMutationPredicate& PcDidNotChange();
	CpuMutationPredicate& InstructionExecutedWithoutBranch();
	CpuMutationPredicate& RegisterValueEquals(reg_idx index, uint32_t value);
	CpuMutationPredicate& APSRFlagsMatches(const std::string& apsrFlags);
	CpuMutationPredicate& XPSRRegisterDidNotChange();
	CpuMutationPredicate& APSRFlagsDidNotChange();
	CpuMutationPredicate& IPSRFlagsDidNotChange();
	CpuMutationPredicate& EPSRFlagsDidNotChange();
	CpuMutationPredicate& RegistersDidNotChange();
	CpuMutationPredicate& NoInterruptIsPending();
	CpuMutationPredicate& NoInterruptIsActive();
	CpuMutationPredicate& NoInterruptIsActiveOrPending();
	CpuMutationPredicate& ExceptionIsPending(micromachine::system::exception::Type ex);
	CpuMutationPredicate& ExceptionIsActive(micromachine::system::exception::Type ex);
	CpuMutationPredicate& ExceptionHandlerReached(micromachine::system::exception::Type ex);
	CpuMutationPredicate& HardfaultHandlerReached();
	CpuMutationPredicate& PrimaskStatusIs(bool value);
	CpuMutationPredicate& IPSRExceptionNumberIs(micromachine::system::exception::Type ex);
	CpuMutationPredicate& ExecutionIsInHandlerMode();
	CpuMutationPredicate& ExecutionIsInThreadMode();
	CpuMutationPredicate& ThumbBitIsSet();
	CpuMutationPredicate& ThumbBitIsNotSet();

	RegisterMutationPredicate Register(reg_idx regIdx);

	template <typename uint_type>
	MemoryMutationPredicate<uint_type> MemoryAt(uint32_t address);


};


