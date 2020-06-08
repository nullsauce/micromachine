cmake_minimum_required(VERSION 3.2)
project(timer_test)

add_executable(timer_test
	timer.c
)
add_micromachine_executable(timer_test)
target_link_libraries(timer_test PUBLIC cmsis)
generate_dissasembly(timer_test)
