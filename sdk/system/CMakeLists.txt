cmake_minimum_required(VERSION 3.2)
project(system-library)

set(ASM_SRCS
	boot.s
)
set(SRCS
	interrupt_handlers.c
	tinyprintf.c
	random.c
	mem.c
)
set(HEADERS
	include/control_registers.h
	include/instructions.h
	include/interrupt_handlers.h
	include/entrypoint.h
	include/systick.h
	include/tinyprintf.h
	include/mem.h
)
set_property(SOURCE ${ASM_SRCS} PROPERTY LANGUAGE C)
add_library(system OBJECT ${ASM_SRCS} ${HEADERS} ${SRCS})
set_property(TARGET system PROPERTY C_STANDARD 11)