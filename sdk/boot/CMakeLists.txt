cmake_minimum_required(VERSION 3.2)
project(boot-library)

set(ASM_SRCS
	boot.s
)

set_property(SOURCE ${ASM_SRCS} PROPERTY LANGUAGE C)
add_library(boot OBJECT ${ASM_SRCS})
