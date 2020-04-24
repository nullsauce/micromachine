/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#pragma once

#include "cpu.hpp"
#include "mcu.hpp"

#include <array>

namespace micromachine::testing {

/**
 * A machine is a mpu + memory
 */
class TestMachine {
public:
	static constexpr size_t MEMORY_SIZE = 0x00008000;
	static constexpr uint32_t INITIAL_SP = 0x00002000;
	static constexpr uint32_t INITIAL_PC = 0x00001000;

private:
	std::vector<uint8_t> _memoryStorage;
	micromachine::system::mcu _mcu;

	void initializeInterruptVectorTableWithStubs() {
		constexpr uint32_t handlers_base = 0x3000;
		constexpr uint32_t fake_handler_size = 0x10;

		// note that these stub handlers are not valid code
		for(uint32_t i = 1; i < _mcu.get_cpu().exceptions().supported_exception_count(); ++i) {
			uint32_t handler_address = handlers_base + (i * fake_handler_size);
			_mcu.get_memory().write32(i * sizeof(uint32_t), handler_address | 1);
		}
	}

public:
	TestMachine()
		: _memoryStorage(MEMORY_SIZE) {
		// Maps host to virtual memory
		// There's only one segment in the test machine
		_mcu.get_memory().map(_memoryStorage.data(), 0, _memoryStorage.size());

		// zero memory
		std::fill(_memoryStorage.begin(), _memoryStorage.end(), 0);

		// write SP to memory address 0x0
		((uint32_t*)_memoryStorage.data())[0] = INITIAL_SP;

		initializeInterruptVectorTableWithStubs();

		// initialize the machine
		_mcu.reset(INITIAL_PC);
	};

	TestMachine(const TestMachine& other)
		: _memoryStorage(other._memoryStorage)
		, _mcu(other._mcu) {
		// Map the existing memory
		_mcu.get_memory().map(_memoryStorage.data(), 0, _memoryStorage.size());
	}

	const micromachine::system::mcu& getMCU() const {
		return _mcu;
	}

	micromachine::system::mcu& getMCU() {
		return _mcu;
	}

	const micromachine::system::cpu& getCpu() const {
		return _mcu.get_cpu();
	}

	micromachine::system::cpu& getCpu() {
		return _mcu.get_cpu();
	}

	uint32_t exceptionHandlerAddress(exception::Type exception) const {
		uint32_t address = _mcu.get_memory().read32_unchecked(exception * sizeof(uint32_t));
		return address & ~1;
	}
};

}
