#include <gtest/gtest.h>
#include <assert.h>

#include "cpu.hpp"


class CPUTest : public ::testing::Test {
protected:

	virtual void SetUp() {
		const size_t data_size = 1024;
		_host_mem = (uint8_t*)malloc(data_size);

		// taint each word
		uint32_t* data32 = (uint32_t*)_host_mem;
		for(uint32_t i = 0; i < data_size/sizeof(uint32_t); i++)
			data32[i] = i;

		// map host mem
		_cpu.mem().map(_host_mem, 0, data_size);
	}

	virtual void TearDown() {
		if(_host_mem) {
			free(_host_mem);
			_host_mem = nullptr;
		}
	}

	cpu _cpu;
	uint8_t* _host_mem  = nullptr;
};


TEST_F(CPUTest, TestExecLDR_NegativeOffset) {
	_cpu.regs().set(1, 0xffffffff);   // dest
	_cpu.regs().set_pc(14);

	// ldr r0 -8
	halfword ins = 0b0100100100000010;//   (0b01001 << 11 ) | (0b001 << 5) | (uint8_t)2;
	_cpu.dispatch_and_exec(ins);

	// pc should have aligned to 12
	EXPECT_EQ(5, _cpu.regs().get(1)); // mem[(12+(2<<2))] == 5

}