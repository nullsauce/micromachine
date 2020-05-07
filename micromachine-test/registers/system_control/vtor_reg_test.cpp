//
// Created by fla on 17-5-8.
//


#include "registers/system_control/vtor_reg.hpp"

#include <gtest/gtest.h>

using namespace micromachine::system;

TEST(VectorTableOffsetReg, HasCorrectHardCodedSZeroValue) {
	vtable_offset_reg reg;
	EXPECT_EQ(0U, (uint32_t)reg);
}
