cmake_minimum_required(VERSION 3.2)
project(system-library)
set(SRCS
	startup.c
	interrupt_handlers.c
	tinyprintf.c
	random.c
	mem.c
	system.c
	$<TARGET_OBJECTS:libaeabi>
)
set(HEADERS
	include/startup.h
	include/control_registers.h
	include/instructions.h
	include/interrupt_handlers.h
	include/entrypoint.h
	include/systick.h
	include/tinyprintf.h
	include/mem.h
	include/system.h
)
add_library(system STATIC  ${HEADERS} ${SRCS})
target_compile_features(system PRIVATE c_std_11)
