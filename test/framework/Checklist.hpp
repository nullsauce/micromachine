/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_CHECKLIST_HPP
#define MICROMACHINE_CHECKLIST_HPP

#include <vector>
#include <functional>
#include <gtest/gtest.h>

#include "cpu.hpp"

class Checklist {
public:
	using check_fn = std::function<void(const cpu&)>;

private:
	std::vector<check_fn> _checks;

public:
	template<typename... Args>
	Checklist(Args... args)
		: _checks({std::forward<Args>(args)...}) {}

	template<typename T>
	Checklist& append(T&& check) {
		_checks.push_back(std::move(check));
		return *this;
	}

	void check(const cpu& cpu) {
		for (auto& check : _checks) {
			check(cpu);
		}
	}
};

Checklist::check_fn PcEquals(uint32_t val) {
	return [=](const cpu& cpu) {
		EXPECT_EQ(val, cpu.regs().get_pc());
	};
}

class ReferringToCpu {
private:
	const cpu& _cpu;
public:
	ReferringToCpu(const cpu& cpu)
		: _cpu(cpu)
	{}

	Checklist::check_fn PcWasIncrementedBy(size_t amount) {
		return PcEquals(_cpu.regs().get_pc() + amount);
	}
};


Checklist::check_fn ThumBitIsSet() {
	return [](const cpu& cpu) {
		EXPECT_TRUE(cpu.regs().execution_status_register().thumb_bit_set());
	};
}

Checklist::check_fn FlagsAre(int val) {
	return [=](const cpu& cpu) {
		assert(cpu.flags == val);
	};
}


#endif //MICROMACHINE_CHECKLIST_HPP
