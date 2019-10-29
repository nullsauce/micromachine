/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#include "framework/static_assert_exception.hpp"
#include "bits.hpp"
#include <gtest/gtest.h>
#include <type_traits>

#define DUMP_VAR(var) fprintf(stderr, #var" = %08x = %s\n", var, binops::to_string(var).c_str())

TEST(BitsTest, SameSliceEqualityAfterAssignation)
{
	using a_slice = bits<4, 8>;
	const uint16_t initial_a = 0x1234;
	const uint16_t initial_b = 0x5678;
	uint16_t a = initial_a;
	uint16_t b = initial_b;
	a_slice::of(a) = a_slice::of(b);
	EXPECT_EQ((a_slice::of(b)), (a_slice::of(a)));
	EXPECT_EQ(initial_b, b);
}

TEST(BitsTest, SameSliceEqualityAfterAssignationDifferentSlices)
{
	const uint16_t initial_b = 0b101011001111000;
	uint16_t a = 0b001001000110100;
	uint16_t b = initial_b;
	bits<7, 7>::of(a) = bits<2, 7>::of(b);
	EXPECT_EQ((bits<7, 7>::of(a)), (bits<2, 7>::of(b)));
	EXPECT_EQ(initial_b, b);
	EXPECT_EQ(a, 0b000111100110100);
}

TEST(BitsTest, SameSliceAssignationCorectness)
{
	using mid_slice = bits<4, 8>;
	uint16_t a = 0x1234;
	uint16_t b = 0x5678;
	uint16_t e = (a & 0xf00f) | (b & 0x0ff0);
	mid_slice::of(a) = mid_slice::of(b);
	EXPECT_EQ(e, a);
}

TEST(BitsTest, RefSliceReadCorectnessMin)
{
	uint16_t a = 0x1234;
	EXPECT_EQ(0x34, (bits<0, 8>::of(a)));
}

TEST(BitsTest, RefSliceReadCorectnessMax)
{
	uint16_t a = 0x1234;
	EXPECT_EQ(0x12, (bits<8, 8>::of(a)));
}

TEST(BitsTest, RefSliceReadCorectnessMid)
{
	uint16_t a = 0x1234;
	EXPECT_EQ(0x23, (bits<4, 8>::of(a)));
}

TEST(BitsTest, ConstRefSliceReadCorectnessMin)
{
	EXPECT_EQ(0x34, (bits<0, 8>::of(0x1234)));
}

TEST(BitsTest, ConstRefSliceReadCorectnessMax)
{
	EXPECT_EQ(0x12, (bits<8, 8>::of(0x1234)));
}

TEST(BitsTest, ConstRefSliceReadCorectnessMid)
{
	EXPECT_EQ(0x23, (bits<4, 8>::of(0x1234)));
}

TEST(BitsTest, SingleBitSliceAssignationCorectness)
{
	using thebit = bits<2>;
	const uint16_t initial_value = 0b0001010100011101;
	uint16_t a = initial_value;
	EXPECT_EQ(1, thebit::of(a));
	thebit::of(a) = false;
	EXPECT_EQ(0, thebit::of(a));
	thebit::of(a) = true;
	EXPECT_EQ(initial_value, a);
}

TEST(BitsTest, SliceWindowCorectness)
{
	const uint16_t v = 0b0001010100011101;
	EXPECT_EQ(1, (bits<0, 1>::of(v)));
	EXPECT_EQ(2, (bits<1, 2>::of(v)));
	EXPECT_EQ(3, (bits<3, 3>::of(v)));
	EXPECT_EQ(4, (bits<6, 4>::of(v)));
	EXPECT_EQ(5, (bits<10, 5>::of(v)));
	EXPECT_EQ(0, (bits<15, 1>::of(v)));
}

TEST(BitsTest, ClearSomeBits)
{
	uint16_t v = 0b0001010100011101;
	bits<10, 6>::of(v).clear();
	EXPECT_EQ(0b0000000100011101, v);
}

TEST(BitsTest, ClearAllBits)
{
	uint16_t v = 0b0001010100011101;
	bits<0, 16>::of(v).clear();
	EXPECT_EQ(0, v);
}

TEST(BitsTest, ClearOneBit)
{
	uint16_t v = 0b0001010100011101;
	bits<2>::of(v).clear();
	EXPECT_EQ(0b0001010100011001, v);
}

TEST(BitsTest, SingleBitWordConvertibleToBool)
{
	testing::assert_convertible<slice<0, 1, uint16_t>, bool>
		("bitslice of length 1 should be convertible to bool");
}

TEST(BitsTest, SingleBitSliceAssignableFromBool)
{
	testing::assert_assignable<slice<0, 1, uint16_t>, bool>
		("bitslice of length 1 should be assignable from bool");
}

TEST(BitsTest, SingleBitSliceNotAssignableFromLargerInt)
{
	testing::assert_not_assignable<slice<0, 1, uint16_t>, uint8_t>
		("bitslice of length 1 should not be assignable from uint8_t");
}

TEST(BitsTest, SmallBitSliceNotAssignableFromLargerInt)
{
	testing::assert_not_assignable<slice<0, 6, uint16_t>, uint8_t>
		("bitslice of length 6 should not be assignable from uint8_t");
}

TEST(BitsTest, SubSliceReadConsistency)
{
	uint16_t a = 0x1234;
	EXPECT_EQ((bits<6, 2>::of(a)), (bits<2, 2>::of(bits<4, 8>::of(a))));
}

TEST(BitsTest, SubSliceWriteConsistency)
{
	using a_slice = bits<4, 8>;
	uint16_t a = 0b1111001011110000;
	bits<4, 8>::of(bits<4, 12>::of(a)) = (uint8_t) 0;
	EXPECT_EQ(0b0000000011110000, a);
}

TEST(BitsTest, SelfliceWriteConsistency)
{
	uint16_t a = 0b0000110100001111;
	bits<4, 4>::of(a) = bits<8, 4>::of(a);
	EXPECT_EQ(0b0000110111011111, a);
}

TEST(BitsTest, ByteSwapWithTemporary)
{
	uint16_t a = 0xdeaf;
	auto tmp = (uint8_t) bits<0, 8>::of(a);
	bits<0, 8>::of(a) = bits<8, 8>::of(a);
	EXPECT_EQ(0xde, (bits<0, 8>::of(a)));
	bits<8, 8>::of(a) = tmp;
	EXPECT_EQ(0xafde, a);
}
