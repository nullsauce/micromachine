/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_MEMORYMUTATIONPREDICATE_HPP
#define MICROMACHINE_MEMORYMUTATIONPREDICATE_HPP

#include "CpuMutationPredicate.hpp"

template <typename uint_type>
class MemoryMutationPredicate: public CpuMutationPredicate {
private:
	const uint32_t _address;

	uint32_t currentValue() {
		return _current.mem().template read<uint_type>(_address);
	}

	uint32_t previousValue() {
		return _previous.mem().template read<uint_type>(_address);
	}

public:
	MemoryMutationPredicate(const cpu& previous, const cpu& current, uint32_t address);
	MemoryMutationPredicate<uint_type>& Equals(uint32_t value);
	MemoryMutationPredicate<uint_type>& DidNotChange();
	MemoryMutationPredicate<uint_type>& Changed();

};


#endif //MICROMACHINE_MEMORYMUTATIONPREDICATE_HPP
