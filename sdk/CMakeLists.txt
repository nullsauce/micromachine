cmake_minimum_required(VERSION 3.2)
project(test_image)


macro(make_program)
	set(options "")
	set(oneValueArgs TARGET LINKER_SCRIPT)
	set(multiValueArgs C_SOURCES ASM_SOURCES)
	cmake_parse_arguments(make_program "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
	set(SOURCES ${make_program_C_SOURCES})
	set(ASM_SOURCES ${make_program_ASM_SOURCES})
	set(TARGET ${make_program_TARGET})
	set(LINKER_SCRIPT ${make_program_LINKER_SCRIPT})


	set(TARGET_PREFIX "arm-none-eabi-")
	set(CMAKE_C_COMPILER "${TARGET_PREFIX}gcc")
	set(CMAKE_CXX_COMPILER "${TARGET_PREFIX}g++")
	set(CMAKE_LINKER "${TARGET_PREFIX}ld")
	set(CMAKE_ASM_COMPILER "${TARGET_PREFIX}as")

	set(CMAKE_C_LINK_FLAGS "${LD_FLAGS}")
	set(CMAKE_CXX_LINK_FLAGS "${CMAKE_C_LINK_FLAGS}")
	set(CMAKE_C_LINK_EXECUTABLE "<CMAKE_LINKER> -T ${CMAKE_SOURCE_DIR}/${LINKER_SCRIPT} <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET>")
	set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_C_LINK_EXECUTABLE}")
	set(CROSS_OBJCOPY "${TARGET_PREFIX}objcopy")
	set(CROSS_OBJDUMP "${TARGET_PREFIX}objdump")

	set(CMAKE_C_FLAGS "\
	-c \
	-g \
	-O2 \
	-std=c99 \
	-ffreestanding \
	-mcpu=cortex-m0 \
	-mthumb \
	-mfloat-abi=soft \
	-nostdlib \
	-march=armv6s-m \
	")


	set(LD_FLAGS "\
	-nostdlib \
	-nodefaultlibs \
	-nostartfiles \
	")

	set_property(SOURCE ${ASM_SOURCES} PROPERTY LANGUAGE C)

	set(ELF_TARGET "${TARGET}_elf")
	set(BIN_TARGET "${TARGET}_bin")
	set(DISASM_TARGET "${TARGET}_disasm")
	set(DISASM_FILE_NAME "${DISASM_TARGET}.txt")
	set(DISASM_FULL_FILE_NAME "${DISASM_TARGET}-full.txt")
	set(BINARY_FILE_NAME "${BIN_TARGET}.bin")
	set(HEADER_TARGET "${TARGET}_h")
	set(HEADER_ARRAY_FILE_NAME "${TARGET}.h")

	add_executable(${ELF_TARGET} ${ASM_SOURCES} ${SOURCES})
	set_target_properties(${ELF_TARGET} PROPERTIES PREFIX "")

	add_custom_target(${BIN_TARGET} ALL
		COMMAND ${CROSS_OBJCOPY} -O binary $<TARGET_FILE:${ELF_TARGET}> ${BINARY_FILE_NAME}
		DEPENDS ${ELF_TARGET}
		VERBATIM
	)
	set_target_properties(${BIN_TARGET} PROPERTIES OUTFILE "${BINARY_FILE_NAME}")

	add_custom_target(${HEADER_TARGET} ALL
			COMMAND xxd -i $<TARGET_PROPERTY:${BIN_TARGET},OUTFILE> > ${HEADER_ARRAY_FILE_NAME}
			DEPENDS ${ELF_TARGET}
			VERBATIM
			)
	set_target_properties(${HEADER_TARGET} PROPERTIES OUTFILE "${HEADER_ARRAY_FILE_NAME}")

	add_custom_target(${DISASM_TARGET} ALL
		COMMAND ${CROSS_OBJDUMP} -b binary -Mforce-thumb -marm -D --prefix-addresses  $<TARGET_PROPERTY:${BIN_TARGET},OUTFILE> > ${DISASM_FILE_NAME}
		COMMAND ${CROSS_OBJDUMP} -D -S $<TARGET_FILE:${ELF_TARGET}> > ${DISASM_FULL_FILE_NAME}
		DEPENDS ${BIN_TARGET}
		VERBATIM
	)
	set_target_properties(${DISASM_NAME} PROPERTIES OUTFILE "${DISASM_FILE_NAME}")
endmacro()


make_program(
	TARGET simple_c
	LINKER_SCRIPT link.ld
	C_SOURCES simple.c
	ASM_SOURCES boot.s
)

