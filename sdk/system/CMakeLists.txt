cmake_minimum_required(VERSION 3.2)
project(system-library)
set(SRCS
	startup.c
	interrupt_handlers.c
	tinyprintf.c
	random.c
	mem.c
	io.c
	$<TARGET_OBJECTS:libaeabi>
)

set(CMSIS_HEADERS
	include/CMSIS/Core/Include/cmsis_compiler.h
	include/CMSIS/Core/Include/cmsis_gcc.h
	include/CMSIS/Core/Include/cmsis_version.h
	include/CMSIS/Core/Include/core_cm0plus.h
)

set(HEADERS
	${CMSIS_HEADERS}
	include/startup.h
	include/control_registers.h
	include/instructions.h
	include/interrupt_handlers.h
	include/entrypoint.h
	include/systick.h
	include/tinyprintf.h
	include/mem.h
	include/system.h
	include/io.h
	include/micromachine_mcu.h
)
add_library(system STATIC  ${HEADERS} ${SRCS})
