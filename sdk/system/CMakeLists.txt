cmake_minimum_required(VERSION 3.2)
project(system-library)

set(ASM_SRCS
	boot.s
)
set(SRCS
	tinyprintf.c
)
set(HEADERS
	include/tinyprintf.h
)
set_property(SOURCE ${ASM_SRCS} PROPERTY LANGUAGE C)
add_library(system OBJECT ${ASM_SRCS} ${SRCS})