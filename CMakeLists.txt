cmake_minimum_required(VERSION 3.0.2)

include(ExternalProject)

include(env.cmake)

project(micromachine_emu)

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

find_package(Threads)

include(cotire)

add_subdirectory(lib)
add_subdirectory(run)
add_subdirectory(debugger)
add_subdirectory(test)
