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
    lib/registers.hpp
    lib/apsr_register.hpp
    lib/memory.hpp
    lib/cpu.hpp
    lib/exec_mode.hpp
    lib/exception_vector.hpp
    lib/exception.hpp
)

add_executable(micromachine ${HEADER_FILES} ${SOURCE_FILES})

include_directories(lib)
add_subdirectory(test)
