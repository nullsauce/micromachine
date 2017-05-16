// @micromachine_project_header@
// Created by roth on 04.05.17.
//

#include <gtest/gtest.h>
#include <assert.h>

#include "../lib/cpu.hpp"


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
	_cpu.regs()[0] = 0;   // dest
	_cpu.regs()[1] = 10;  // base addr
	_cpu.regs()[2]	= -6; // offset

	// equivalent to *(uint32_t*)mem[10 + -6]
	exec_ldr(0, 1, 2, _cpu.regs(), _cpu.mem());

	EXPECT_EQ(1, _cpu.regs()[0]); // mem[4] == 1
	EXPECT_EQ(10,_cpu.regs()[1]); // didnt change
	EXPECT_EQ(-6,_cpu.regs()[2]); // didnt change
}