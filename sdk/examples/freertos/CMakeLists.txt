cmake_minimum_required(VERSION 3.2)
project(timer_test)

add_executable(freertos_example
	main.c
)
target_link_libraries(freertos_example PUBLIC freertos)
add_micromachine_executable(freertos_example)
