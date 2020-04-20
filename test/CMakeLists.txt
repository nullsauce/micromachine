
include(gtest)

find_package(Threads)

set(ALL_TEST_OBJS "")

# This static lib holds all the common test sources
# and pulls the common dependencies such as:
# * gtest
# * micromachine
# all test targets link to this library and automatically
# inherit the proper includes and libraries
add_library(test_common STATIC
	framework/CpuTestHarness.hpp
	framework/CpuTestHarness.cpp
	framework/CpuMutationPredicate.cpp
	framework/CpuMutationPredicate.hpp
	framework/RegisterMutationPredicate.cpp
	framework/RegisterMutationPredicate.hpp
	framework/MemoryMutationPredicate.cpp
	framework/MemoryMutationPredicate.hpp
	framework/code_generator.hpp
	framework/code_generator.cpp
	framework/Checklist.hpp
	framework/TestSystem.hpp
	framework/CpuTestFixture.hpp
)
target_include_directories(test_common PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/framework")
target_link_libraries(test_common libgtest micromachine)
target_compile_definitions(test_common PUBLIC TEST_SRC_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

# Declares teo targets: 1) An object of the target test and adds it to ALL_TEST_OBJS
# and 2) An executable running the target test
macro(declare_test test_src_file)
	# generate a name from the source test file
	STRING(REGEX REPLACE "[\\./-]" "_" test_name ${test_src_file})
	set(test_lib_name test_${test_name})
	set(test_executable_name run_test_${test_name})

	# An OBJECT library built from the test file only. It is added to ALL_TEST_OBJS
	add_library(${test_lib_name} OBJECT ${test_src_file})
	# Link test_common so that include paths are set for compiling the source file
	target_link_libraries(${test_lib_name} test_common)
	# Append the compiled object to ALL_TEST_OBJS so that it can be linked in the
	# all_tests executable too.
	list(APPEND ALL_TEST_OBJS $<TARGET_OBJECTS:${test_lib_name}>)

	# An executable that will run only this test
	add_executable(${test_executable_name} $<TARGET_OBJECTS:${test_lib_name}>)
	# Link test_common so that the appropriate link dependecies are set.
	# Additionally link libgtest_main to allow the test to run
	target_link_libraries(${test_executable_name} test_common libgtest_main ${CMAKE_THREAD_LIBS_INIT})
endmacro()

declare_test(framework/CpuTestFixtureTests.cpp)

declare_test(instructions/adcRegisterInstrTest.cpp)
declare_test(instructions/addImmediateInstrTest.cpp)
declare_test(instructions/addRegisterInstrTest.cpp)
declare_test(instructions/addSPInstrTest.cpp)
declare_test(instructions/adrInstrTest.cpp)
declare_test(instructions/andRegisterInstrTest.cpp)
declare_test(instructions/asrImmediateInstrTest.cpp)
declare_test(instructions/asrRegisterInstrTest.cpp)
declare_test(instructions/bicRegisterInstrTest.cpp)
declare_test(instructions/blInstrTest.cpp)
declare_test(instructions/blxInstrTest.cpp)
declare_test(instructions/branchInstrTest.cpp)
declare_test(instructions/bxInstrTest.cpp)
declare_test(instructions/cmnRegisterInstrTest.cpp)
declare_test(instructions/cmpImmediateInstrTest.cpp)
declare_test(instructions/cmpRegisterInstrTest.cpp)
declare_test(instructions/eorRegisterInstrTest.cpp)
declare_test(instructions/ldmInstrTest.cpp)
declare_test(instructions/ldrbImmediateInstrTest.cpp)
declare_test(instructions/ldrbRegisterInstrTest.cpp)
declare_test(instructions/ldrhImmediateInstrTest.cpp)
declare_test(instructions/ldrhRegisterInstrTest.cpp)
declare_test(instructions/ldrImmediateInstrTest.cpp)
declare_test(instructions/ldrLiteralInstrTest.cpp)
declare_test(instructions/ldrRegisterInstrTest.cpp)
declare_test(instructions/ldrsbRegisterInstrTest.cpp)
declare_test(instructions/ldrshRegisterInstrTest.cpp)
declare_test(instructions/lslImmediateInstrTest.cpp)
declare_test(instructions/lslRegisterInstrTest.cpp)
declare_test(instructions/lsrImmediateInstrTest.cpp)
declare_test(instructions/lsrRegisterInstrTest.cpp)
declare_test(instructions/movImmediateInstrTest.cpp)
declare_test(instructions/movRegisterInstrTest.cpp)
declare_test(instructions/mulInstrTest.cpp)
declare_test(instructions/mvnRegisterInstrTest.cpp)
declare_test(instructions/nopInstrTest.cpp)
declare_test(instructions/orrRegisterInstrTest.cpp)
declare_test(instructions/popInstrTest.cpp)
declare_test(instructions/pushInstrTest.cpp)
declare_test(instructions/rev16InstrTest.cpp)
declare_test(instructions/revInstrTest.cpp)
declare_test(instructions/revshInstrTest.cpp)
declare_test(instructions/rorRegisterInstrTest.cpp)
declare_test(instructions/rsbImmediateInstrTest.cpp)
declare_test(instructions/sbcRegisterInstrTest.cpp)
declare_test(instructions/stmInstrTest.cpp)
declare_test(instructions/strbImmediateInstrTest.cpp)
declare_test(instructions/strbRegisterInstrTest.cpp)
declare_test(instructions/strhImmediateInstrTest.cpp)
declare_test(instructions/strhRegisterInstrTest.cpp)
declare_test(instructions/strImmediateInstrTest.cpp)
declare_test(instructions/strRegisterInstrTest.cpp)
declare_test(instructions/subImmediateInstrTest.cpp)
declare_test(instructions/subRegisterInstrTest.cpp)
declare_test(instructions/subSPInstrTest.cpp)
declare_test(instructions/svcInstrTest.cpp)
declare_test(instructions/sxtbInstrTest.cpp)
declare_test(instructions/sxthInstrTest.cpp)
declare_test(instructions/tstRegisterInstrTest.cpp)
declare_test(instructions/undInstrTest.cpp)
declare_test(instructions/uxtbInstrTest.cpp)
declare_test(instructions/uxthInstrTest.cpp)
declare_test(instructions/mrsInstrTest.cpp)
declare_test(instructions/msrInstrTest.cpp)
declare_test(instructions/isbInstrTest.cpp)
declare_test(instructions/dmbInstrTest.cpp)
declare_test(instructions/dsbInstrTest.cpp)
declare_test(instructions/cpsInstrTest.cpp)



declare_test(instructions-new-framework/adcRegisterInstrTest.cpp)
declare_test(instructions-new-framework/addImmediateInstrTest.cpp)
declare_test(instructions-new-framework/addRegisterInstrTest.cpp)
declare_test(instructions-new-framework/addSPInstrTest.cpp)
declare_test(instructions-new-framework/adrInstrTest.cpp)
declare_test(instructions-new-framework/andRegisterInstrTest.cpp)
declare_test(instructions-new-framework/asrImmediateInstrTest.cpp)
declare_test(instructions-new-framework/asrRegisterInstrTest.cpp)
declare_test(instructions-new-framework/bicRegisterInstrTest.cpp)
declare_test(instructions-new-framework/bkptInstrTest.cpp)
declare_test(instructions-new-framework/blInstrTest.cpp)
declare_test(instructions-new-framework/blxInstrTest.cpp)
declare_test(instructions-new-framework/branchInstrTest.cpp)
declare_test(instructions-new-framework/bxInstrTest.cpp)
declare_test(instructions-new-framework/cmnRegisterInstrTest.cpp)
declare_test(instructions-new-framework/cmpImmediateInstrTest.cpp)
declare_test(instructions-new-framework/cmpRegisterInstrTest.cpp)
declare_test(instructions-new-framework/eorRegisterInstrTest.cpp)
declare_test(instructions-new-framework/ldmInstrTest.cpp)
declare_test(instructions-new-framework/ldrbImmediateInstrTest.cpp)
declare_test(instructions-new-framework/ldrbRegisterInstrTest.cpp)
declare_test(instructions-new-framework/ldrhImmediateInstrTest.cpp)
declare_test(instructions-new-framework/ldrhRegisterInstrTest.cpp)
#declare_test(instructions-new-framework/ldrImmediateInstrTest.cpp)
declare_test(instructions-new-framework/ldrLiteralInstrTest.cpp)
declare_test(instructions-new-framework/ldrRegisterInstrTest.cpp)
declare_test(instructions-new-framework/ldrsbRegisterInstrTest.cpp)
declare_test(instructions-new-framework/ldrshRegisterInstrTest.cpp)
declare_test(instructions-new-framework/lslImmediateInstrTest.cpp)
declare_test(instructions-new-framework/lslRegisterInstrTest.cpp)
declare_test(instructions-new-framework/lsrImmediateInstrTest.cpp)
declare_test(instructions-new-framework/lsrRegisterInstrTest.cpp)
declare_test(instructions-new-framework/movImmediateInstrTest.cpp)
declare_test(instructions-new-framework/movRegisterInstrTest.cpp)
declare_test(instructions-new-framework/mulInstrTest.cpp)
declare_test(instructions-new-framework/mvnRegisterInstrTest.cpp)
declare_test(instructions-new-framework/nopInstrTest.cpp)
declare_test(instructions-new-framework/orrRegisterInstrTest.cpp)
declare_test(instructions-new-framework/popInstrTest.cpp)
declare_test(instructions-new-framework/pushInstrTest.cpp)
declare_test(instructions-new-framework/rev16InstrTest.cpp)
declare_test(instructions-new-framework/revInstrTest.cpp)
declare_test(instructions-new-framework/revshInstrTest.cpp)
#declare_test(instructions-new-framework/rorRegisterInstrTest.cpp)
#declare_test(instructions-new-framework/rsbImmediateInstrTest.cpp)
#declare_test(instructions-new-framework/sbcRegisterInstrTest.cpp)
#declare_test(instructions-new-framework/stmInstrTest.cpp)
#declare_test(instructions-new-framework/strbImmediateInstrTest.cpp)
#declare_test(instructions-new-framework/strbRegisterInstrTest.cpp)
#declare_test(instructions-new-framework/strhImmediateInstrTest.cpp)
#declare_test(instructions-new-framework/strhRegisterInstrTest.cpp)
#declare_test(instructions-new-framework/strImmediateInstrTest.cpp)
#declare_test(instructions-new-framework/strRegisterInstrTest.cpp)
#declare_test(instructions-new-framework/subImmediateInstrTest.cpp)
#declare_test(instructions-new-framework/subRegisterInstrTest.cpp)
#declare_test(instructions-new-framework/subSPInstrTest.cpp)
#declare_test(instructions-new-framework/svcInstrTest.cpp)
#declare_test(instructions-new-framework/sxtbInstrTest.cpp)
#declare_test(instructions-new-framework/sxthInstrTest.cpp)
#declare_test(instructions-new-framework/tstRegisterInstrTest.cpp)
#declare_test(instructions-new-framework/undInstrTest.cpp)
#declare_test(instructions-new-framework/uxtbInstrTest.cpp)
#declare_test(instructions-new-framework/uxthInstrTest.cpp)
declare_test(instructions-new-framework/mrsInstrTest.cpp)
declare_test(instructions-new-framework/msrInstrTest.cpp)
declare_test(instructions-new-framework/isbInstrTest.cpp)
declare_test(instructions-new-framework/dmbInstrTest.cpp)
declare_test(instructions-new-framework/dsbInstrTest.cpp)
declare_test(instructions-new-framework/cpsInstrTest.cpp)


declare_test(binary_helpers_tests.cpp)
declare_test(exceptions/exception_prioritisation.cpp)
declare_test(exceptions/exception_vector.cpp)
declare_test(systick.cpp)
declare_test(nvic.cpp)

# disabled until we dont rely anymore on bkp to stop execution flow.
#declare_test(instructions/bkptInstrTest.cpp)

# these tests have yet to be integrated
#declare_test(instructions/sevInstrTest.cpp)
#declare_test(instructions/wfeInstrTest.cpp)
#declare_test(instructions/wfiInstrTest.cpp)
#declare_test(instructions/yieldInstrTest.cpp)
#declare_test(alu_tests alu_tests.cpp)

#declare_test(disasm/disasm.cpp)

# Build a single executable with ALL tests collected in ALL_TEST_OBJS
add_executable(all_tests ${ALL_TEST_OBJS})
target_link_libraries(all_tests test_common libgtest_main ${CMAKE_THREAD_LIBS_INIT})
add_test(all_tests all_tests)
set_tests_properties(all_tests PROPERTIES LABELS cpuTests)

