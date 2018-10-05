cmake_minimum_required(VERSION 3.2)
project(libaeabi-cortexm0-library)

set(RUNTIME_SRCS
	crt.S
	divdi3.S
	idiv.S
	idivmod.S
	ldivmod.S
	moddi3.S
	modsi3.S
	umoddi3.S
	umodsi3.S
	lasr.S
	lcmp.S
	llsl.S
	llsr.S
	lmul.S
	memmove.S
	memset.S
	ulcmp.S
	uread4.S
	uread8.S
	uwrite4.S
	uwrite8.S
)
set_property(SOURCE ${RUNTIME_SRCS} PROPERTY LANGUAGE C)
add_library(libaeabi OBJECT ${RUNTIME_SRCS})
set_target_properties(libaeabi PROPERTIES OUTPUT_NAME "libaeabi-cortexm0")