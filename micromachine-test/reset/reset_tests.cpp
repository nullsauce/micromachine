
#include "CpuTestFixture.hpp"
#include "exception/exception.hpp"
#include "registers/core_registers/core_registers.hpp"

using namespace micromachine::testing;


MICROMACHINE_TEST_F(ResetBehavior, GeneralPurposeRegistersAreSetTozero, CpuTestFixture) {

	resetMachine(0);

	CheckThat(
		reg(registers::R0).equals(0),
		reg(registers::R1).equals(0),
		reg(registers::R2).equals(0),
		reg(registers::R3).equals(0),
		reg(registers::R4).equals(0),
		reg(registers::R5).equals(0),
		reg(registers::R6).equals(0),
		reg(registers::R7).equals(0),
		reg(registers::R8).equals(0),
		reg(registers::R9).equals(0),
		reg(registers::R10).equals(0),
		reg(registers::R11).equals(0),
		reg(registers::R12).equals(0)
	);
}

MICROMACHINE_TEST_F(ResetBehavior, PcIsSetToGivenPosition, CpuTestFixture) {
	setReg(registers::PC, 0x10);
	resetMachine(0x20);
	CheckThat(reg(registers::PC).equals(0x20));
}

MICROMACHINE_TEST_F(ResetBehavior, LrIsReset, CpuTestFixture) {
	setReg(registers::LR, 0xdeed00aa);
	resetMachine(0x20);
	CheckThat(reg(registers::LR).equals(0));
}

MICROMACHINE_TEST_F(ResetBehavior, PendingInternalExceptionIsCleared, CpuTestFixture) {
	raiseHardfault();
	resetMachine(0x20);
	CheckThat(noExceptionIsActiveOrPending());
}

MICROMACHINE_TEST_F(ResetBehavior, ActiveInternalExceptionIsCleared, CpuTestFixture) {
	raiseHardfault();
	resetMachine(0x20);
	CheckThat(noExceptionIsActiveOrPending());
}

MICROMACHINE_TEST_F(ResetBehavior, PendingExternalInterruptIsCleared, CpuTestFixture) {
	raiseExternalInterrupt(exception::EXTI_07);
	resetMachine(0x20);
	CheckThat(noExceptionIsActiveOrPending());
}

MICROMACHINE_TEST_F(ResetBehavior, ActiveExternalInterruptIsCleared, CpuTestFixture) {
	enabledAndRaiseExternalInterrupt(exception::EXTI_07);
	resetMachine(0x20);
	CheckThat(noExceptionIsActiveOrPending());
}

MICROMACHINE_TEST_F(ResetBehavior, InterruptPrioritiesAreReset, CpuTestFixture) {
	setExceptionPriority(exception::EXTI_07, 3);
	setExceptionPriority(exception::SVCALL, 1);
	resetMachine(0x20);

	CheckThat(exceptionStateOf(exception::EXTI_07).hasPriorityOf(0),
			  exceptionStateOf(exception::SVCALL).hasPriorityOf(0),
			  exceptionStateOf(exception::RESET).hasPriorityOf(exception::RESET_PRIORITY),
			  exceptionStateOf(exception::NMI).hasPriorityOf(exception::NMI_PRIORITY),
			  exceptionStateOf(exception::HARDFAULT).hasPriorityOf(exception::HARDFAULT_PRIORITY),
		noExceptionIsActiveOrPending()
	);
}

MICROMACHINE_TEST_F(ResetBehavior, XpsrIsCleared, CpuTestFixture) {
	// TODO: must pass setXPSR(0xffffffff) and rely on XPSR ignoring RAZ bits.
	setXPSR(0b11111111111111111111110111111111);
	resetMachine(0x20);
	CheckThat(apsrFlagsEquals("nzcv"), thumbFlagIsSet(), noExceptionIsActiveOrPending());
}

MICROMACHINE_TEST_F(ResetBehavior, ExecModeIsSetToThreadMode, CpuTestFixture) {
	enterHandlerMode();
	resetMachine(0x20);
	CheckThat(executionModeIsThread(), noExceptionIsPending());
}
