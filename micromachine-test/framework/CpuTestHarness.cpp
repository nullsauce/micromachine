/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#include "CpuTestHarness.hpp"
#include "exception/exception.hpp"
#include "helpers/binops.hpp"

#define RETURN_TO_THREAD_FROM_SP_MAIN 0xFFFFFFF9

using namespace micromachine::system;

uint32_t CpuTestHarness::interrupt_handler_address(uint32_t exception_number)
{
	uint32_t vector_table_offset = exception_number * sizeof(uint32_t);
	// clears the thumb bit from the address
	uint32_t address = _mcu.get_memory().read32(vector_table_offset) & ~1;
	return address;
}

void CpuTestHarness::memory_write_32(uint32_t address, uint32_t value)
{
	_mcu.get_memory().write32(address, value);
}

uint32_t CpuTestHarness::memory_read_32(uint32_t address)
{
	return _mcu.get_memory().read32(address);
}

void CpuTestHarness::step()
{
	// unused method.
	// here to satisfy pinkysim test files
	pinkySimStep();
}

void CpuTestHarness::SetUp()
{

	// allocate memory
	_memory.resize(MEMORY_SIZE);

	// map host memory
	_mcu.get_memory().map(_memory.data(), 0, _memory.size());

	// expose cpu memory to assembler
	_code_gen.setMem(&_mcu.get_memory());

	initContext();
}

void CpuTestHarness::initContext()
{

	// zero memory
	std::fill(_memory.begin(), _memory.end(), 0);

	_mcu.reset(INITIAL_PC);

	// assembler will start emiting instructions at the initial code position
	_code_gen.setWriteAddress(INITIAL_PC);

	m_expectedStepReturn = 0;
	m_expectedXPSRflags = 0;
	std::fill(m_expectedRegisterValues.begin(), m_expectedRegisterValues.end(), 0);
	m_expectedSPmain = 0;
	m_expectedLR = 0;
	m_expectedPC = 0;
	m_expectedIPSR = 0;
	PRIMASK = 0;

	m_expectedStepReturn = CPU_STEP_OK;

	// fake vector table
	for (uint32_t i = 1; i < 32; i++) {
		uint32_t vt_offset = i * sizeof(uint32_t);
		_mcu.get_memory().write32(vt_offset, (0x1000 + (vt_offset * 32)) | 1);
	}
	_mcu.get_memory().write32(INITIAL_PC, 0);

	// all tests are expected to run in thumb mode
	m_expectedXPSRflags |= EPSR_T;

	/* Expect the interrupt number to be 0 by default. */
	setExpectedIPSR(0);

	/* Randomly initialize each APSR flag to help verify that the simulator doesn't clear/set a bit that the
	   specification indicates shouldn't be modified by an instruction. */
	for (uint32_t bit = 28; bit < 32; bit++) {
		int setOrClear = rand() & 1;
		if (setOrClear) {
			binops::set_bit(
				_mcu.get_cpu().special_regs().xpsr_register(), bit);
			m_expectedXPSRflags |= (1 << bit);
		} else {
			binops::clear_bit(
				_mcu.get_cpu().special_regs().xpsr_register(), bit);
			m_expectedXPSRflags &= ~(1 << bit);
		}
	}

	/* Place 0x11111111 in R1, 0x22222222 in R2, etc. */
	uint32_t value = 0;

	for (int i = 0; i < 13; i++) {
		_mcu.get_cpu().regs().set(i, value);
		m_expectedRegisterValues[i] = value;
		value += 0x11111111;
	}

	/* These defaults are values that would work on the LPC1768. */
	setRegisterValue(registers::SP, INITIAL_SP);
	setRegisterValue(registers::LR, INITIAL_LR);
	setRegisterValue(registers::PC, INITIAL_PC);
}

void CpuTestHarness::TearDown()
{

}

void CpuTestHarness::setExpectedStackGrowthSinceBeginning(int growth)
{
	setExpectedSPMain(INITIAL_SP - growth);
}

void CpuTestHarness::setExpectedExceptionTaken(int exceptionNumber)
{
	setExpectedXPSRflags("T");
	setExpectedIPSR(exceptionNumber);
	setExpectedStackGrowthSinceBeginning(32); // taking an exception wil save context on the stack (32 words)
	setExpectedRegisterValue(registers::LR, RETURN_TO_THREAD_FROM_SP_MAIN); // return to thread using main stack
	uint32_t handler_address = interrupt_handler_address(exceptionNumber);
	setExpectedRegisterValue(registers::PC, handler_address);
}

void CpuTestHarness::setExpectNoExceptionTaken() {
	setExpectedIPSR(0);
}

void CpuTestHarness::setExpectedStepReturn(int expectedStepReturn)
{
	m_expectedStepReturn = expectedStepReturn;
}

void CpuTestHarness::setExpectedSPMain(uint32_t sp)
{
	m_expectedSPmain = sp;
}

void CpuTestHarness::setExpectedXPSRflags(const char *pExpectedFlags)
{
	// Remember what expected APSR flags should be after instruction execution and flip initial flag state to make
	// sure that simular correctly flips the state and doesn't just get lucky to match a pre-existing condition.
	//uint32_t apsr = 0;
	while (*pExpectedFlags) {
		switch (*pExpectedFlags) {
			case 'n':
				m_expectedXPSRflags &= ~APSR_N;
				_mcu.get_cpu().special_regs().app_status_register().write_neg_flag(true);
				//apsr |= APSR_N;
				break;
			case 'N':
				m_expectedXPSRflags |= APSR_N;
				_mcu.get_cpu().special_regs().app_status_register().write_neg_flag(false);
				//apsr &= ~APSR_N;
				break;
			case 'z':
				m_expectedXPSRflags &= ~APSR_Z;
				_mcu.get_cpu().special_regs().app_status_register().write_zero_flag(true);
				//apsr |= APSR_Z;
				break;
			case 'Z':
				m_expectedXPSRflags |= APSR_Z;
				_mcu.get_cpu().special_regs().app_status_register().write_zero_flag(false);
				//apsr &= ~APSR_Z;
				break;
			case 'c':
				m_expectedXPSRflags &= ~APSR_C;
				_mcu.get_cpu().special_regs().app_status_register().write_carry_flag(true);
				//apsr |= APSR_C;
				break;
			case 'C':
				m_expectedXPSRflags |= APSR_C;
				_mcu.get_cpu().special_regs().app_status_register().write_carry_flag(false);
				//apsr &= ~APSR_C;
				break;
			case 'v':
				m_expectedXPSRflags &= ~APSR_V;
				_mcu.get_cpu().special_regs().app_status_register().write_overflow_flag(true);
				//apsr |= APSR_V;
				break;
			case 'V':
				m_expectedXPSRflags |= APSR_V;
				_mcu.get_cpu().special_regs().app_status_register().write_overflow_flag(false);
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

void CpuTestHarness::setExpectedIPSR(uint32_t expectedValue)
{
	m_expectedIPSR = expectedValue;
}

void CpuTestHarness::setExpectedRegisterValue(reg_idx index, uint32_t expectedValue)
{
	assert(index <= registers::PC);

	if (index == registers::PC)
		m_expectedPC = expectedValue;
	else if (index == registers::LR)
		m_expectedLR = expectedValue;
	else if (index == registers::SP)
		m_expectedSPmain = expectedValue;
	else
		m_expectedRegisterValues[index] = expectedValue;
}

void CpuTestHarness::setRegisterValue(reg_idx index, uint32_t value)
{
	assert (index <= registers::PC);

	setExpectedRegisterValue(index, value);
	_mcu.get_cpu().regs().set(index, value);

	if (index == registers::PC) {
		setExpectedRegisterValue(index, value + 2);
	}
}

void CpuTestHarness::pinkySimStep()
{
	// TODO: why do we have a PRIMASK variable here ?
	_mcu.get_cpu().special_regs().primask_register() = PRIMASK;
	_mcu.step();
	validateXPSR();
	validateRegisters();
	validateSignaledException();
	PRIMASK = _mcu.get_cpu().special_regs().primask_register();
}

void CpuTestHarness::validateSignaledException()
{
	if (CPU_STEP_HARDFAULT == m_expectedStepReturn) {
		EXPECT_TRUE(_mcu.get_cpu().exceptions().interrupt_state<exception::Type::HARDFAULT>().is_active());
	} else {
		assert("TODO implement");
	}
}

void CpuTestHarness::validateXPSR()
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
		(_mcu.get_cpu().special_regs().xpsr_register() & APSR_N) ? 'N' : 'n',
		(_mcu.get_cpu().special_regs().xpsr_register() & APSR_Z) ? 'Z' : 'z',
		(_mcu.get_cpu().special_regs().xpsr_register() & APSR_C) ? 'C' : 'c',
		(_mcu.get_cpu().special_regs().xpsr_register() & APSR_V) ? 'V' : 'v',
		(_mcu.get_cpu().special_regs().xpsr_register() & EPSR_T) ? 'T' : 't',
		0
	};

	//fprintf(stderr, "%s, %s\n", expectedFlagsStr, currentFlagsStr);

	EXPECT_STREQ(expectedFlagsStr, currentFlagsStr);
	EXPECT_EQ(m_expectedXPSRflags,
			  _mcu.get_cpu().special_regs().xpsr_register() & (APSR_NZCV | EPSR_T));
	EXPECT_EQ(m_expectedIPSR, _mcu.get_cpu().special_regs().xpsr_register() & IPSR_MASK);
}

void CpuTestHarness::validateRegisters()
{
	for (int i = 0; i < 13; i++)
		EXPECT_EQ(m_expectedRegisterValues[i], _mcu.get_cpu().regs().get(i));
	EXPECT_EQ(m_expectedSPmain,
			  _mcu.get_cpu().regs().sp_register().get_specific_banked_sp(sp_reg::StackType::Main));
	EXPECT_EQ(m_expectedLR, _mcu.get_cpu().regs().lr());
	EXPECT_EQ(m_expectedPC, _mcu.get_cpu().regs().pc());
}

void CpuTestHarness::setCarry()
{
	_mcu.get_cpu().special_regs().app_status_register().write_carry_flag(true);
}

void CpuTestHarness::clearCarry()
{
	_mcu.get_cpu().special_regs().app_status_register().write_carry_flag(false);
}

void CpuTestHarness::setZero()
{
	_mcu.get_cpu().special_regs().app_status_register().write_zero_flag(true);
}

void CpuTestHarness::clearZero()
{
	_mcu.get_cpu().special_regs().app_status_register().write_zero_flag(false);
}

void CpuTestHarness::setNegative()
{
	_mcu.get_cpu().special_regs().app_status_register().write_neg_flag(true);
}

void CpuTestHarness::clearNegative()
{
	_mcu.get_cpu().special_regs().app_status_register().write_neg_flag(false);
}

void CpuTestHarness::setOverflow()
{
	_mcu.get_cpu().special_regs().app_status_register().write_overflow_flag(true);
}

void CpuTestHarness::clearOverflow()
{
	_mcu.get_cpu().special_regs().app_status_register().write_overflow_flag(false);
}

void CpuTestHarness::setIPSR(uint32_t ipsr)
{
	_mcu.get_cpu().special_regs().xpsr_register() = (_mcu.get_cpu().special_regs().xpsr_register() & ~IPSR_MASK) | (ipsr & IPSR_MASK);
}
