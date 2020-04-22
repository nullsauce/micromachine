/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_TESTSYSTEM_HPP
#define MICROMACHINE_TESTSYSTEM_HPP

#include "system.hpp"

#include <array>

/**
 * A system is a cpu + memory
 */
class TestSystem {
public:
	static constexpr size_t MEMORY_SIZE = 0x00008000;
	static constexpr uint32_t INITIAL_SP = 0x00002000;
	static constexpr uint32_t INITIAL_PC = 0x00001000;
	static constexpr uint32_t INITIAL_LR = 0xFFFFFFFF;

private:
	std::vector<uint8_t> _memoryStorage;
	class system _system;

	void initializeInterruptVectorTableWithStubs() {
		constexpr uint32_t handlers_base = 0x3000;
		constexpr uint32_t fake_handler_size = 0x10;

		// note that these stub handlers are not valid code
		for (uint32_t i = 1; i < _system.get_cpu().exceptions().supported_exception_count(); ++i) {
			uint32_t handler_address = handlers_base + (i * fake_handler_size);
			_system.get_memory().write32(i * sizeof(uint32_t), handler_address);
		}
	}

public:
	TestSystem()
		: _memoryStorage(MEMORY_SIZE) {
		// Maps host to virtual memory
		// There's only one segment in the test system
		_system.get_memory().map(_memoryStorage.data(), 0, _memoryStorage.size());

		// zero memory
		std::fill(_memoryStorage.begin(), _memoryStorage.end(), 0);

		// write SP to memory address 0x0
		((uint32_t*)_memoryStorage.data())[0] = INITIAL_SP;

		initializeInterruptVectorTableWithStubs();

		// initialize the system
		_system.reset(INITIAL_PC);

	};

	TestSystem(const TestSystem& other)
		: _memoryStorage(other._memoryStorage)
		, _system(other._system) {
		// Map the existing memory
		_system.get_memory().map(_memoryStorage.data(), 0, _memoryStorage.size());
	}

	const class system& getSystem() const {
		return _system;
	}

	class system& getSystem() {
		return _system;
	}

	const cpu& getCpu() const {
		return _system.get_cpu();
	}

	cpu& getCpu() {
		return _system.get_cpu();
	}
};




#endif //MICROMACHINE_TESTSYSTEM_HPP
