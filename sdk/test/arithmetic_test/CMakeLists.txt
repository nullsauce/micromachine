cmake_minimum_required(VERSION 3.2)
project(arithmetictest)

add_executable(arithmetic_test
		arithmetic_test.c
)

add_micromachine_executable(arithmetic_test)
generate_dissasembly(arithmetic_test)
