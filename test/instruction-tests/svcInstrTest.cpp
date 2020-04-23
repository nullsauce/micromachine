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
#include "exception/exception_defs.hpp"

/* SVC
   Encoding: 1101 1111 Imm:8 */
MICROMACHINE_TEST_F(svc, SmallestImmediate, CpuTestFixture) {
	code_gen().emit_ins16("11011111iiiiiiii", 0);
	Step();
	ExpectThat().ExceptionIsActive(micromachine::system::exception::SVCALL);
}

MICROMACHINE_TEST_F(svc, LargestImmediate, CpuTestFixture) {
	code_gen().emit_ins16("11011111iiiiiiii", 255);
	Step();
	ExpectThat().ExceptionIsActive(micromachine::system::exception::SVCALL);
}
