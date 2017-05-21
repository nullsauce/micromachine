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

#include "pinkysim_glue.hpp"

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

struct PinkySimContext {
	void* pMemory;
};

class pinkySimBase : public ::testing::Test
{
protected:
    int             m_expectedStepReturn;
    uint32_t        m_expectedXPSRflags;
    uint32_t        m_expectedRegisterValues[13];
    uint32_t        m_expectedSPmain;
    uint32_t        m_expectedLR;
    uint32_t        m_expectedPC;
    uint32_t        m_expectedIPSR;
    uint32_t        m_emitAddress;
	PinkySimContext m_context;
	cpu 			_cpu;
	uint8_t*		_host_mem;

	const size_t mem_size = 0x8000;

	using IMemory = void;
	void SimpleMemory_SetMemory(IMemory* pMem, uint32_t address, uint32_t value, int readOnly) {
		_cpu.mem().write32(address, value);
	}

	uint32_t IMemory_Read32(IMemory* pThis, uint32_t address) {
		return _cpu.mem().read32(address);
	}

	void pinkySimStep(PinkySimContext* ctx) {
		// unused method.
		// here to satisfy pinkysim test files
		pinkySimStep();
	}

	void setup() {
		// unused method.
		// here to satisfy pinkysim test files
		assert(false);
	}

	void teardown() {
		// unused method.
		// here to satisfy pinkysim test files
		assert(false);
	}

	virtual void SetUp() {

		_host_mem = (uint8_t*) malloc(mem_size);

		// map host mem
		_cpu.mem().map(_host_mem, 0, mem_size);

		initContext();

	}

	void initContext() {

		memset(_host_mem, 0, mem_size);

		_cpu.reset();

		m_expectedStepReturn = 0;
		m_expectedXPSRflags = 0;
		memset(m_expectedRegisterValues, 0,sizeof(uint32_t)*13);
		m_expectedSPmain = 0;
		m_expectedLR = 0;
		m_expectedPC = 0;
		m_expectedIPSR = 0;
		m_emitAddress = 0;

        m_expectedStepReturn = PINKYSIM_STEP_OK;

		_cpu.mem().write32(INITIAL_PC, 0);

		/* By default we will place the processor in Thumb mode. */
		_cpu.xpsr() = EPSR_T;
		m_expectedXPSRflags |= EPSR_T;

		/* Expect the interrupt number to be 0 by default. */
		setExpectedIPSR(0);

		/* Randomly initialize each APSR flag to help verify that the simulator doesn't clear/set a bit that the
		   specification indicates shouldn't be modified by an instruction. */
		for (uint32_t bit = 28 ; bit < 32 ; bit++)
		{
			int setOrClear = rand() & 1;
			if (setOrClear)
			{
				_cpu.xpsr().set_bit(bit);
				m_expectedXPSRflags |= (1 << bit);
			}
			else
			{
				_cpu.xpsr().clear_bit(bit);
				m_expectedXPSRflags &= ~(1 << bit);
			}
		}

		/* Place 0x11111111 in R1, 0x22222222 in R2, etc. */
		uint32_t value = 0;

		for (int i = 0 ; i < 13 ; i++)
		{
			_cpu.regs().set(i, value);
			m_expectedRegisterValues[i] = value;
			value += 0x11111111;
		}

		/* These defaults are values that would work on the LPC1768. */
		setRegisterValue(SP, INITIAL_SP);
		setRegisterValue(LR, INITIAL_LR);
		setRegisterValue(PC, INITIAL_PC);

		m_emitAddress = INITIAL_PC;

    }

	virtual void TearDown() {
		if(_host_mem) {
			free(_host_mem);
			_host_mem = nullptr;
		}
	}

    void setExpectedStepReturn(int expectedStepReturn)
    {
        m_expectedStepReturn = expectedStepReturn;
    }

    void setExpectedXPSRflags(const char* pExpectedFlags)
    {
        // Remember what expected APSR flags should be after instruction execution and flip initial flag state to make
        // sure that simular correctly flips the state and doesn't just get lucky to match a pre-existing condition.
        //uint32_t xpsr = 0;
		while (*pExpectedFlags)
        {
            switch (*pExpectedFlags)
            {
            case 'n':
                m_expectedXPSRflags &= ~APSR_N;
				_cpu.xpsr().write_neg_flag(false);
				//xpsr |= APSR_N;
                break;
            case 'N':
                m_expectedXPSRflags |= APSR_N;
				_cpu.xpsr().write_neg_flag(true);
				//xpsr &= ~APSR_N;
                break;
            case 'z':
                m_expectedXPSRflags &= ~APSR_Z;
				_cpu.xpsr().write_zero_flag(false);
				//xpsr |= APSR_Z;
                break;
            case 'Z':
                m_expectedXPSRflags |= APSR_Z;
				_cpu.xpsr().write_zero_flag(true);
				//xpsr &= ~APSR_Z;
                break;
            case 'c':
                m_expectedXPSRflags &= ~APSR_C;
				_cpu.xpsr().write_carry_flag(false);
				//xpsr |= APSR_C;
                break;
            case 'C':
                m_expectedXPSRflags |= APSR_C;
				_cpu.xpsr().write_carry_flag(true);
                //xpsr &= ~APSR_C;
                break;
            case 'v':
                m_expectedXPSRflags &= ~APSR_V;
				_cpu.xpsr().write_overflow_flag(false);
				//xpsr |= APSR_V;
                break;
            case 'V':
                m_expectedXPSRflags |= APSR_V;
				_cpu.xpsr().write_overflow_flag(true);
				//xpsr &= ~APSR_V;
                break;
            case 't':
				//xpsr &= ~EPSR_T;
                break;
            case 'T':
				//xpsr |= EPSR_T;
                break;
            }

            pExpectedFlags++;
        }


    }

    void setExpectedIPSR(uint32_t expectedValue)
    {
        m_expectedIPSR = expectedValue;
    }

    void setExpectedRegisterValue(int index, uint32_t expectedValue)
    {
        assert (index >= 0 && index <= PC);

        if (index == PC)
            m_expectedPC = expectedValue;
        else if (index == LR)
            m_expectedLR = expectedValue;
        else if (index == SP)
            m_expectedSPmain = expectedValue;
        else
            m_expectedRegisterValues[index] = expectedValue;
    }

    void setRegisterValue(int index, uint32_t value)
    {
        assert (index >= 0 && index <= PC);

        setExpectedRegisterValue(index, value);
		_cpu.regs().set(index, value);

        if (index == PC)
        {
            setExpectedRegisterValue(index, value + 2);

        }

    }



    void emitInstruction16(const char* pEncoding, ...)
    {
        va_list     valist;

        va_start(valist, pEncoding);
        emitInstruction16Varg(pEncoding, valist);
        va_end(valist);
    }

    void emitInstruction32(const char* pEncoding1, const char* pEncoding2, ...)
    {
        va_list     valist;

        va_start(valist, pEncoding2);
        emitInstruction16Varg(pEncoding1, valist);
        emitInstruction16Varg(pEncoding2, valist);
        va_end(valist);

        setExpectedRegisterValue(PC, INITIAL_PC + 4);
    }

    void emitInstruction16Varg(const char* pEncoding, va_list valist)
    {
        uint16_t    instr = 0;
        size_t      i = 0;
        char        last = '\0';
        const char* p;
        struct Field
        {
            uint32_t value;
            char     c;
        } fields[6];

        assert (16 == strlen(pEncoding));
        memset(fields, 0, sizeof(fields));

        // Go through pEncoding from left to right and find all fields to be inserted.
        p = pEncoding;
        while (*p)
        {
            char c = *p++;

            if (c != '1' && c != '0' && c != last)
            {
                // Determine if we already saw this field earlier.
                bool found = false;
                for (size_t j = 0 ; j < i ; j++)
                {
                    if (fields[j].c == c)
                        found = true;
                }

                // If this is the first time we have seen the field, then save its value in fields array.
                if (!found)
                {
                    assert (i < sizeof(fields)/sizeof(fields[0]));

                    fields[i].value = va_arg(valist, uint32_t);
                    fields[i].c = c;
                    last = c;
                    i++;
                }
            }
        }

        // Go through pEncoding again from right to left and insert field bits.
        p = pEncoding + 15;
        while (p >= pEncoding)
        {
            char c = *p--;

            instr >>= 1;

            if (c == '1')
            {
                instr |= (1 << 15);
            }
            else if (c == '0')
            {
                instr |= (0 << 15);
            }
            else
            {
                size_t j;
                for (j = 0 ; j < i ; j++)
                {
                    if (fields[j].c == c)
                        break;
                }
                assert (j != i);

                instr |= (fields[j].value & 1) << 15;
                fields[j].value >>= 1;
            }
        }

		_cpu.mem().write16(m_emitAddress, instr);
        m_emitAddress += 2;
    }

    void pinkySimStep()
    {
        _cpu.step();
        validateXPSR();
        validateRegisters();
		validateSignaledException();
    }

	void validateSignaledException() {
		if(PINKYSIM_STEP_HARDFAULT == m_expectedStepReturn) {
			EXPECT_TRUE(_cpu.active_exceptions().is_signaled(exception::HARDFAULT));
		} else {
			assert("TODO implement");
		}
	}

    void validateXPSR()
    {

		char expectedFlagsStr[6] = {
			(m_expectedXPSRflags & APSR_N) ? 'N' : 'n',
			(m_expectedXPSRflags & APSR_Z) ? 'Z' : 'z',
			(m_expectedXPSRflags & APSR_C) ? 'C' : 'c',
			(m_expectedXPSRflags & APSR_V) ? 'V' : 'v',
			(m_expectedXPSRflags & EPSR_T) ? 'T' : 't',
			0
		};

		char currentFlagsStr[6] = {
			(_cpu.xpsr() & APSR_N) ? 'N' : 'n',
			(_cpu.xpsr() & APSR_Z) ? 'Z' : 'z',
			(_cpu.xpsr() & APSR_C) ? 'C' : 'c',
			(_cpu.xpsr() & APSR_V) ? 'V' : 'v',
			(_cpu.xpsr() & EPSR_T) ? 'T' : 't',
			0
		};

		//fprintf(stderr, "%s, %s\n", expectedFlagsStr, currentFlagsStr);

		EXPECT_STREQ(expectedFlagsStr, currentFlagsStr);
        EXPECT_EQ(m_expectedXPSRflags, _cpu.xpsr() & (APSR_NZCV | EPSR_T));
		EXPECT_EQ(m_expectedIPSR, _cpu.xpsr() & IPSR_MASK);
    }

    void validateRegisters()
    {
        for (int i = 0 ; i < 13 ; i++)
			EXPECT_EQ(m_expectedRegisterValues[i], _cpu.regs().get(i));
		EXPECT_EQ(m_expectedSPmain, _cpu.regs().get_sp());
		EXPECT_EQ(m_expectedLR, _cpu.regs().get_lr());
		EXPECT_EQ(m_expectedPC, _cpu.regs().get_pc());
    }

    void setCarry()
    {
		_cpu.flags().write_carry_flag(true);
    }

    void clearCarry()
    {
		_cpu.flags().write_carry_flag(false);
    }

    void setZero()
    {
		_cpu.flags().write_zero_flag(true);
    }

    void clearZero()
    {
		_cpu.flags().write_zero_flag(false);
    }

    void setNegative()
    {
		_cpu.flags().write_neg_flag(true);
    }

    void clearNegative()
    {
		_cpu.flags().write_neg_flag(false);
    }

    void setOverflow()
    {
		_cpu.flags().write_overflow_flag(true);
    }

    void clearOverflow()
    {
		_cpu.flags().write_overflow_flag(false);
    }

    void setIPSR(uint32_t ipsr)
    {
		_cpu.xpsr() = (_cpu.xpsr() & ~IPSR_MASK) | (ipsr & IPSR_MASK);
    }
};
