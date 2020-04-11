cmake_minimum_required(VERSION 3.2)
project(hello_world)

add_executable(hello_world
	main.c
)
add_micromachine_executable(hello_world)
generate_dissasembly(hello_world)
