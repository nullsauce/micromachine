cmake_minimum_required(VERSION 3.2)
project(cpptest)

add_executable(cpptest
	cpp.cpp
)
add_micromachine_executable(cpptest)
generate_dissasembly(cpptest)
