//
// Created by fla on 17-5-8.
//


#include "registers/system_control/icsr_reg.hpp"
#include "CpuTestFixture.hpp"

#include <gtest/gtest.h>

using namespace micromachine::testing;

static constexpr uint32_t ICSR_ADDR = 0xE000ED04;

static constexpr uint32_t NMI_PENDING_FLAG = 1 << 31;
static constexpr uint32_t PENDSV_PENDING_FLAG = 1 << 28;
static constexpr uint32_t PENDSV_CLEAR_FLAG = 1 << 27;
static constexpr uint32_t SYSTICK_PENDING_FLAG = 1 << 26;
static constexpr uint32_t SYSTICK_CLEAR_FLAG = 1 << 25;
static constexpr uint32_t TOP_EXCEPTION_OFFSET = 12;


MICROMACHINE_TEST_F(InterruptControlStateReg, VectPendingIndicatesHighestPriorityPendingException, CpuTestFixture) {
	raiseException(exception::SYSTICK);
	raiseException(exception::HARDFAULT);
	CheckThat(memoryAt(ICSR_ADDR).equals32(SYSTICK_PENDING_FLAG | exception::HARDFAULT << TOP_EXCEPTION_OFFSET),
			  exceptionStateOf(exception::SYSTICK).isPending(),
			  exceptionStateOf(exception::HARDFAULT).isPending());
}

MICROMACHINE_TEST_F(InterruptControlStateReg, VectActiveIndicatesActiveException, CpuTestFixture) {
	raiseException(exception::HARDFAULT);
	StepAndExpectThat(
		hardfaultHandlerReached(),
		memoryAt(ICSR_ADDR).equals32(exception::HARDFAULT));
}

MICROMACHINE_TEST_F(InterruptControlStateReg, VectActiveIndicatesHioghestPriorityActiveException, CpuTestFixture) {
	raiseExternalInterrupt(exception::EXTI_12);
	raiseException(exception::HARDFAULT);
	StepAndExpectThat(hardfaultHandlerReached(), memoryAt(ICSR_ADDR).equals32(exception::HARDFAULT));
}

MICROMACHINE_TEST_F(InterruptControlStateReg, SetNMIPendBitDoesSetNMIPendingFlag, CpuTestFixture) {
	memWrite32(ICSR_ADDR, NMI_PENDING_FLAG);
	CheckThat(
		memoryAt(ICSR_ADDR).equals32(NMI_PENDING_FLAG | exception::NMI << TOP_EXCEPTION_OFFSET),
		exceptionStateOf(exception::NMI).isPending()
	);
}

MICROMACHINE_TEST_F(InterruptControlStateReg, ClearNMIPendBitDoesNotClearNMIPendingFlag, CpuTestFixture) {
	raiseException(exception::NMI);
	memWrite32(ICSR_ADDR, 0);
	CheckThat(
		memoryAt(ICSR_ADDR).equals32(NMI_PENDING_FLAG | exception::NMI << TOP_EXCEPTION_OFFSET),
		exceptionStateOf(exception::NMI).isPending()
	);
}

MICROMACHINE_TEST_F(InterruptControlStateReg, SetPendSVPendBitDoesSetPendSVPendingFlag, CpuTestFixture) {
	memWrite32(ICSR_ADDR, PENDSV_PENDING_FLAG);
	CheckThat(
		memoryAt(ICSR_ADDR).equals32(PENDSV_PENDING_FLAG | exception::PENDSV << TOP_EXCEPTION_OFFSET),
		exceptionStateOf(exception::PENDSV).isPending()
	);
}

MICROMACHINE_TEST_F(InterruptControlStateReg, SetPendSVClearBitDoesClearsPendSVPendingFlag, CpuTestFixture) {
	raiseException(exception::PENDSV);
	memWrite32(ICSR_ADDR, PENDSV_CLEAR_FLAG);
	CheckThat(
		memoryAt(ICSR_ADDR).equals32(0),
		exceptionStateOf(exception::PENDSV).isNotPending()
	);
}

MICROMACHINE_TEST_F(InterruptControlStateReg, ClearPendSVClearBitDoesNothing, CpuTestFixture) {
	raiseException(exception::PENDSV);
	memWrite32(ICSR_ADDR, 0);
	CheckThat(
		memoryAt(ICSR_ADDR).equals32(PENDSV_PENDING_FLAG | exception::PENDSV << TOP_EXCEPTION_OFFSET),
		exceptionStateOf(exception::PENDSV).isPending()
	);
}

MICROMACHINE_TEST_F(InterruptControlStateReg, SetSystickPendBitDoesSetSystickPendingFlag, CpuTestFixture) {
	memWrite32(ICSR_ADDR, SYSTICK_PENDING_FLAG);
	CheckThat(
		memoryAt(ICSR_ADDR).equals32(SYSTICK_PENDING_FLAG | exception::SYSTICK << TOP_EXCEPTION_OFFSET),
		exceptionStateOf(exception::SYSTICK).isPending()
	);
}

MICROMACHINE_TEST_F(InterruptControlStateReg, SetSystickClearBitDoesClearSystickFlag, CpuTestFixture) {
	raiseException(exception::SYSTICK);
	memWrite32(ICSR_ADDR, SYSTICK_CLEAR_FLAG);
	CheckThat(
		memoryAt(ICSR_ADDR).equals32(0),
		exceptionStateOf(exception::SYSTICK).isNotPending()
	);
}

MICROMACHINE_TEST_F(InterruptControlStateReg, ClearSystickClearBitDoesNothing, CpuTestFixture) {
	raiseException(exception::SYSTICK);
	memWrite32(ICSR_ADDR, 0);
	CheckThat(
		memoryAt(ICSR_ADDR).equals32(SYSTICK_PENDING_FLAG | exception::SYSTICK << TOP_EXCEPTION_OFFSET),
		exceptionStateOf(exception::SYSTICK).isPending()
	);
}

