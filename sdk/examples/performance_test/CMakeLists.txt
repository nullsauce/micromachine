cmake_minimum_required(VERSION 3.2)
project(performancetest)

add_executable(performance_test
	performance.c
	$<TARGET_OBJECTS:system>
	$<TARGET_OBJECTS:libaeabi>
)
add_micromachine_executable(performance_test)
generate_dissasembly(performance_test)