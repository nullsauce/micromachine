cmake_minimum_required(VERSION 3.2)
project(cmsis_examples)

add_executable(cmsis_example
	main.c
)
add_micromachine_executable(cmsis_example)
target_link_libraries(cmsis_example cmsis)
generate_dissasembly(cmsis_example)
