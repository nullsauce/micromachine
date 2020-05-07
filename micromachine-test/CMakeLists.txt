
include(gtest)
find_package(Threads)
enable_testing()

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
	framework/CodeGenerator.hpp
	framework/CodeGenerator.cpp
	framework/TestMachine.hpp
	framework/CpuTestFixture.hpp
	framework/MachineTestHarness.hpp
	framework/MachineTestHarness.cpp
	framework/Predicates.hpp
	framework/Predicates.cpp
	framework/PredicateHelpers.hpp
	framework/PredicateHelpers.cpp
	framework/TestHelpers.hpp
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

declare_test(instruction-tests/adcRegisterInstrTest.cpp)
declare_test(instruction-tests/addImmediateInstrTest.cpp)
declare_test(instruction-tests/addRegisterInstrTest.cpp)
declare_test(instruction-tests/addSPInstrTest.cpp)
declare_test(instruction-tests/adrInstrTest.cpp)
declare_test(instruction-tests/andRegisterInstrTest.cpp)
declare_test(instruction-tests/asrImmediateInstrTest.cpp)
declare_test(instruction-tests/asrRegisterInstrTest.cpp)
declare_test(instruction-tests/bicRegisterInstrTest.cpp)
declare_test(instruction-tests/bkptInstrTest.cpp)
declare_test(instruction-tests/blInstrTest.cpp)
declare_test(instruction-tests/blxInstrTest.cpp)
declare_test(instruction-tests/branchInstrTest.cpp)
declare_test(instruction-tests/bxInstrTest.cpp)
declare_test(instruction-tests/cmnRegisterInstrTest.cpp)
declare_test(instruction-tests/cmpImmediateInstrTest.cpp)
declare_test(instruction-tests/cmpRegisterInstrTest.cpp)
declare_test(instruction-tests/eorRegisterInstrTest.cpp)
declare_test(instruction-tests/ldmInstrTest.cpp)
declare_test(instruction-tests/ldrbImmediateInstrTest.cpp)
declare_test(instruction-tests/ldrbRegisterInstrTest.cpp)
declare_test(instruction-tests/ldrhImmediateInstrTest.cpp)
declare_test(instruction-tests/ldrhRegisterInstrTest.cpp)
declare_test(instruction-tests/ldrImmediateInstrTest.cpp)
declare_test(instruction-tests/ldrLiteralInstrTest.cpp)
declare_test(instruction-tests/ldrRegisterInstrTest.cpp)
declare_test(instruction-tests/ldrsbRegisterInstrTest.cpp)
declare_test(instruction-tests/ldrshRegisterInstrTest.cpp)
declare_test(instruction-tests/lslImmediateInstrTest.cpp)
declare_test(instruction-tests/lslRegisterInstrTest.cpp)
declare_test(instruction-tests/lsrImmediateInstrTest.cpp)
declare_test(instruction-tests/lsrRegisterInstrTest.cpp)
declare_test(instruction-tests/movImmediateInstrTest.cpp)
declare_test(instruction-tests/movRegisterInstrTest.cpp)
declare_test(instruction-tests/mulInstrTest.cpp)
declare_test(instruction-tests/mvnRegisterInstrTest.cpp)
declare_test(instruction-tests/nopInstrTest.cpp)
declare_test(instruction-tests/orrRegisterInstrTest.cpp)
declare_test(instruction-tests/popInstrTest.cpp)
declare_test(instruction-tests/pushInstrTest.cpp)
declare_test(instruction-tests/rev16InstrTest.cpp)
declare_test(instruction-tests/revInstrTest.cpp)
declare_test(instruction-tests/revshInstrTest.cpp)
declare_test(instruction-tests/rorRegisterInstrTest.cpp)
declare_test(instruction-tests/rsbImmediateInstrTest.cpp)
declare_test(instruction-tests/sbcRegisterInstrTest.cpp)
declare_test(instruction-tests/sevInstrTest.cpp)
declare_test(instruction-tests/stmInstrTest.cpp)
declare_test(instruction-tests/strbImmediateInstrTest.cpp)
declare_test(instruction-tests/strbRegisterInstrTest.cpp)
declare_test(instruction-tests/strhImmediateInstrTest.cpp)
declare_test(instruction-tests/strhRegisterInstrTest.cpp)
declare_test(instruction-tests/strImmediateInstrTest.cpp)
declare_test(instruction-tests/strRegisterInstrTest.cpp)
declare_test(instruction-tests/subImmediateInstrTest.cpp)
declare_test(instruction-tests/subRegisterInstrTest.cpp)
declare_test(instruction-tests/subSPInstrTest.cpp)
declare_test(instruction-tests/svcInstrTest.cpp)
declare_test(instruction-tests/sxtbInstrTest.cpp)
declare_test(instruction-tests/sxthInstrTest.cpp)
declare_test(instruction-tests/tstRegisterInstrTest.cpp)
declare_test(instruction-tests/undInstrTest.cpp)
declare_test(instruction-tests/uxtbInstrTest.cpp)
declare_test(instruction-tests/uxthInstrTest.cpp)
declare_test(instruction-tests/mrsInstrTest.cpp)
declare_test(instruction-tests/msrInstrTest.cpp)
declare_test(instruction-tests/isbInstrTest.cpp)
declare_test(instruction-tests/dmbInstrTest.cpp)
declare_test(instruction-tests/dsbInstrTest.cpp)
declare_test(instruction-tests/cpsInstrTest.cpp)
declare_test(instruction-tests/wfeInstrTest.cpp)
declare_test(instruction-tests/wfiInstrTest.cpp)
declare_test(instruction-tests/yieldInstrTest.cpp)

declare_test(binary_helpers_tests.cpp)
declare_test(exceptions/exception_prioritisation.cpp)
declare_test(exceptions/exception_vector.cpp)
declare_test(exceptions/exception_vector_reset.cpp)
declare_test(systick_control_reg.cpp)
declare_test(config_and_control_reg.cpp)
declare_test(interrupt_and_reset_control_reg.cpp)
declare_test(vtable_offset_reg.cpp)
declare_test(icsr_reg.cpp)
declare_test(nvic.cpp)
declare_test(reset/reset_tests.cpp)
declare_test(reset/exception_state_reset.cpp)
declare_test(peripherals/usart-tests.cpp)
#declare_test(disasm/disasm.cpp)


option(BUILD_LEGACY_INSTRUCTION_TESTS "Build the legacy instruction tests" OFF)

if(BUILD_LEGACY_INSTRUCTION_TESTS)
	message(STATUS "Legacy tests will be built and run")

	declare_test(legacy-instruction-tests/adcRegisterInstrTest.cpp)
	declare_test(legacy-instruction-tests/addImmediateInstrTest.cpp)
	declare_test(legacy-instruction-tests/addRegisterInstrTest.cpp)
	declare_test(legacy-instruction-tests/addSPInstrTest.cpp)
	declare_test(legacy-instruction-tests/adrInstrTest.cpp)
	declare_test(legacy-instruction-tests/andRegisterInstrTest.cpp)
	declare_test(legacy-instruction-tests/asrImmediateInstrTest.cpp)
	declare_test(legacy-instruction-tests/asrRegisterInstrTest.cpp)
	declare_test(legacy-instruction-tests/bicRegisterInstrTest.cpp)
	declare_test(legacy-instruction-tests/blInstrTest.cpp)
	declare_test(legacy-instruction-tests/blxInstrTest.cpp)
	declare_test(legacy-instruction-tests/branchInstrTest.cpp)
	declare_test(legacy-instruction-tests/bxInstrTest.cpp)
	declare_test(legacy-instruction-tests/cmnRegisterInstrTest.cpp)
	declare_test(legacy-instruction-tests/cmpImmediateInstrTest.cpp)
	declare_test(legacy-instruction-tests/cmpRegisterInstrTest.cpp)
	declare_test(legacy-instruction-tests/eorRegisterInstrTest.cpp)
	declare_test(legacy-instruction-tests/ldmInstrTest.cpp)
	declare_test(legacy-instruction-tests/ldrbImmediateInstrTest.cpp)
	declare_test(legacy-instruction-tests/ldrbRegisterInstrTest.cpp)
	declare_test(legacy-instruction-tests/ldrhImmediateInstrTest.cpp)
	declare_test(legacy-instruction-tests/ldrhRegisterInstrTest.cpp)
	declare_test(legacy-instruction-tests/ldrImmediateInstrTest.cpp)
	declare_test(legacy-instruction-tests/ldrLiteralInstrTest.cpp)
	declare_test(legacy-instruction-tests/ldrRegisterInstrTest.cpp)
	declare_test(legacy-instruction-tests/ldrsbRegisterInstrTest.cpp)
	declare_test(legacy-instruction-tests/ldrshRegisterInstrTest.cpp)
	declare_test(legacy-instruction-tests/lslImmediateInstrTest.cpp)
	declare_test(legacy-instruction-tests/lslRegisterInstrTest.cpp)
	declare_test(legacy-instruction-tests/lsrImmediateInstrTest.cpp)
	declare_test(legacy-instruction-tests/lsrRegisterInstrTest.cpp)
	declare_test(legacy-instruction-tests/movImmediateInstrTest.cpp)
	declare_test(legacy-instruction-tests/movRegisterInstrTest.cpp)
	declare_test(legacy-instruction-tests/mulInstrTest.cpp)
	declare_test(legacy-instruction-tests/mvnRegisterInstrTest.cpp)
	declare_test(legacy-instruction-tests/nopInstrTest.cpp)
	declare_test(legacy-instruction-tests/orrRegisterInstrTest.cpp)
	declare_test(legacy-instruction-tests/popInstrTest.cpp)
	declare_test(legacy-instruction-tests/pushInstrTest.cpp)
	declare_test(legacy-instruction-tests/rev16InstrTest.cpp)
	declare_test(legacy-instruction-tests/revInstrTest.cpp)
	declare_test(legacy-instruction-tests/revshInstrTest.cpp)
	declare_test(legacy-instruction-tests/rorRegisterInstrTest.cpp)
	declare_test(legacy-instruction-tests/rsbImmediateInstrTest.cpp)
	declare_test(legacy-instruction-tests/sbcRegisterInstrTest.cpp)
	declare_test(legacy-instruction-tests/stmInstrTest.cpp)
	declare_test(legacy-instruction-tests/strbImmediateInstrTest.cpp)
	declare_test(legacy-instruction-tests/strbRegisterInstrTest.cpp)
	declare_test(legacy-instruction-tests/strhImmediateInstrTest.cpp)
	declare_test(legacy-instruction-tests/strhRegisterInstrTest.cpp)
	declare_test(legacy-instruction-tests/strImmediateInstrTest.cpp)
	declare_test(legacy-instruction-tests/strRegisterInstrTest.cpp)
	declare_test(legacy-instruction-tests/subImmediateInstrTest.cpp)
	declare_test(legacy-instruction-tests/subRegisterInstrTest.cpp)
	declare_test(legacy-instruction-tests/subSPInstrTest.cpp)
	declare_test(legacy-instruction-tests/svcInstrTest.cpp)
	declare_test(legacy-instruction-tests/sxtbInstrTest.cpp)
	declare_test(legacy-instruction-tests/sxthInstrTest.cpp)
	declare_test(legacy-instruction-tests/tstRegisterInstrTest.cpp)
	declare_test(legacy-instruction-tests/undInstrTest.cpp)
	declare_test(legacy-instruction-tests/uxtbInstrTest.cpp)
	declare_test(legacy-instruction-tests/uxthInstrTest.cpp)
	declare_test(legacy-instruction-tests/mrsInstrTest.cpp)
	declare_test(legacy-instruction-tests/msrInstrTest.cpp)
	declare_test(legacy-instruction-tests/isbInstrTest.cpp)
	declare_test(legacy-instruction-tests/dmbInstrTest.cpp)
	declare_test(legacy-instruction-tests/dsbInstrTest.cpp)
	declare_test(legacy-instruction-tests/cpsInstrTest.cpp)

	add_executable(all_legacy_tests ${ALL_TEST_OBJS})
	target_link_libraries(all_legacy_tests test_common libgtest_main ${CMAKE_THREAD_LIBS_INIT})
	add_test(all_legacy_tests all_legacy_tests)
	set_tests_properties(all_legacy_tests PROPERTIES LABELS cpuTests)

endif()


# Build a single executable with ALL tests collected in ALL_TEST_OBJS
add_executable(all_tests ${ALL_TEST_OBJS})
target_link_libraries(all_tests test_common libgtest_main ${CMAKE_THREAD_LIBS_INIT})
add_test(all_tests all_tests)
set_tests_properties(all_tests PROPERTIES LABELS cpuTests)

