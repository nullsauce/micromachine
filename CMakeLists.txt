cmake_minimum_required(VERSION 3.0.2)
project(micromachine)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
enable_testing()
find_package(GTest REQUIRED)
find_package(Threads)
include_directories(${GTEST_INCLUDE_DIRS})

set(SOURCE_FILES
    main.cpp
)

set(HEADER_FILES
    lib/binops.hpp
    lib/types.hpp
    lib/integer_type.hpp
    lib/instructions.hpp
    lib/precond.hpp
    lib/exec.hpp
    lib/alu.hpp
    lib/register.hpp
    lib/register_set.hpp
    lib/apsr_register.hpp
    lib/memory.hpp
    lib/cpu.hpp
)

add_executable(micromachine ${HEADER_FILES} ${SOURCE_FILES})

macro(declare_test test_name test_files)
    add_executable(${test_name} ${HEADER_FILES} ${test_files})
    target_link_libraries(${test_name} ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
    add_test(all_tests ${test_name})
endmacro()

declare_test(integer_tests test/integer_tests.cpp)
declare_test(alu_tests test/alu_tests.cpp)
