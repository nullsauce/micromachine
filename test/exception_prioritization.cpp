//
// Created by fla on 17-5-8.
//

#include <gtest/gtest.h>
#include "exception_vector.hpp"

TEST(ExceptionsTest, NotPendingWhenEmpty) {
	exception_vector vec;
	EXPECT_FALSE(vec.any_pending());
}

TEST(ExceptionsTest, RasingOnePendingShouldBePending) {
	exception_vector vec;
	vec.raise(exception_number::name::HARDFAULT);
	EXPECT_TRUE(vec.any_pending());
}

TEST(ExceptionsTest, RasingOnePendingShouldBeTheSamePending) {
	exception_vector vec;
	vec.raise(exception_number::name::HARDFAULT);
	EXPECT_TRUE(vec.any_pending());
	ASSERT_NE(nullptr, vec.top_pending_exception());
	EXPECT_EQ(exception_number::name::HARDFAULT, vec.top_pending_exception()->number());
}

TEST(ExceptionsTest, PendingFlagScope) {
	exception_vector vec;
	vec.raise(exception_number::name::HARDFAULT);
	EXPECT_TRUE(vec.any_pending());
	vec.activate(exception_number::name::HARDFAULT);
	EXPECT_FALSE(vec.any_pending());
	vec.deactivate(exception_number::name::HARDFAULT);
	EXPECT_FALSE(vec.any_pending());
}

TEST(ExceptionsTest, TopPendingShouldBeHighestPriority) {
	exception_vector vec;

	// Raise a HARDFAULT exception
	vec.raise(exception_number::name::HARDFAULT);
	ASSERT_NE(nullptr, vec.top_pending_exception());
	EXPECT_EQ(exception_number::name::HARDFAULT, vec.top_pending_exception()->number());

	// Raise a RESET exception
	// Reset is higher priority and should take precedence as top pending
	vec.raise(exception_number::name::RESET);
	EXPECT_EQ(exception_number::name::RESET, vec.top_pending_exception()->number());

	// Simulate handling of the RESET exception
	vec.activate(exception_number::name::RESET);
	vec.deactivate(exception_number::name::RESET);

	// The HARDFAULT exception should still be pending
	ASSERT_NE(nullptr, vec.top_pending_exception());
	EXPECT_EQ(exception_number::name::HARDFAULT, vec.top_pending_exception()->number());

	// Simulate handling of the HARDFAULT exception
	vec.activate(exception_number::name::HARDFAULT);
	// There should be no more pending exceptions
	EXPECT_EQ(nullptr, vec.top_pending_exception());
	vec.deactivate(exception_number::name::HARDFAULT);

	// There should be no more pending exceptions
	EXPECT_EQ(nullptr, vec.top_pending_exception());



}
