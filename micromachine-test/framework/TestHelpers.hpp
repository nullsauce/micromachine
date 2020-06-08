#pragma once

/**
 * Utility macro to declare tests with a test_case_name different
 * from the fixture class name.
 */
#define MICROMACHINE_TEST_F(test_case_name, test_name, test_fixture)\
  GTEST_TEST_(test_case_name, test_name, test_fixture, ::testing::internal::GetTypeId<test_fixture>())
