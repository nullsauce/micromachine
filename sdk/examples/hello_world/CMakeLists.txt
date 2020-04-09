cmake_minimum_required(VERSION 3.2)
project(hello_world)

add_executable(hello_world
	main.c
	$<TARGET_OBJECTS:system>
	$<TARGET_OBJECTS:libaeabi>
)
add_micromachine_executable(hello_world)
generate_dissasembly(hello_world)
