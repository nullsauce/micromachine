//
// Created by fla on 17-5-8.
//


#include "registers/system_control/interrupt_control_state_reg.hpp"
#include "CpuTestFixture.hpp"

#include <gtest/gtest.h>

using namespace micromachine::testing;

static constexpr uint32_t ICSR_ADDR = 0xE000ED04;

MICROMACHINE_TEST_F(InterruptControlStateReg, SetNMIPendBitDoesSetNMIPendingFlag, CpuTestFixture) {
	memWrite32(ICSR_ADDR, 1 << 31);
	CheckThat(
		memoryAt(ICSR_ADDR).equals32(1 << 31),
		exceptionStateOf(exception::NMI).isPending()
	);
}

MICROMACHINE_TEST_F(InterruptControlStateReg, ClearNMIPendBitDoesNotClearNMIPendingFlag, CpuTestFixture) {
	raiseException(exception::NMI);
	memWrite32(ICSR_ADDR, 0);
	CheckThat(
		memoryAt(ICSR_ADDR).equals32(1 << 31),
		exceptionStateOf(exception::NMI).isPending()
	);
}

MICROMACHINE_TEST_F(InterruptControlStateReg, SetPendSVPendBitDoesSetPendSVPendingFlag, CpuTestFixture) {
	memWrite32(ICSR_ADDR, 1 << 28);
	CheckThat(
		memoryAt(ICSR_ADDR).equals32(1 << 28),
		exceptionStateOf(exception::PENDSV).isPending()
	);
}

MICROMACHINE_TEST_F(InterruptControlStateReg, SetPendSVClearBitDoesClearsPendSVPendingFlag, CpuTestFixture) {
	raiseException(exception::PENDSV);
	memWrite32(ICSR_ADDR, 1 << 27);
	CheckThat(
		memoryAt(ICSR_ADDR).equals32(0),
		exceptionStateOf(exception::PENDSV).isNotPending()
	);
}

MICROMACHINE_TEST_F(InterruptControlStateReg, ClearPendSVClearBitDoesNothing, CpuTestFixture) {
	raiseException(exception::PENDSV);
	memWrite32(ICSR_ADDR, 0);
	CheckThat(
		memoryAt(ICSR_ADDR).equals32(1 << 28),
		exceptionStateOf(exception::PENDSV).isPending()
	);
}

MICROMACHINE_TEST_F(InterruptControlStateReg, SetSystickPendBitDoesSetSystickPendingFlag, CpuTestFixture) {
	memWrite32(ICSR_ADDR, 1 << 26);
	CheckThat(
		memoryAt(ICSR_ADDR).equals32(1 << 26),
		exceptionStateOf(exception::SYSTICK).isPending()
	);
}

MICROMACHINE_TEST_F(InterruptControlStateReg, SetSystickClearBitDoesClearsSystickFlag, CpuTestFixture) {
	raiseException(exception::SYSTICK);
	memWrite32(ICSR_ADDR, 1 << 25);
	CheckThat(
		memoryAt(ICSR_ADDR).equals32(0),
		exceptionStateOf(exception::SYSTICK).isNotPending()
	);
}

MICROMACHINE_TEST_F(InterruptControlStateReg, ClearSystickClearBitDoesNothing, CpuTestFixture) {
	raiseException(exception::SYSTICK);
	memWrite32(ICSR_ADDR, 0);
	CheckThat(
		memoryAt(ICSR_ADDR).equals32(1 << 26),
		exceptionStateOf(exception::SYSTICK).isPending()
	);
}
