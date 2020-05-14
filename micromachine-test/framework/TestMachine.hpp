/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#pragma once

#include <array>
#include <vector>
#include <memory>

namespace micromachine::system {
	class mcu;
	class cpu;
}

#include "exception/exception.hpp"

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
	std::unique_ptr<micromachine::system::mcu> _mcu;
	void initializeInterruptVectorTableWithStubs();

public:
	TestMachine();
	TestMachine(const TestMachine& other);
	const micromachine::system::mcu& getMCU() const;
	micromachine::system::mcu& getMCU();
	const micromachine::system::cpu& getCpu() const;
	micromachine::system::cpu& getCpu();
	uint32_t exceptionHandlerAddress(micromachine::system::exception exception) const;
};

}
