cmake_minimum_required(VERSION 3.2)
project(malloctest)

add_executable(malloc_test
	malloc_test.c
)

add_micromachine_executable(malloc_test)
generate_dissasembly(malloc_test)
