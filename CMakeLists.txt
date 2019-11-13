cmake_minimum_required(VERSION 3.0.2)

include(ExternalProject)
include(env.cmake)

project(micromachine_emu)
enable_testing()

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Winit-self -Wextra -Wno-ignored-qualifiers  -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-maybe-uninitialized")

find_package(Threads)

include(cotire)

add_subdirectory(lib)
add_subdirectory(run)
#add_subdirectory(debugger)
add_subdirectory(termdbg)
add_subdirectory(test)
add_subdirectory(benchmark)

ExternalProject_Add(micromachine_sdk_project
	SOURCE_DIR ${CMAKE_SOURCE_DIR}/sdk
	BINARY_DIR ${CMAKE_SOURCE_DIR}/sdk
	CONFIGURE_COMMAND cmake -DCMAKE_TOOLCHAIN_FILE=${CMAKE_SOURCE_DIR}/sdk/micromachine.toolchain
	-DMICROMACHINE_VM=$<TARGET_FILE:run_micromachine>
	BUILD_COMMAND make
	INSTALL_COMMAND ""
	TEST_COMMAND ctest -V
)

add_custom_target(sdk ALL DEPENDS micromachine_sdk_project)
