cmake_minimum_required(VERSION 3.2)
project(fault_test)

add_executable(fault
	fault.c
)

add_library(alib alib.c alib.h)
add_micromachine_executable(fault)
target_link_libraries(fault PUBLIC alib)
generate_dissasembly(fault)
