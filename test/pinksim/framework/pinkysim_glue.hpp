//
// Created by fla on 17-5-20.
//

#ifndef MICROMACHINE_PINKYSIM_GLUE_HPP
#define MICROMACHINE_PINKYSIM_GLUE_HPP

#include "SimpleMemory_glue.hpp"
#define PASTER(x,y) x##_##y
#define NAME_GLUE(x,y) PASTER(x,y)
#define PINKYFY(classname) NAME_GLUE(PinkyTestBase, classname)
#define TEST_GROUP_BASE(b, c) class PINKYFY(b) : public c

#define PINKY_TEST(x,y) TEST_F(PINKYFY(x), y)
#define CHECK_EQUAL(...) EXPECT_EQ(__VA_ARGS__)

#endif //MICROMACHINE_PINKYSIM_GLUE_HPP
