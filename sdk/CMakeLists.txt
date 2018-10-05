
project(micromachine-sdk)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_BINARY_DIR}/micromachine.toolchain")
cmake_minimum_required(VERSION 3.2)

function(add_micromachine_executable)
	set_property(TARGET ${ARGV0} PROPERTY C_STANDARD 11)
	set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/link.ld)
	set_target_properties(${ARGV0} PROPERTIES LINK_FLAGS "-T ${LINKER_SCRIPT}")
	set_target_properties(${ARGV0} PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})
endfunction()

function(generate_dissasembly)
	set(CROSS_OBJCOPY "${TARGET_PREFIX}objcopy")
	set(CROSS_OBJDUMP "${TARGET_PREFIX}objdump")
	set(INPUT_TARGET_NAME ${ARGV0})
	set(OUTPUT_TARGET_NAME ${INPUT_TARGET_NAME}_disassembly)
	set(OUTPUT_FILE "${OUTPUT_TARGET_NAME}.txt")
	add_custom_target(${OUTPUT_TARGET_NAME} ALL
		COMMAND
			${CROSS_OBJDUMP} -Mforce-thumb -marm  -D -S --prefix-addresses $<TARGET_FILE:${INPUT_TARGET_NAME}> > ${OUTPUT_FILE}
		DEPENDS
			${INPUT_TARGET_NAME}
		VERBATIM
	)
	set_target_properties(${OUTPUT_TARGET_NAME} PROPERTIES OUTFILE "${OUTPUT_FILE}")
endfunction()

add_subdirectory(runtime-abi)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/system/include)
add_subdirectory(system)
add_subdirectory(examples)
