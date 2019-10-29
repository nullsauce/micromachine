/*  Copyright (C) 2014  Adam Green (https://github.com/adamgreen)

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/

#include <cstdint>


#include "cpu.hpp"
#include "pinksim.hpp"


// Include standard headers.
#include <assert.h>

// Include C++ headers for test harness.
//#include "CppUTest/TestHarness.h"
#include <gtest/gtest.h>

#include <stdarg.h>

// Initial values for special registers.
#define INITIAL_SP 0x00008000
#define INITIAL_LR 0xFFFFFFFF
#define INITIAL_PC 0x00004000


#define READ_WRITE 0
#define READ_ONLY  1

struct PinkySimContext
{
	void *pMemory;
};

class CpuTestHelper : public ::testing::Test
{
protected:
	const size_t MEMORY_SIZE = 0x8000;
	cpu _cpu;
	std::vector<uint8_t> _memory;

	int m_expectedStepReturn;
	uint32_t m_expectedXPSRflags;
	std::array<uint32_t, 13> m_expectedRegisterValues;
	uint32_t m_expectedSPmain;
	uint32_t m_expectedLR;
	uint32_t m_expectedPC;
	uint32_t m_expectedIPSR;
	uint32_t m_emitAddress;
	uint32_t PRIMASK;
	PinkySimContext m_context;


	using IMemory = void;

	void memory_write_32(uint32_t address, uint32_t value);

	uint32_t memory_read_32(uint32_t address);

	void step();

	void setup();

	void teardown();

	virtual void SetUp();

	void initContext();

	virtual void TearDown();

	void setExpectedStackGrowthSinceBeginning(int growth);

	void setExpectedExceptionTaken(int exceptionNumber);

	void setExpectedStepReturn(int expectedStepReturn);

	void setExpectedSPMain(uint32_t sp);

	void setExpectedXPSRflags(const char *pExpectedFlags);

	void setExpectedIPSR(uint32_t expectedValue);

	void setExpectedRegisterValue(int index, uint32_t expectedValue);

	void setRegisterValue(int index, uint32_t value);

	void emitInstruction16(const char *pEncoding, ...);

	void emitInstruction32(const char *pEncoding1, const char *pEncoding2, ...);

	void emitInstruction16Varg(const char *pEncoding, va_list valist);

	void pinkySimStep();

	void validateSignaledException();

	void validateXPSR();

	void validateRegisters();

	void setCarry();

	void clearCarry();

	void setZero();

	void clearZero();

	void setNegative();

	void clearNegative();

	void setOverflow();

	void clearOverflow();

	void setIPSR(uint32_t ipsr);
};
