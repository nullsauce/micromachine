//
// Created by fla on 17-5-8.
//


#include "registers/system_control/config_and_control_reg.hpp"


#include <gtest/gtest.h>

using namespace micromachine::system;


TEST(ConfigAndControlReg, HasCorrectHardCodedStackAlignFlag) {
	config_and_control_reg reg;
	EXPECT_TRUE(reg.stack_align());
}

TEST(ConfigAndControlReg, HasCorrectHardCodedUnalignTrapFlag) {
	config_and_control_reg reg;
	EXPECT_TRUE(reg.unaligned_trap());
}

TEST(ConfigAndControlReg, HasCorrectHardCodedValue) {
	uint32_t expectedValue = 0;
	config_and_control_reg::unalign_trap_bit::of(expectedValue) = true;
	config_and_control_reg::stack_align_bit::of(expectedValue) = true;
	config_and_control_reg reg;
	EXPECT_EQ(expectedValue, (uint32_t)reg);
}
