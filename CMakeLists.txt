cmake_minimum_required(VERSION 3.0.2)
project(micromachine)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")


find_package(Threads)


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
	lib/string_format.hpp
	lib/exec_dispatcher.hpp
	lib/dispatcher.hpp)

add_executable(micromachine ${HEADER_FILES} ${SOURCE_FILES})

include_directories(lib)
add_subdirectory(test)
