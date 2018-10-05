cmake_minimum_required(VERSION 3.0.2)

include(ExternalProject)
include(env.cmake)

project(micromachine_emu)
enable_testing()

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ")

find_package(Threads)

include(cotire)

add_subdirectory(lib)
add_subdirectory(run)
add_subdirectory(debugger)
add_subdirectory(test)

ExternalProject_Add(micromachine-sdk
	SOURCE_DIR ${CMAKE_SOURCE_DIR}/sdk
	BINARY_DIR ${CMAKE_SOURCE_DIR}/sdk/build
	CONFIGURE_COMMAND cmake -DCMAKE_TOOLCHAIN_FILE=${CMAKE_SOURCE_DIR}/sdk/micromachine.toolchain ${CMAKE_SOURCE_DIR}/sdk
	BUILD_COMMAND make
	INSTALL_COMMAND ""
)
