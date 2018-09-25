
#include "types.hpp"
#include <gtest/gtest.h>

TEST(BitsTestHalfWord, SameSliceHalf) {
	halfword a = 0b0000001011111111;
	halfword b = 0b1111100000001000;
	a.bits<0,8>() = b.bits<0,8>();
	EXPECT_EQ(0b0000001000001000, a);
}

TEST(BitsTestHalfWord, SameSliceOne) {
	halfword a = 0b0000000011111111;
	halfword b = 0b1111100000000000;
	a.bits<0,1>() = b.bits<0,1>();
	EXPECT_EQ(0b0000000011111110, a);
}

TEST(BitsTestHalfWord, OffsetOfTwo) {
	halfword a = 0b1111111111111111;
	halfword b = 0b0000000101010100;
	a.bits<2,7>() = b.bits<0,7>();
	EXPECT_EQ(0b1111111101010011, a);
}

TEST(BitsTestHalfWord, LastBitOnly) {
	halfword a = 0b0000000000000000;
	halfword b = 0b1100000000000000;
	a.bits<15,1>() = b.bits<15,1>();
	EXPECT_EQ(0b1000000000000000, a);
}

TEST(BitsTestHalfWord, DoubleReverted) {
	halfword a = 0b1111111111111111;
	halfword b = 0b0000000100001000;
	a.bits<4,4>() = b.bits<4,4>();
	EXPECT_EQ(0b1111111100001111, a);
	b.bits<4,4>() = a.bits<4,4>();
	EXPECT_EQ(0b0000000100001000, b);
}

TEST(BitsTestHalfWord, DoubleWithOffsets) {
	halfword a = 0b1111111111111111;
	halfword b = 0b0000000100001000;
	a.bits<2,4>() = b.bits<4,4>();
	EXPECT_EQ(0b1111111111000011, a);
	b.bits<4,4>() = a.bits<2,4>();
	EXPECT_EQ(0b0000000100001000, b);
}

TEST(BitsTestHalfWord, ValueExtractionSimple) {
	halfword a = 0b0000000000000010;
	halfword extracted = a.bits<1,2>();
	EXPECT_EQ(0b1, extracted);
}

TEST(BitsTestHalfWord, ValueExtractionMedium) {
	halfword a = 0b0000000010000010;
	halfword extracted = a.bits<2,7>();
	EXPECT_EQ(0b0100000, extracted);
}

TEST(BitsTestHalfWord, ExtractSubSlices) {
	halfword a = 0b1101011100010010;
	halfword extracted = a.bits<0,16>().extract();
	EXPECT_EQ(0b1101011100010010, extracted);

	halfword extracted2 = extracted.bits<2,10>().extract();
	EXPECT_EQ(0b0111000100, extracted2);

	halfword extracted3 = extracted2.bits<3,4>().extract();
	EXPECT_EQ(0b1000, extracted3);

	EXPECT_EQ(4, (extracted3.bits<1,3>().extract()));
}

TEST(BitsTestHalfWord, AssignFromInteger) {
	halfword a = 0b1111111111111111;
	auto slice = a.bits<3, 12>();
	slice = 0;
	EXPECT_EQ(0b1000000000000111, a);
	slice.bits<1, 2>() = 0b10;
	EXPECT_EQ(0b1000000000000101, a);
}

TEST(BitsTestHalfWord, AssignFromTooShortInteger) {
	halfword a = 0b0000000000000000;
	auto slice = a.bits<1, 15>();
	slice = (uint8_t)0b11111111;
	EXPECT_EQ(0b0000000111111110, a);
}

TEST(BitsTestHalfWord, AssignFromTooLargeInteger) {
	halfword a = 0b0000000000000000;
	auto slice = a.bits<1, 7>();
	slice = (uint16_t)0b1111111111111111;
	EXPECT_EQ(0b0000000011111110, a);
}

template<typename TSlice>
static void clear_slice(TSlice& slice) {
	slice = 0;
}

TEST(BitsTestHalfWord, PassRefToFunction) {
	halfword a = 0b1111111111111111;
	auto slice = a.bits<3, 12>();
	clear_slice(slice);
	EXPECT_EQ(0b1000000000000111, a);
}