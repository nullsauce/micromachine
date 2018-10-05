cmake_minimum_required(VERSION 3.2)
project(system-library)

set(ASM_SRCS
	boot.s
)
set(SRCS
	interrupt_handlers.c
	tinyprintf.c
	random.c
)
set(HEADERS
	include/control_registers.h
	include/instructions.h
	include/interrupt_handlers.h
	include/systick.h
	include/tinyprintf.h
)
set_property(SOURCE ${ASM_SRCS} PROPERTY LANGUAGE C)
include_directories(include)
add_library(system OBJECT ${ASM_SRCS} ${HEADERS} ${SRCS})