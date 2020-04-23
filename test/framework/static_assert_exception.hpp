/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#pragma once

#include <string>
#include <stdexcept>

namespace testing {

	struct exceptionalized_static_assert : std::logic_error
	{
		exceptionalized_static_assert(char const *what)
			: std::logic_error(what)
		{};

		virtual ~exceptionalized_static_assert() noexcept
		{}
	};

	template<bool Cond>
	struct exceptionalize_static_assert;

	template<>
	struct exceptionalize_static_assert<true>
	{
		explicit exceptionalize_static_assert(const char *)
		{}
	};


	template<>
	struct exceptionalize_static_assert<false>
	{
		explicit exceptionalize_static_assert(char const *reason)
		{
			std::string s("static_assert would fail with reason: ");
			s += reason;
			throw exceptionalized_static_assert(s.c_str());
		}
	};

	/*
	template <typename FromType, typename ToType>
	struct conversion_assertion {
		using check_type = std::is_convertible<FromType, ToType>;

		using conversion_exception = testing::exceptionalize_static_assert<check_type::value>;

		using not_conversion_exception = testing::exceptionalize_static_assert<!check_type::value>;
	};*/

	template<bool Exp>
	void static_assert_exception(const char *message)
	{
		testing::exceptionalize_static_assert<Exp> ex(message);
	}

	template<typename FromType, typename ToType>
	void assert_convertible(const char *message)
	{
		static_assert_exception<std::is_convertible<FromType, ToType>::value>(message);
	}

	template<typename FromType, typename ToType>
	void assert_not_convertible(const char *message)
	{
		static_assert_exception<!std::is_convertible<FromType, ToType>::value>(message);
	}

	template<typename FromType, typename ToType>
	void assert_assignable(const char *message)
	{
		static_assert_exception<std::is_assignable<FromType, ToType>::value>(message);
	}

	template<typename FromType, typename ToType>
	void assert_not_assignable(const char *message)
	{
		static_assert_exception<!std::is_assignable<FromType, ToType>::value>(message);
	}

	/*

	template <typename FromType, typename ToType>
	struct assert_convertible : conversion_assertion<FromType, ToType>::conversion_exception {
		assert_convertible(const char* message) : conversion_assertion<FromType, ToType>::conversion_exception(message) {}
	};

	template <typename FromType, typename ToType>
	struct assert_not_convertible : conversion_exception<FromType, ToType>::not_conversion_exception {
		assert_not_convertible(const char* message) : conversion_exception<FromType,
		ToType>::not_conversion_exception(message) {}
	};*/
}

