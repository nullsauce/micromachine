/* Copyright (C) MindMaze SA - 2016 - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of MindMaze SA.

*/

#ifndef MICROMACHINE_EMU_STATIC_ASSERT_EXCEPTION_HPP
#define MICROMACHINE_EMU_STATIC_ASSERT_EXCEPTION_HPP

#include <string>
#include <stdexcept>

namespace testing {

	struct exceptionalized_static_assert : std::logic_error{
		exceptionalized_static_assert(char const* what)
		: std::logic_error(what){};
		virtual ~exceptionalized_static_assert() noexcept {}
	};

	template<bool Cond>
	struct exceptionalize_static_assert;

	template<>
	struct exceptionalize_static_assert<true> {
		explicit exceptionalize_static_assert(const char*) {}
	};


	template<>
	struct exceptionalize_static_assert<false> {
		explicit exceptionalize_static_assert(char const * reason) {
			std::string s("static_assert would fail with reason: ");
			s += reason;
			throw exceptionalized_static_assert(s.c_str());
		}
	};

	template <typename FromType, typename ToType>
	using conversion_exception = testing::exceptionalize_static_assert<std::is_convertible<FromType, ToType>::value>;

	template <typename FromType, typename ToType>
	using not_conversion_exception = testing::exceptionalize_static_assert<!std::is_convertible<FromType,
	ToType>::value>;

	template <typename FromType, typename ToType>
	struct assert_convertible : conversion_exception<FromType, ToType> {
		assert_convertible(const char* message) : conversion_exception<FromType, ToType>(message) {}
	};

	template <typename FromType, typename ToType>
	struct assert_not_convertible : not_conversion_exception<FromType, ToType> {
		assert_not_convertible(const char* message) : not_conversion_exception<FromType, ToType>(message) {}
	};
}

#endif //MICROMACHINE_EMU_STATIC_ASSERT_EXCEPTION_HPP
