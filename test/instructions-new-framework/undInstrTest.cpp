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


MICROMACHINE_TEST_F(undefined, Undedfined16BitWithAllZeroesForImmedaite, CpuTestFixture) {
	code_gen().emit_ins16("11011110iiiiiiii", 0);
	Step();
	ExpectThat().HardfaultHandlerReached();
}

MICROMACHINE_TEST_F(undefined, Undedfined16BitWithAllOnesForImmedaite, CpuTestFixture) {
	code_gen().emit_ins16("11011110iiiiiiii", -1);
	Step();
	ExpectThat().HardfaultHandlerReached();
}

MICROMACHINE_TEST_F(undefined, Undefined32BitWithAllZeroesForImmediate, CpuTestFixture) {
	code_gen().emit_ins32("111101111111iiii", "1010iiiiiiiiiiii", 0, 0);
	Step();
	ExpectThat().HardfaultHandlerReached();
}

MICROMACHINE_TEST_F(undefined, Undefined32BitWithAllOnesForImmediate, CpuTestFixture) {
	code_gen().emit_ins32("111101111111iiii", "1010iiiiiiiiiiii", -1, -1);
	Step();
	ExpectThat().HardfaultHandlerReached();
}
