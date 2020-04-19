/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_CPUMUTATIONPREDICATE_HPP
#define MICROMACHINE_CPUMUTATIONPREDICATE_HPP

#include "cpu.hpp"

class RegisterMutationPredicate;
class CpuMutationPredicate {
protected:
	const cpu& _previous;
	const cpu& _current;

public:
	CpuMutationPredicate(const cpu& previous, const cpu& current);
	CpuMutationPredicate& PcWasIncrementedBy(uint32_t amount);
	CpuMutationPredicate& PcDidNotChange();
	CpuMutationPredicate& InstructionExecutedWithoutBranch();
	CpuMutationPredicate& RegisterValueEquals(reg_idx index, uint32_t value);
	CpuMutationPredicate& XPSRFlagsEquals(const std::string& apsrFlags);
	CpuMutationPredicate& XPSRRegisterDidNotChange();
	CpuMutationPredicate& APSRFlagsDidNotChange();
	CpuMutationPredicate& RegistersDidNotChange();
	CpuMutationPredicate& NoInterruptIsPending();
	CpuMutationPredicate& ExceptionIsPending(exception::Type ex);
	CpuMutationPredicate& ExceptionIsActive(exception::Type ex);
	CpuMutationPredicate& ExceptionHandlerReached(exception::Type ex);
	CpuMutationPredicate& HardfaultHandlerReached();
	RegisterMutationPredicate Register(reg_idx regIdx);
};


#endif //MICROMACHINE_CPUMUTATIONPREDICATE_HPP
