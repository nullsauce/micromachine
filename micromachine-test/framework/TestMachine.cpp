
#include "TestMachine.hpp"

#include "mcu.hpp"

namespace micromachine::testing {

void TestMachine::initializeInterruptVectorTableWithStubs() {
	constexpr uint32_t handlers_base = 0x3000;
	constexpr uint32_t fake_handler_size = 0x10;

	// note that these stub handlers are not valid code
	for(uint32_t i = 1; i < _mcu->exceptions().supported_exception_count(); ++i) {
		uint32_t handler_address = handlers_base + (i * fake_handler_size);
		_mcu->get_memory().write32(i * sizeof(uint32_t), handler_address | 1);
	}
}

TestMachine::TestMachine()
	: _memoryStorage(MEMORY_SIZE)
	, _mcu(std::make_unique<mcu>()){
	// Maps host to virtual memory
	// There's only one segment in the test machine
	_mcu->get_memory().map(_memoryStorage.data(), 0, _memoryStorage.size());

	// zero memory
	std::fill(_memoryStorage.begin(), _memoryStorage.end(), 0);

	// write SP to memory address 0x0
	((uint32_t*)_memoryStorage.data())[0] = INITIAL_SP;

	initializeInterruptVectorTableWithStubs();

	// initialize the machine
	_mcu->reset(INITIAL_PC);
}

TestMachine::TestMachine(const TestMachine& other)
	: _memoryStorage(other._memoryStorage)
	, _mcu(std::make_unique<mcu>(*other._mcu)) {
	// Map the existing memory
	_mcu->get_memory().map(_memoryStorage.data(), 0, _memoryStorage.size());
}

const micromachine::system::mcu& TestMachine::getMCU() const {
	return *_mcu;
}

micromachine::system::mcu& TestMachine::getMCU() {
	return *_mcu;
}

const micromachine::system::cpu& TestMachine::getCpu() const {
	return _mcu->get_cpu();
}

micromachine::system::cpu& TestMachine::getCpu() {
	return _mcu->get_cpu();
}

uint32_t TestMachine::exceptionHandlerAddress(micromachine::system::exception exception) const {
	uint32_t address = _mcu->get_memory().read32_unchecked(exception * sizeof(uint32_t));
	return address & ~1;
}

}

