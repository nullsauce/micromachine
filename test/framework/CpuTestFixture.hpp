/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_CPUTESTFIXTURE_HPP
#define MICROMACHINE_CPUTESTFIXTURE_HPP

#include "TestSystem.hpp"
#include "code_generator.hpp"

#include <gtest/gtest.h>
#include <deque>

#include "CpuMutationPredicate.hpp"
#include "RegisterMutationPredicate.hpp"
#include "MemoryMutationPredicate.hpp"



/**
 * Utility macro to declare tests with a test_case_name different
 * from the fixture class name.
 */
#define MICROMACHINE_TEST_F(test_case_name, test_name, test_fixture)\
  GTEST_TEST_(test_case_name, test_name, test_fixture, \
              ::testing::internal::GetTypeId<test_fixture>())


class CpuTestFixture : public ::testing::Test {
private:
	std::deque<TestSystem> _systems;
	code_generator _code_generator;

	const cpu& getPreviousCpu() {
		//ASSERT_TRUE((_systems.size() >= 2));
		return _systems[_systems.size() - 2].getCpu();
	}

	void stash() {
		_systems.push_back(_systems.back());

		// always keep the code generator pointing to
		// the current system
		_code_generator.set_mem(&getCpu().mem());
	}

protected:

	cpu& getCpu() {
		//ASSERT_FALSE((_systems.empty()));
		return _systems.back().getCpu();
	}

	code_generator& code_gen() {
		return _code_generator;
	}

	virtual void SetUp() override {
		// create the initial system and pushes it on the stack
		_systems.emplace_back();

		// immediately save the current system
		stash();

		// assembler will start emitting instructions at the initial code position
		_code_generator.set_write_address(TestSystem::INITIAL_PC);
	}

	CpuMutationPredicate ExpectThat() {
		return {getPreviousCpu(), getCpu()};
	}

	/// copies the current system and put it
	/// om top of the stack
	void Step() {
		// save the current system state
		stash();

		// step the cpu
		getCpu().step();
	}
};

#endif //MICROMACHINE_CPUTESTFIXTURE_HPP
