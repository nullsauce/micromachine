/* Copyright (C) MindMaze SA - 2016 - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of MindMaze SA.

*/

#ifndef MICROMACHINE_EMU_EXCEPTION_DEFS_HPP
#define MICROMACHINE_EMU_EXCEPTION_DEFS_HPP

class Exception {
public:
	enum Type : uint32_t
	{
		INVALID = 0,
		RESET = 1,
		NMI = 2,
		HARDFAULT = 3,

		// 4-10 reserved
		_RESERVED_0 = 4,
		_RESERVED_1 = 5,
		_RESERVED_2 = 6,
		_RESERVED_3 = 7,
		_RESERVED_4 = 8,
		_RESERVED_5 = 9,
		_RESERVED_6 = 10,

		SVCALL = 11,

		// 12-13 reserved
		_RESERVED_7 = 12,
		_RESERVED_8 = 13,

		PENDSV = 14,
		SYSTICK = 15,

		// external interrupts
		EXTI_00 = 16,
		EXTI_01 = 17,
		EXTI_02 = 18,
		EXTI_03 = 19,
		EXTI_04 = 20,
		EXTI_05 = 21,
		EXTI_06 = 22,
		EXTI_07 = 23,
		EXTI_08 = 24,
		EXTI_09 = 25,
		EXTI_10 = 26,
		EXTI_11 = 27,
		EXTI_12 = 28,
		EXTI_13 = 29,
		EXTI_14 = 30,
		EXTI_15 = 31,
	};

	static Type from_number(uint8_t number) {
		return static_cast<Type>(number);
	}
};

#endif //MICROMACHINE_EMU_EXCEPTION_DEFS_HPP
