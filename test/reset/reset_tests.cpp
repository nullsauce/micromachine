/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#include "CpuTestFixture.hpp"

class CpuResetTest : public CpuTestFixture {
protected:
	virtual void SetUp() override {
		CpuTestFixture::SetUp();
		for (reg_idx regIdx = 0; regIdx < registers::NUM_GP_REGS; regIdx++) {
			getCpu().regs().set(regIdx, 0x11111111U * (1U + regIdx));
		}
	}
};

MICROMACHINE_TEST_F(ResetBehavior, GeneralPurposeRegistersAreSetTozero, CpuResetTest) {
	for (reg_idx regIdx = 0; regIdx < registers::NUM_GP_REGS; regIdx++) {
		ExpectThat().Register(regIdx).Equals(0x11111111U * (1U + regIdx));
	}

	getCpu().reset(0);

	for (reg_idx regIdx = 0; regIdx < registers::NUM_GP_REGS; regIdx++) {
		ExpectThat().Register(regIdx).Equals(0);
	}
}

MICROMACHINE_TEST_F(ResetBehavior, PcIsSetToGivenPosition, CpuResetTest) {
	getCpu().regs().set_pc(0x10);
	getCpu().reset(0x20);
	ExpectThat().Register(registers::PC).Equals(0x20);
}


MICROMACHINE_TEST_F(ResetBehavior, LrIsReset, CpuResetTest) {
	getCpu().regs().set_lr(0xdeed00aa);
	getCpu().reset(0x20);
	ExpectThat().Register(registers::LR).Equals(0);
}


MICROMACHINE_TEST_F(ResetBehavior, PendingInternalExceptionIsCleared, CpuResetTest) {
	getCpu().interrupt().raise_hardfault();
	getCpu().reset(0x20);
	ExpectThat().NoInterruptIsActiveOrPending();
}

MICROMACHINE_TEST_F(ResetBehavior, ActiveInternalExceptionIsCleared, CpuResetTest) {
	getCpu().interrupt().raise_hardfault();
	Step();
	ExpectThat().HardfaultHandlerReached();
	getCpu().reset(0x20);
	ExpectThat().NoInterruptIsActiveOrPending();
}

MICROMACHINE_TEST_F(ResetBehavior, PendingExternalInterruptIsCleared, CpuResetTest) {
	getCpu().interrupt().enable_external_interrupt(7);
	getCpu().interrupt().raise_external_interrupt(7);
	Step();
	ExpectThat().ExceptionHandlerReached(exception::EXTI_07);
	getCpu().reset(0x20);
	ExpectThat().NoInterruptIsActiveOrPending();
}

MICROMACHINE_TEST_F(ResetBehavior, ActiveExternalInterruptIsCleared, CpuResetTest) {
	getCpu().interrupt().enable_external_interrupt(7);
	getCpu().interrupt().raise_external_interrupt(7);
	Step();
	ExpectThat().ExceptionHandlerReached(exception::EXTI_07);
	getCpu().reset(0x20);
	ExpectThat().NoInterruptIsActiveOrPending();
}

MICROMACHINE_TEST_F(ResetBehavior, InterruptPrioritiesAreReset, CpuResetTest) {
	getCpu().interrupt().set_exception_priority<exception::EXTI_07>(3);
	getCpu().interrupt().set_exception_priority<exception::SVCALL>(1);
	getCpu().reset(0x20);
	EXPECT_EQ(0U, getCpu().interrupt().exception_priority<exception::EXTI_07>());
	EXPECT_EQ(0U, getCpu().interrupt().exception_priority<exception::SVCALL>());
	EXPECT_EQ(exception::RESET_PRIORITY, getCpu().interrupt().exception_priority<exception::RESET>());
	EXPECT_EQ(exception::NMI_PRIORITY, getCpu().interrupt().exception_priority<exception::NMI>());
	EXPECT_EQ(exception::HARDFAULT_PRIORITY, getCpu().interrupt().exception_priority<exception::HARDFAULT>());
	ExpectThat().NoInterruptIsActiveOrPending();
}

MICROMACHINE_TEST_F(ResetBehavior, XpsrIsCleared, CpuResetTest) {
	getCpu().regs().xpsr_register() = 0xffffffff;
	getCpu().reset(0x20);
	ExpectThat()
		.APSRFlagsMatches("nzcv")
		.ThumbBitIsSet()
		.NoInterruptIsActiveOrPending();
}

MICROMACHINE_TEST_F(ResetBehavior, ExecModeIsSetToThreadMode, CpuResetTest) {
	getCpu().regs().exec_mode_register().set_handler_mode();
	getCpu().reset(0x20);
	ExpectThat()
		.ExecutionIsInThreadMode()
		.NoInterruptIsActiveOrPending();
}
