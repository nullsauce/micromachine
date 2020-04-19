/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/


#include "CpuTestFixture.hpp"
#include "RegisterMutationPredicate.hpp"

TEST_F(CpuTestFixture, CpuTestFixture_NothingChangesOnIdenticalState) {

	ExpectThat()
		.PcDidNotChange()
		.RegistersDidNotChange()
		.XPSRRegisterDidNotChange()
		.NoInterruptIsPending();

}


TEST_F(CpuTestFixture, CpuTestFixture_ChangesAfterNopExecution) {

	Step();

	ExpectThat()
		.InstructionExecutedWithoutBranch()
		.NoInterruptIsPending();
}


TEST_F(CpuTestFixture, CpuTestFixture_SetRegisterValues) {

	for (reg_idx r = 0; r < registers::NUM_GP_REGS; r++) {
		getCpu().regs().set(r, 0x11111111U * (r + 1));
	}

	for (reg_idx r = 0; r < registers::NUM_GP_REGS; r++) {
		ExpectThat().Register(r).Equals(0x11111111U * (r + 1));
	}

	for (reg_idx r = 0; r < registers::NUM_GP_REGS; r++) {
		ExpectThat().Register(r).Changed();
	}

}
