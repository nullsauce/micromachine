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

#include "CpuTestFixture.hpp"
using namespace micromachine::testing;
#include "exception/exception.hpp"

/* SVC
   Encoding: 1101 1111 Imm:8 */
MICROMACHINE_TEST_F(svc, SmallestImmediate, CpuTestFixture) {
	emitInstruction16("11011111iiiiiiii", 0);

	// Account for the fact that SVCall saved context contains a different return address
	auto context = captureCurrentContext();
	context.returnAddress = getReg(registers::PC) + 2;
	StepAndExpectThat(exceptionHandlerReachedWithContext(exception::SVCALL, context));
}

MICROMACHINE_TEST_F(svc, LargestImmediate, CpuTestFixture) {
	emitInstruction16("11011111iiiiiiii", 255);

	// Account for the fact that SVCall saved context contains a different return address
	auto context = captureCurrentContext();
	context.returnAddress = getReg(registers::PC) + 2;
	StepAndExpectThat(exceptionHandlerReachedWithContext(exception::SVCALL, context));
}
