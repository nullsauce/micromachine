//
// Created by fla
//

#include <gtest/gtest.h>
#include "nvic.hpp"

TEST(NVICRegisters, ISER_ReadIsCoherent) {
	nvic nv;
	const uint32_t initial_enable_status = 0b1111011100110001;
	nvic::tester(&nv).enable_status() = initial_enable_status;
	EXPECT_EQ(nv._iser_reg, initial_enable_status);
}

TEST(NVICRegisters, ISER_WriteZeroHasNoEffect) {
	nvic nv;
	const uint32_t initial_enable_status = 0b1111011100110001;
	nvic::tester(&nv).enable_status() = initial_enable_status;
	nv._iser_reg = 0;
	EXPECT_EQ(nv._iser_reg, initial_enable_status);
}

TEST(NVICRegisters, ISER_WriteOneSetsEnabledStatus) {
	nvic nv;
	nvic::tester(&nv).enable_status() = 0b1111011100110001;
	nv._iser_reg = 0b0000000000000010;
	EXPECT_EQ(nv._iser_reg, 0b1111011100110011);
}

TEST(NVICRegisters, ICER_ReadIsCoherent) {
	nvic nv;
	const uint32_t initial_enable_status = 0b1111011100110001;
	nvic::tester(&nv).enable_status() = initial_enable_status;
	EXPECT_EQ(nv._icer_reg, initial_enable_status);
}

TEST(NVICRegisters, ICER_WriteZeroHasNoEffect) {
	nvic nv;
	const uint32_t initial_enable_status = 0b1111011100110001;
	nvic::tester(&nv).enable_status() = initial_enable_status;
	nv._icer_reg = 0;
	EXPECT_EQ(nv._icer_reg, initial_enable_status);
}

TEST(NVICRegisters, ICER_WriteOneClearsEnabledStatus) {
	nvic nv;
	nvic::tester(&nv).enable_status() = 0b1111011100110001;
	nv._icer_reg = 0b0000000000000001;
	EXPECT_EQ(nv._icer_reg, 0b1111011100110000);
}

TEST(NVICRegisters, ICER_ISER_ReadsAreCoherent) {
	nvic nv;
	const uint32_t initial_enable_status = 0b1111011100110001;
	nvic::tester(&nv).enable_status() = initial_enable_status;
	// disable interrupt 0
	nv._icer_reg = 0b0000000000000001;
	EXPECT_EQ(nv._icer_reg, 0b1111011100110000);
	EXPECT_EQ(nv._iser_reg, nv._icer_reg);
}


TEST(NVICRegisters, ISPR_ReadIsCoherent) {
	nvic nv;
	const uint32_t initial_pending_status = 0b1111011100110001;
	nvic::tester(&nv).pending_status() = initial_pending_status;
	EXPECT_EQ(nv._ispr_reg, initial_pending_status);
}

TEST(NVICRegisters, ISPR_WriteZeroHasNoEffect) {
	nvic nv;
	const uint32_t initial_pending_status = 0b1111011100110001;
	nvic::tester(&nv).pending_status() = initial_pending_status;
	nv._ispr_reg = 0;
	EXPECT_EQ(nv._ispr_reg, initial_pending_status);
}

TEST(NVICRegisters, ISPR_WriteOneSetsEnabledStatus) {
	nvic nv;
	nvic::tester(&nv).pending_status() = 0b1111011100110001;
	nv._ispr_reg = 0b0000000000000010;
	EXPECT_EQ(nv._ispr_reg, 0b1111011100110011);
}

TEST(NVICRegisters, ICPR_ReadIsCoherent) {
	nvic nv;
	const uint32_t initial_pending_status = 0b1111011100110001;
	nvic::tester(&nv).pending_status() = initial_pending_status;
	EXPECT_EQ(nv._icpr_reg, initial_pending_status);
}

TEST(NVICRegisters, ICPR_WriteZeroHasNoEffect) {
	nvic nv;
	const uint32_t initial_pending_status = 0b1111011100110001;
	nvic::tester(&nv).pending_status() = initial_pending_status;
	nv._icpr_reg = 0;
	EXPECT_EQ(nv._icpr_reg, initial_pending_status);
}

TEST(NVICRegisters, ICPR_WriteOneClearsEnabledStatus) {
	nvic nv;
	nvic::tester(&nv).pending_status() = 0b1111011100110001;
	nv._icpr_reg = 0b0000000000000001;
	EXPECT_EQ(nv._icpr_reg, 0b1111011100110000);
}

TEST(NVICRegisters, ICPR_ISPR_ReadsAreCoherent) {
	nvic nv;
	const uint32_t initial_pending_status = 0b1111011100110001;
	nvic::tester(&nv).pending_status() = initial_pending_status;
	// disable interrupt 0
	nv._icpr_reg = 0b0000000000000001;
	EXPECT_EQ(nv._icpr_reg, 0b1111011100110000);
	EXPECT_EQ(nv._ispr_reg, nv._icpr_reg);
}