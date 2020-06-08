

#include "SystemTestFixture.hpp"

#include "CodeGenerator.hpp"
#include "TestSystem.hpp"
#include "mcu.hpp"
#include <gtest/gtest.h>

#include <algorithm>
#include <cstddef>
#include <utility>
#include <iomanip>


using namespace micromachine::testing;

/* ADC - Register (ADd with Carry)
   Encoding: 010000 0101 Rm:3 Rdn:3 */
TEST_F(SystemTestFixture, SomeTest) {

	getCpu().regs().set(registers::R1, -1);
	getCpu().regs().set(registers::R2, 1);

	code_gen().emitAdc(registers::R1, registers::R2);

	/*
	StepAndExpectThat(
		apsrFlagsEquals("nZCv"),
		registerValueEquals(registers::R2, -1 + 1),
		reg(registers::R2).equals(-1 + 1),
		exceptionHandlerReached(exception::EXTI_07)
	);*/

	StepAndExpectThat(apsrFlagsEquals("nZCv"),
					  registerValueEquals(registers::R2, -1 + 1),
					  reg(registers::R2).equals(-1 + 1),
					  instruction16Executed());
}
