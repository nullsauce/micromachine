cmake_minimum_required(VERSION 3.2)
project(cmsis-library)

set(HEADERS
	Core/Include/cmsis_compiler.h
	Core/Include/cmsis_gcc.h
	Core/Include/cmsis_version.h
	Core/Include/core_cm0plus.h
	micromachine/micromachine_mcu.h
	micromachine/micromachine.h
)

add_library(cmsis INTERFACE)
target_include_directories(cmsis PUBLIC INTERFACE Core/Include)
target_include_directories(cmsis PUBLIC INTERFACE micromachine)
