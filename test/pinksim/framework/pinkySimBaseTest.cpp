/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#include "pinkySimBaseTest.h"

#define RETURN_TO_THREAD_FROM_SP_MAIN 0xFFFFFFF9

static uint32_t interrupt_handler_address(uint32_t exception_number) {
	uint32_t offset = exception_number * sizeof(uint32_t);
	return (100U + offset);
}

void pinkySimBase::SimpleMemory_SetMemory(IMemory* pMem, uint32_t address, uint32_t value, int readOnly) {
	_cpu.mem().write32(address, value);
}

uint32_t pinkySimBase::IMemory_Read32(IMemory* pThis, uint32_t address) {
	return _cpu.mem().read32(address);
}

void pinkySimBase::pinkySimStep(PinkySimContext* ctx) {
	// unused method.
	// here to satisfy pinkysim test files
	pinkySimStep();
}

void pinkySimBase::setup() {
	// unused method.
	// here to satisfy pinkysim test files
	assert(false);
}

void pinkySimBase::teardown() {
	// unused method.
	// here to satisfy pinkysim test files
	assert(false);
}

void pinkySimBase::SetUp() {

	_host_mem = (uint8_t*) malloc(mem_size);

	// map host mem
	_cpu.mem().map(_host_mem, 0, mem_size);

	initContext();

}

void pinkySimBase::initContext() {

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
	PRIMASK = 0;

	m_expectedStepReturn = PINKYSIM_STEP_OK;

	// fake vector table
	for(uint32_t i = 1; i < 32; i++) {
		uint32_t vt_offset = i * sizeof(uint32_t);
		_cpu.mem().write32(vt_offset, interrupt_handler_address(i) | 1);
	}
	_cpu.mem().write32(INITIAL_PC, 0);

	/* By default we will place the processor in Thumb mode. */
	_cpu.regs().execution_status_register().set_thumb_bit(true);
	// all tests are expected to run in thumb mode
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
			_cpu.regs().xpsr_register().set_bit(bit);
			m_expectedXPSRflags |= (1 << bit);
		}
		else
		{
			_cpu.regs().xpsr_register().clear_bit(bit);
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

void pinkySimBase::TearDown() {
	if(_host_mem) {
		free(_host_mem);
		_host_mem = nullptr;
	}
}

void pinkySimBase::setExpectedStackGrowthSinceBeginning(int growth) {
	setExpectedSPMain(INITIAL_SP - growth);
}

void pinkySimBase::setExpectedExceptionTaken(int exceptionNumber) {
	setExpectedXPSRflags("T");
	setExpectedStepReturn(exceptionNumber);
	setExpectedIPSR(exceptionNumber);
	setExpectedStackGrowthSinceBeginning(32); // taking an exception wil save context on the stack (32 words)
	setExpectedRegisterValue(LR, RETURN_TO_THREAD_FROM_SP_MAIN); // return to thread using main stack
	setExpectedRegisterValue(PC, interrupt_handler_address(exceptionNumber));
}

void pinkySimBase::setExpectedStepReturn(int expectedStepReturn)
{
	m_expectedStepReturn = expectedStepReturn;
}

void pinkySimBase::setExpectedSPMain(uint32_t sp) {
	m_expectedSPmain = sp;
}

void pinkySimBase::setExpectedXPSRflags(const char* pExpectedFlags)
{
	// Remember what expected APSR flags should be after instruction execution and flip initial flag state to make
	// sure that simular correctly flips the state and doesn't just get lucky to match a pre-existing condition.
	//uint32_t apsr = 0;
	while (*pExpectedFlags)
	{
		switch (*pExpectedFlags)
		{
		case 'n':
			m_expectedXPSRflags &= ~APSR_N;
			_cpu.regs().app_status_register().write_neg_flag(true);
			//apsr |= APSR_N;
			break;
		case 'N':
			m_expectedXPSRflags |= APSR_N;
			_cpu.regs().app_status_register().write_neg_flag(false);
			//apsr &= ~APSR_N;
			break;
		case 'z':
			m_expectedXPSRflags &= ~APSR_Z;
			_cpu.regs().app_status_register().write_zero_flag(true);
			//apsr |= APSR_Z;
			break;
		case 'Z':
			m_expectedXPSRflags |= APSR_Z;
			_cpu.regs().app_status_register().write_zero_flag(false);
			//apsr &= ~APSR_Z;
			break;
		case 'c':
			m_expectedXPSRflags &= ~APSR_C;
			_cpu.regs().app_status_register().write_carry_flag(true);
			//apsr |= APSR_C;
			break;
		case 'C':
			m_expectedXPSRflags |= APSR_C;
			_cpu.regs().app_status_register().write_carry_flag(false);
			//apsr &= ~APSR_C;
			break;
		case 'v':
			m_expectedXPSRflags &= ~APSR_V;
			_cpu.regs().app_status_register().write_overflow_flag(true);
			//apsr |= APSR_V;
			break;
		case 'V':
			m_expectedXPSRflags |= APSR_V;
			_cpu.regs().app_status_register().write_overflow_flag(false);
			//apsr &= ~APSR_V;
			break;
		case 't':
			m_expectedXPSRflags &= ~EPSR_T;
			break;
		case 'T':
			m_expectedXPSRflags |= EPSR_T;
			break;
		}

		pExpectedFlags++;
	}


}

void pinkySimBase::setExpectedIPSR(uint32_t expectedValue)
{
	m_expectedIPSR = expectedValue;
}

void pinkySimBase::setExpectedRegisterValue(int index, uint32_t expectedValue)
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

void pinkySimBase::setRegisterValue(int index, uint32_t value)
{
	assert (index >= 0 && index <= PC);

	setExpectedRegisterValue(index, value);
	_cpu.regs().set(index, value);

	if (index == PC)
	{
		setExpectedRegisterValue(index, value + 2);

	}

}



void pinkySimBase::emitInstruction16(const char* pEncoding, ...)
{
	va_list     valist;

	va_start(valist, pEncoding);
	emitInstruction16Varg(pEncoding, valist);
	va_end(valist);
}

void pinkySimBase::emitInstruction32(const char* pEncoding1, const char* pEncoding2, ...)
{
	va_list     valist;

	va_start(valist, pEncoding2);
	emitInstruction16Varg(pEncoding1, valist);
	emitInstruction16Varg(pEncoding2, valist);
	va_end(valist);

	setExpectedRegisterValue(PC, INITIAL_PC + 4);
}

void pinkySimBase::emitInstruction16Varg(const char* pEncoding, va_list valist)
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

void pinkySimBase::pinkySimStep()
{
	_cpu.regs().primask_register() = PRIMASK;
	_cpu.step();
	validateXPSR();
	validateRegisters();
	validateSignaledException();
	PRIMASK = _cpu.regs().primask_register();
}

void pinkySimBase::validateSignaledException() {
	if(PINKYSIM_STEP_HARDFAULT == m_expectedStepReturn) {
		EXPECT_TRUE(_cpu.exceptions().is_active(exception_number::ex_name::HARDFAULT));
	} else {
		assert("TODO implement");
	}
}

void pinkySimBase::validateXPSR()
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
		(_cpu.regs().xpsr_register() & APSR_N) ? 'N' : 'n',
		(_cpu.regs().xpsr_register() & APSR_Z) ? 'Z' : 'z',
		(_cpu.regs().xpsr_register() & APSR_C) ? 'C' : 'c',
		(_cpu.regs().xpsr_register() & APSR_V) ? 'V' : 'v',
		(_cpu.regs().xpsr_register() & EPSR_T) ? 'T' : 't',
		0
	};

	//fprintf(stderr, "%s, %s\n", expectedFlagsStr, currentFlagsStr);

	EXPECT_STREQ(expectedFlagsStr, currentFlagsStr);
	EXPECT_EQ(m_expectedXPSRflags, _cpu.regs().xpsr_register() & (APSR_NZCV | EPSR_T));
	EXPECT_EQ(m_expectedIPSR, _cpu.regs().xpsr_register() & IPSR_MASK);
}

void pinkySimBase::validateRegisters()
{
	for (int i = 0 ; i < 13 ; i++)
		EXPECT_EQ(m_expectedRegisterValues[i], _cpu.regs().get(i));
	EXPECT_EQ(m_expectedSPmain, _cpu.regs().sp_register().get_specific_banked_sp(sp_reg::StackType::Main));
	EXPECT_EQ(m_expectedLR, _cpu.regs().get_lr());
	EXPECT_EQ(m_expectedPC, _cpu.regs().get_pc());
}

void pinkySimBase::setCarry()
{
	_cpu.regs().app_status_register().write_carry_flag(true);
}

void pinkySimBase::clearCarry()
{
	_cpu.regs().app_status_register().write_carry_flag(false);
}

void pinkySimBase::setZero()
{
	_cpu.regs().app_status_register().write_zero_flag(true);
}

void pinkySimBase::clearZero()
{
	_cpu.regs().app_status_register().write_zero_flag(false);
}

void pinkySimBase::setNegative()
{
	_cpu.regs().app_status_register().write_neg_flag(true);
}

void pinkySimBase::clearNegative()
{
	_cpu.regs().app_status_register().write_neg_flag(false);
}

void pinkySimBase::setOverflow()
{
	_cpu.regs().app_status_register().write_overflow_flag(true);
}

void pinkySimBase::clearOverflow()
{
	_cpu.regs().app_status_register().write_overflow_flag(false);
}

void pinkySimBase::setIPSR(uint32_t ipsr)
{
	_cpu.regs().xpsr_register() = (_cpu.regs().xpsr_register() & ~IPSR_MASK) | (ipsr & IPSR_MASK);
}
