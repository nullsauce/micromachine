/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/


#include "types.hpp"
#include <gtest/gtest.h>

TEST(BitsTest32bitsWholeRange, BitSliceTests) {
	uint32_t a = 0xFFFFFFFF;
	EXPECT_EQ((bits<0,32>::of(a)), 0xFFFFFFFF);
}

TEST(BitsTest32bitsReadCorrectness, BitSliceTests) {
	uint32_t a = 0xe6517df4;
	EXPECT_EQ((bits<0,32>::of(a)), 0b11100110010100010111110111110100);
	EXPECT_EQ((bits<0,1>::of(a)), 0b0);
	EXPECT_EQ((bits<31,1>::of(a)), 0b1);
	EXPECT_EQ((bits<3,10>::of(a)), 0b1110111110);
	EXPECT_EQ((bits<8,8>::of(a)), 0b01111101);
}


TEST(BitsTest32bitsWriteCorrectness, BitSliceTests) {
	uint32_t a = 0x309d2ae0;
	const uint32_t b = 0xe6517df4;
	bits<0,32>::of(a) = b;
	EXPECT_EQ(a, b);
}

TEST(BitsTest32bitsWriteSelfReadAligned, BitSliceTests) {
	uint32_t a = 0xe6517df4;
	bits<0,16>::of(a) = bits<16,16>::of(a);
	EXPECT_EQ(a, a << 16 | a);
	EXPECT_EQ(a, 0xe651e651);
}

TEST(BitsTest32bitsWriteSelfReadUnaligned, BitSliceTests) {
	uint32_t a = 0b11100110010100010111110111110100;
	bits<22,10>::of(a) = bits<3,10>::of(a);
	EXPECT_EQ(a, 0b11101111100100010111110111110100);
}

TEST(BitsTest32bitsWriteSelfReadUnalignedOverlapping, BitSliceTests) {
	uint32_t a = 0b11100110010100010111110111110100;
	bits<12,20>::of(a) = bits<0,20>::of(a);
	EXPECT_EQ(a, 0b00010111110111110100110111110100);
}

TEST(BitsTest32bitsWriteMoreShouldBeIgnored, BitSliceTests) {
	uint32_t a = 0;
	bits<0,1>::of(a) = bits<0,32>::of(0xFFFFFFFF);
	EXPECT_EQ(a, 1);
}

TEST(BitsTest32bitsWriteLessShouldZeroExtendAligned, BitSliceTests) {
	uint32_t a = 0x309d2ae0;
	bits<0,32>::of(a) = bits<0,8>::of(a);
	EXPECT_EQ(a, 0x000000e0);
}

TEST(BitsTest32bitsWriteLessShouldZeroExtendUnaligned, BitSliceTests) {
	uint32_t a = 0x309d2ae0;
	bits<0,32>::of(a) = bits<0,10>::of(a);
	EXPECT_EQ(a, 0b1011100000);
}
