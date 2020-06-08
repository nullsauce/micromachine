cmake_minimum_required(VERSION 3.2)
project(cmsis-header-library)

set(HEADERS
	Core/Include/cmsis_compiler.h
	Core/Include/cmsis_gcc.h
	Core/Include/cmsis_version.h
	Core/Include/core_cm0plus.h
	micromachine/micromachine_mcu.h
	micromachine/micromachine.h
)

add_library(cmsis-headers INTERFACE)
target_include_directories(cmsis-headers PUBLIC INTERFACE Core/Include)
target_include_directories(cmsis-headers PUBLIC INTERFACE micromachine)
