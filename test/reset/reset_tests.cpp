/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#include "CpuTestFixture.hpp"
#include "registers/core_registers.hpp"
#include "exception_defs.hpp"

using namespace micromachine::system;

class CpuResetTest : public CpuTestFixture {
protected:
	virtual void SetUp() override {
		CpuTestFixture::SetUp();
		for (reg_idx regIdx = 0; regIdx < micromachine::system::core_registers::NUM_GP_REGS; regIdx++) {
			getCpu().regs().set(regIdx, 0x11111111U * (1U + regIdx));
		}
	}
};

MICROMACHINE_TEST_F(ResetBehavior, GeneralPurposeRegistersAreSetTozero, CpuResetTest) {
	for (reg_idx regIdx = 0; regIdx < micromachine::system::core_registers::NUM_GP_REGS; regIdx++) {
		ExpectThat().Register(regIdx).Equals(0x11111111U * (1U + regIdx));
	}

	getSystem().reset(0);

	for (reg_idx regIdx = 0; regIdx < micromachine::system::core_registers::NUM_GP_REGS; regIdx++) {
		ExpectThat().Register(regIdx).Equals(0);
	}
}

MICROMACHINE_TEST_F(ResetBehavior, PcIsSetToGivenPosition, CpuResetTest) {
	getCpu().regs().set_pc(0x10);
	getSystem().reset(0x20);
	ExpectThat().Register(registers::PC).Equals(0x20);
}


MICROMACHINE_TEST_F(ResetBehavior, LrIsReset, CpuResetTest) {
	getCpu().regs().set_lr(0xdeed00aa);
	getSystem().reset(0x20);
	ExpectThat().Register(registers::LR).Equals(0);
}


MICROMACHINE_TEST_F(ResetBehavior, PendingInternalExceptionIsCleared, CpuResetTest) {
	getCpu().interrupt().raise_hardfault();
	getSystem().reset(0x20);
	ExpectThat().NoInterruptIsActiveOrPending();
}

MICROMACHINE_TEST_F(ResetBehavior, ActiveInternalExceptionIsCleared, CpuResetTest) {
	getCpu().interrupt().raise_hardfault();
	Step();
	ExpectThat().HardfaultHandlerReached();
	getSystem().reset(0x20);
	ExpectThat().NoInterruptIsActiveOrPending();
}

MICROMACHINE_TEST_F(ResetBehavior, PendingExternalInterruptIsCleared, CpuResetTest) {
	getCpu().interrupt().enable_external_interrupt(7);
	getCpu().interrupt().raise_external_interrupt(7);
	Step();
	ExpectThat().ExceptionHandlerReached(micromachine::system::exception::EXTI_07);
	getSystem().reset(0x20);
	ExpectThat().NoInterruptIsActiveOrPending();
}

MICROMACHINE_TEST_F(ResetBehavior, ActiveExternalInterruptIsCleared, CpuResetTest) {
	getCpu().interrupt().enable_external_interrupt(7);
	getCpu().interrupt().raise_external_interrupt(7);
	Step();
	ExpectThat().ExceptionHandlerReached(micromachine::system::exception::EXTI_07);
	getSystem().reset(0x20);
	ExpectThat().NoInterruptIsActiveOrPending();
}

MICROMACHINE_TEST_F(ResetBehavior, InterruptPrioritiesAreReset, CpuResetTest) {
	getCpu().interrupt().set_exception_priority<micromachine::system::exception::EXTI_07>(3);
	getCpu().interrupt().set_exception_priority<micromachine::system::exception::SVCALL>(1);
	getSystem().reset(0x20);
	EXPECT_EQ(0U, getCpu().interrupt().exception_priority<micromachine::system::exception::EXTI_07>());
	EXPECT_EQ(0U, getCpu().interrupt().exception_priority<micromachine::system::exception::SVCALL>());
	EXPECT_EQ(micromachine::system::exception::RESET_PRIORITY, getCpu().interrupt().exception_priority<micromachine::system::exception::RESET>());
	EXPECT_EQ(micromachine::system::exception::NMI_PRIORITY, getCpu().interrupt().exception_priority<micromachine::system::exception::NMI>());
	EXPECT_EQ(
		micromachine::system::exception::HARDFAULT_PRIORITY, getCpu().interrupt().exception_priority<micromachine::system::exception::HARDFAULT>());
	ExpectThat().NoInterruptIsActiveOrPending();
}

MICROMACHINE_TEST_F(ResetBehavior, XpsrIsCleared, CpuResetTest) {
	getCpu().special_regs().xpsr_register() = 0xffffffff;
	getSystem().reset(0x20);
	ExpectThat()
		.APSRFlagsMatches("nzcv")
		.ThumbBitIsSet()
		.NoInterruptIsActiveOrPending();
}

MICROMACHINE_TEST_F(ResetBehavior, ExecModeIsSetToThreadMode, CpuResetTest) {
	getCpu().set_execution_mode(execution_mode::handler);
	getSystem().reset(0x20);
	ExpectThat()
		.ExecutionIsInThreadMode()
		.NoInterruptIsActiveOrPending();
}
