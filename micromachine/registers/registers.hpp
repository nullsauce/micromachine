/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_REGISTERS_HPP
#define MICROMACHINE_REGISTERS_HPP

#include "types.hpp"

namespace micromachine::system::registers {
	static constexpr reg_idx R0 = 0;
	static constexpr reg_idx R1 = 1;
	static constexpr reg_idx R2 = 2;
	static constexpr reg_idx R3 = 3;
	static constexpr reg_idx R4 = 4;
	static constexpr reg_idx R5 = 5;
	static constexpr reg_idx R6 = 6;
	static constexpr reg_idx R7 = 7;
	static constexpr reg_idx R8 = 8;
	static constexpr reg_idx R9 = 9;
	static constexpr reg_idx R10 = 10;
	static constexpr reg_idx R11 = 11;
	static constexpr reg_idx R12 = 12;
	static constexpr reg_idx SP = 13;
	static constexpr reg_idx LR = 14;
	static constexpr reg_idx PC = 15;
}

#endif // MICROMACHINE_REGISTERS_HPP
