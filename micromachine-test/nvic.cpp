
#include "nvic.hpp"
#include <gtest/gtest.h>

TEST(NVICRegisters, ISER_ReadIsCoherent) {
	using namespace micromachine::system;
	const uint32_t initial_enable_statuses = 0b1111011100110001;
	nvic nv = nvic::with_enable_statuses(initial_enable_statuses);
	EXPECT_EQ(nv.iser_reg(), initial_enable_statuses);
}

TEST(NVICRegisters, ISER_WriteZeroHasNoEffect) {
	using namespace micromachine::system;
	const uint32_t initial_enable_statuses = 0b1111011100110001;
	nvic nv = nvic::with_enable_statuses(initial_enable_statuses);
	nv.iser_reg() = 0;
	EXPECT_EQ(nv.iser_reg(), initial_enable_statuses);
}

TEST(NVICRegisters, ISER_WriteOneSetsEnabledStatus) {
	using namespace micromachine::system;
	nvic nv = nvic::with_enable_statuses(0b1111011100110001);
	nv.iser_reg() = 0b0000000000000010;
	EXPECT_EQ(nv.iser_reg(), 0b1111011100110011);
}

TEST(NVICRegisters, ICER_ReadIsCoherent) {
	using namespace micromachine::system;
	const uint32_t initial_enable_statuses = 0b1111011100110001;
	nvic nv = nvic::with_enable_statuses(initial_enable_statuses);
	EXPECT_EQ(nv.icer_reg(), initial_enable_statuses);
}

TEST(NVICRegisters, ICER_WriteZeroHasNoEffect) {
	using namespace micromachine::system;
	const uint32_t initial_enable_statuses = 0b1111011100110001;
	nvic nv = nvic::with_enable_statuses(initial_enable_statuses);
	nv.icer_reg() = 0;
	EXPECT_EQ(nv.icer_reg(), initial_enable_statuses);
}

TEST(NVICRegisters, ICER_WriteOneClearsEnabledStatus) {
	using namespace micromachine::system;
	nvic nv = nvic::with_enable_statuses(0b1111011100110001);
	nv.icer_reg() = 0b0000000000000001;
	EXPECT_EQ(nv.icer_reg(), 0b1111011100110000);
}

TEST(NVICRegisters, ICER_ISER_ReadsAreCoherent) {
	using namespace micromachine::system;
	const uint32_t initial_enable_statuses = 0b1111011100110001;
	nvic nv = nvic::with_enable_statuses(initial_enable_statuses);
	// disable interrupt 0
	nv.icer_reg() = 0b0000000000000001;
	EXPECT_EQ(nv.icer_reg(), 0b1111011100110000);
	EXPECT_EQ(nv.iser_reg(), nv.icer_reg());
}

TEST(NVICRegisters, ISPR_ReadIsCoherent) {
	using namespace micromachine::system;
	const uint32_t initial_pending_statuses = 0b1111011100110001;
	nvic nv = nvic::with_pending_statuses(initial_pending_statuses);
	EXPECT_EQ(nv.ispr_reg(), initial_pending_statuses);
}

TEST(NVICRegisters, ISPR_WriteZeroHasNoEffect) {
	using namespace micromachine::system;
	const uint32_t initial_pending_statuses = 0b1111011100110001;
	nvic nv = nvic::with_pending_statuses(initial_pending_statuses);
	nv.ispr_reg() = 0;
	EXPECT_EQ(nv.ispr_reg(), initial_pending_statuses);
}

TEST(NVICRegisters, ISPR_WriteOneSetsEnabledStatus) {
	using namespace micromachine::system;
	nvic nv = nvic::with_pending_statuses(0b1111011100110001);
	nv.ispr_reg() = 0b0000000000000010;
	EXPECT_EQ(nv.ispr_reg(), 0b1111011100110011);
}

TEST(NVICRegisters, ICPR_ReadIsCoherent) {
	using namespace micromachine::system;
	const uint32_t initial_pending_statuses = 0b1111011100110001;
	nvic nv = nvic::with_pending_statuses(initial_pending_statuses);
	EXPECT_EQ(nv.icpr_reg(), initial_pending_statuses);
}

TEST(NVICRegisters, ICPR_WriteZeroHasNoEffect) {
	using namespace micromachine::system;
	const uint32_t initial_pending_statuses = 0b1111011100110001;
	nvic nv = nvic::with_pending_statuses(initial_pending_statuses);
	nv.icpr_reg() = 0;
	EXPECT_EQ(nv.icpr_reg(), initial_pending_statuses);
}

TEST(NVICRegisters, ICPR_WriteOneClearsEnabledStatus) {
	using namespace micromachine::system;
	nvic nv = nvic::with_pending_statuses(0b1111011100110001);
	nv.icpr_reg() = 0b0000000000000001;
	EXPECT_EQ(nv.icpr_reg(), 0b1111011100110000);
}

TEST(NVICRegisters, ICPR_ISPR_ReadsAreCoherent) {
	using namespace micromachine::system;
	const uint32_t initial_pending_statuses = 0b1111011100110001;
	nvic nv = nvic::with_pending_statuses(initial_pending_statuses);
	// disable interrupt 0
	nv.icpr_reg() = 0b0000000000000001;
	EXPECT_EQ(nv.icpr_reg(), 0b1111011100110000);
	EXPECT_EQ(nv.ispr_reg(), nv.icpr_reg());
}
