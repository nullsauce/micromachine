cmake_minimum_required(VERSION 3.2)
project(system-library)
set(SRCS
	startup.c
	interrupt_handlers.c
	io.c
)

set(ASM_SRCS
	interrupt_vector.s
)
set_property(SOURCE ${ASM_SRCS} PROPERTY LANGUAGE C)

set(HEADERS
	include/startup.h
	include/control_registers.h
	include/instructions.h
	include/interrupt_handlers.h
	include/system.h
)

add_library(system STATIC ${HEADERS} ${ASM_SRCS} ${SRCS})
