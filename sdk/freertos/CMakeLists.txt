cmake_minimum_required(VERSION 3.2)
project(system-library)

set(SRCS
	croutine.c
	event_groups.c
	list.c
	queue.c
	readme.txt
	stream_buffer.c
	tasks.c
	timers.c
	port.c
)

set(HEADERS
	include/croutine.h
	include/deprecated_definitions.h
	include/event_groups.h
	include/FreeRTOS.h
	include/list.h
	include/message_buffer.h
	include/mpu_prototypes.h
	include/mpu_wrappers.h
	include/portable.h
	include/projdefs.h
	include/queue.h
	include/semphr.h
	include/stack_macros.h
	include/StackMacros.h
	include/stdint.readme
	include/stream_buffer.h
	include/task.h
	include/timers.h
	include/portmacro.h
)

add_library(freertos STATIC ${HEADERS} ${SRCS})
target_include_directories(freertos PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
set_property(TARGET freertos PROPERTY C_STANDARD 11)
