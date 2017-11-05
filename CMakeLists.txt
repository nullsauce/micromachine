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
    lib/instruction_pair.hpp
    lib/integer_type.hpp
    lib/instructions.hpp
    lib/precond.hpp
    lib/exec.hpp
    lib/alu.hpp
    lib/registers/registers.hpp
    lib/registers/xpsr_reg.hpp
    lib/registers/apsr_reg.hpp
    lib/registers/ipsr_reg.hpp
    lib/registers/epsr_reg.hpp
    lib/registers/exec_mode_reg.hpp
    lib/registers/ireg.hpp
    lib/registers/pc_reg.hpp
    lib/registers/sp_reg.hpp
    lib/registers/standard_reg.hpp
    lib/registers/control_reg.hpp
    lib/memory.hpp
    lib/cpu.hpp
    lib/exception_vector.hpp
    lib/exception.hpp
    lib/string_format.hpp
    lib/exec_dispatcher.hpp
    lib/dispatcher.hpp
    lib/disasm.hpp
)

add_executable(micromachine ${HEADER_FILES} ${SOURCE_FILES})

include_directories(lib)
add_subdirectory(test)
