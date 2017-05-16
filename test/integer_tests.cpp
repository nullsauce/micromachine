//
// Created by fla on 17-5-8.
//

#include "../lib/types.hpp"
#include <gtest/gtest.h>

#define ANY_MSG ".*"

TEST(IntegerTests, SignedIntPreconditions) {
	EXPECT_DEATH(byte(0).sint(0,0), ANY_MSG);
	EXPECT_DEATH(byte(0).sint(0,1), ANY_MSG);
	EXPECT_DEATH(byte(0).sint(8,4), ANY_MSG);
	EXPECT_DEATH(byte(0).sint(28,4), ANY_MSG);
	EXPECT_DEATH(byte(0).sint(0,10), ANY_MSG);
	EXPECT_DEATH(halfword(0).sint(0,0), ANY_MSG);
	EXPECT_DEATH(halfword(0).sint(0,1), ANY_MSG);
	EXPECT_DEATH(halfword(0).sint(16,4), ANY_MSG);
	EXPECT_DEATH(halfword(0).sint(28,4), ANY_MSG);
	EXPECT_DEATH(halfword(0).sint(0,20), ANY_MSG);
	EXPECT_DEATH(word(0).sint(0,0), ANY_MSG);
	EXPECT_DEATH(word(0).sint(0,1), ANY_MSG);
	EXPECT_DEATH(word(0).sint(32,4), ANY_MSG);
	EXPECT_DEATH(word(0).sint(32,1), ANY_MSG);
	EXPECT_DEATH(word(0).sint(28,6), ANY_MSG);
	EXPECT_DEATH(word(0).sint(0,34), ANY_MSG);
}

TEST(IntegerTests, SignedSmallAligned) {
	EXPECT_EQ(0, byte(0).sint(0, 8));
	EXPECT_EQ(-60, byte(0b1000100).sint(0, 7));
	EXPECT_EQ(-2, byte(0b1110).sint(0, 4));
	EXPECT_EQ(-128, byte(0b10000000).sint(0, 8));
	EXPECT_EQ(-1, byte(0b11).sint(0, 2));
	EXPECT_EQ(-13, byte(0b10011).sint(0, 5));
}

TEST(IntegerTests, SignedSmallUnAligned) {
	EXPECT_EQ(-7, byte(0b10010000).sint(4, 4));
	EXPECT_EQ(-34, byte(0b10111100).sint(1, 7));
}