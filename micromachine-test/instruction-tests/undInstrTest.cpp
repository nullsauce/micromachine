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

MICROMACHINE_TEST_F(undefined, Undedfined16BitWithAllZeroesForImmedaite, CpuTestFixture) {
	emitInstruction16("11011110iiiiiiii", 0);
	StepAndExpectThat(hardfaultHandlerReached());
}

MICROMACHINE_TEST_F(undefined, Undedfined16BitWithAllOnesForImmedaite, CpuTestFixture) {
	emitInstruction16("11011110iiiiiiii", -1);
	StepAndExpectThat(hardfaultHandlerReached());
}

MICROMACHINE_TEST_F(undefined, Undefined32BitWithAllZeroesForImmediate, CpuTestFixture) {
	emitInstruction32("111101111111iiii", "1010iiiiiiiiiiii", 0, 0);
	StepAndExpectThat(hardfaultHandlerReached());
}

MICROMACHINE_TEST_F(undefined, Undefined32BitWithAllOnesForImmediate, CpuTestFixture) {
	emitInstruction32("111101111111iiii", "1010iiiiiiiiiiii", -1, -1);
	StepAndExpectThat(hardfaultHandlerReached());
}
