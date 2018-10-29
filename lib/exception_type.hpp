//
// Created by fla on 17-5-20.
//

#ifndef MICROMACHINE_EXCEPTION_HPP
#define MICROMACHINE_EXCEPTION_HPP

#include <cstddef>
#include <vector>
#include <functional>
#include <queue>
#include <algorithm>
#include <memory>


struct exception_number {
private:
	using exception_number_t = uint8_t;

public:
	exception_number_t _integer_value;
	enum class ex_name : exception_number_t {
		INVALID = 0,
		RESET	= 1,
		NMI		= 2,
		HARDFAULT = 3,

		// 4-10 reserved
		_RESERVED_0 = 4,
		_RESERVED_1 = 5,
		_RESERVED_2 = 6,
		_RESERVED_3 = 7,
		_RESERVED_4 = 8,
		_RESERVED_5 = 9,
		_RESERVED_6 = 10,

		SVCALL	= 11,

		// 12-13 reserved
		_RESERVED_7 = 12,
		_RESERVED_8 = 13,

		PENDSV = 14,
		SYSTICK = 15,

		IRQ_0 = 16,
		IRQ_1 = 17,
		IRQ_2 = 18,
		IRQ_3 = 19,
		IRQ_4 = 20,
		IRQ_5 = 21,
		IRQ_6 = 22,
		IRQ_7 = 23,
		IRQ_8 = 24,
		IRQ_9 = 25,
		IRQ_10 = 26,
		IRQ_11 = 27,
		IRQ_12 = 28,
		IRQ_13 = 29,
		IRQ_14 = 30,
		IRQ_15 = 31,
		IRQ_16 = 32,
		IRQ_17 = 33,
		IRQ_18 = 34,
		IRQ_19 = 35,
		IRQ_20 = 36,
		IRQ_21 = 37,
		IRQ_22 = 38,
		IRQ_23 = 39,
		IRQ_24 = 40,
		IRQ_25 = 41,
		IRQ_26 = 42,
		IRQ_27 = 43,
		IRQ_28 = 44,
		IRQ_29 = 45,
		IRQ_30 = 46,
		IRQ_31 = 47,
	};
	//exception_number() = default;
	exception_number(exception_number_t number) : _integer_value(number) {}
	exception_number(ex_name name) : _integer_value(from_name(name)) {}
	ex_name name() const {
		return static_cast<ex_name>(*this);
	}

	exception_number_t int_value() const {
		return static_cast<exception_number_t>(*this);
	}

	static exception_number from_uint(uint32_t number) {
		return { static_cast<exception_number_t>(number) };
	}
	static exception_number from_name(ex_name name) {
		return { static_cast<exception_number_t>(name) };
	}
	operator exception_number_t() const {
		return _integer_value;
	}
	operator ex_name() const {
		return static_cast<ex_name>(_integer_value);
	}
	bool operator==(const exception_number& other) const {
		return _integer_value == other._integer_value;
	}
	bool operator<(const exception_number& other) const {
		return _integer_value < other._integer_value;
	}
	bool operator>(const exception_number& other) const {
		return _integer_value > other._integer_value;
	}
	bool operator<=(const exception_number& other) const {
		return _integer_value <= other._integer_value;
	}
	bool operator>=(const exception_number& other) const {
		return _integer_value >= other._integer_value;
	}

	std::string str() const {
		static const char* names[32] = {
			"INVALID",
			"RESET",
			"NMI",
			"HARDFAULT",
			"_RESERVED_0",
			"_RESERVED_1",
			"_RESERVED_2",
			"_RESERVED_3",
			"_RESERVED_4",
			"_RESERVED_5",
			"_RESERVED_6",
			"SVCALL",
			"_RESERVED_7",
			"_RESERVED_8",
			"PENDSV",
			"SYSTICK",
			"IRQ_0",
			"IRQ_1",
			"IRQ_2",
			"IRQ_3",
			"IRQ_4",
			"IRQ_5",
			"IRQ_6",
			"IRQ_7",
			"IRQ_8",
			"IRQ_9",
			"IRQ_10",
			"IRQ_11",
			"IRQ_12",
			"IRQ_13",
			"IRQ_14",
			"IRQ_15",
		};
		return names[int_value()];
	}
};



#endif //MICROMACHINE_EXCEPTION_HPP
