cmake_minimum_required(VERSION 3.2)
project(system-library)
set(SRCS
	startup.c
	interrupt_handlers.c
	io.c
)

set(HEADERS
	include/startup.h
	include/control_registers.h
	include/instructions.h
	include/interrupt_handlers.h
	include/system.h
)
add_library(system OBJECT ${HEADERS} ${SRCS})
target_link_libraries(system PUBLIC cmsis)
