//
// Created by fla on 17-5-8.
//

#include <gtest/gtest.h>
#include "exception_vector.hpp"

TEST(ExceptionsTest, NotPendingWhenEmpty) {
	exception_vector vec;
	EXPECT_FALSE(vec.any_active());
}

TEST(ExceptionsTest, RasingOnePendingShouldBePending) {
	exception_vector vec;
	vec.raise(exception_type::HARDFAULT);
	EXPECT_TRUE(vec.any_active());
}

TEST(ExceptionsTest, RasingOnePendingShouldBeTheSamePending) {
	exception_vector vec;
	vec.raise(exception_type::HARDFAULT);
	EXPECT_TRUE(vec.any_active());
	ASSERT_NE(nullptr, vec.top_pending_exception());
	EXPECT_EQ(exception_type::HARDFAULT, vec.top_pending_exception()->type());
}

TEST(ExceptionsTest, ActiveFlagScope) {
	exception_vector vec;
	vec.raise(exception_type::HARDFAULT);
	EXPECT_TRUE(vec.any_active());
	vec.activate(exception_type::HARDFAULT);
	EXPECT_TRUE(vec.any_active());
	vec.deactivate(exception_type::HARDFAULT);
	EXPECT_FALSE(vec.any_active());
}

TEST(ExceptionsTest, TopPendingShouldBeHighestPriority) {
	exception_vector vec;

	// Raise a HARDFAULT exception
	vec.raise(exception_type::HARDFAULT);
	ASSERT_NE(nullptr, vec.top_pending_exception());
	EXPECT_EQ(exception_type::HARDFAULT, vec.top_pending_exception()->type());

	// Raise a RESET exception
	// Reset is higher priority and should take precedence as top pending
	vec.raise(exception_type::RESET);
	EXPECT_EQ(exception_type::RESET, vec.top_pending_exception()->type());

	// Simulate handling of the RESET exception
	vec.activate(exception_type::RESET);
	vec.deactivate(exception_type::RESET);

	// The HARDFAULT exception should still be pending
	ASSERT_NE(nullptr, vec.top_pending_exception());
	EXPECT_EQ(exception_type::HARDFAULT, vec.top_pending_exception()->type());

	// Simulate handling of the HARDFAULT exception
	vec.activate(exception_type::HARDFAULT);
	// There should be no more pending exceptions
	EXPECT_EQ(nullptr, vec.top_pending_exception());
	vec.deactivate(exception_type::HARDFAULT);

	// There should be no more pending exceptions
	EXPECT_EQ(nullptr, vec.top_pending_exception());



}