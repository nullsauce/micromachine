cmake_minimum_required(VERSION 3.2)
project(fault_test)

add_executable(fault
	fault.c
	$<TARGET_OBJECTS:system>
	$<TARGET_OBJECTS:libaeabi>
)
add_micromachine_executable(fault)
generate_dissasembly(fault)
