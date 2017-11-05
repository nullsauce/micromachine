/* Copyright (C) The Micromachine project - 2016 - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of The Micromachine project.

*/

#ifndef MICROMACHINE_INSTRUCTION_PAIR_HPP
#define MICROMACHINE_INSTRUCTION_PAIR_HPP

#include "types.hpp"

struct instruction_pair {
	instruction_pair(halfword a, halfword b)
		: first(a)
		, second(b) {}
	const halfword first;
	const halfword second;
};
#endif //MICROMACHINE_INSTRUCTION_PAIR_HPP
