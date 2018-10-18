cmake_minimum_required(VERSION 3.2)
project(cpptest)

add_executable(cpptest
		cpp.cpp
	$<TARGET_OBJECTS:system>
	$<TARGET_OBJECTS:libaeabi>
)
add_micromachine_executable(cpptest)
generate_dissasembly(cpptest)