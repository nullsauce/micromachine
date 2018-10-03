cmake_minimum_required(VERSION 3.2)
project(test_image)

function(make_program)
	set(options "")
	set(oneValueArgs TARGET LINKER_SCRIPT CFLAGS)
	set(multiValueArgs C_SOURCES ASM_SOURCES)
	cmake_parse_arguments(make_program "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
	set(SOURCES ${make_program_C_SOURCES})
	set(ASM_SOURCES ${make_program_ASM_SOURCES})
	set(TARGET ${make_program_TARGET})
	set(ADDITIONAL_CFLAGS ${make_program_CFLAGS})
	set(LINKER_SCRIPT ${make_program_LINKER_SCRIPT})

	set(CROSS_OBJCOPY "${TARGET_PREFIX}objcopy")
	set(CROSS_OBJDUMP "${TARGET_PREFIX}objdump")

	set_property(SOURCE ${ASM_SOURCES} PROPERTY LANGUAGE C)

	set(ELF_TARGET "${TARGET}")
	set(BIN_TARGET "${TARGET}_bin")
	set(DISASM_TARGET "${TARGET}_disasm")
	set(DISASM_FILE_NAME "${DISASM_TARGET}.txt")
	set(DISASM_FULL_FILE_NAME "${DISASM_TARGET}-full.txt")
	set(BINARY_FILE_NAME "${BIN_TARGET}.bin")
	set(HEADER_TARGET "${TARGET}_h")
	set(HEADER_ARRAY_FILE_NAME "${TARGET}.h")


	set_property(SOURCE ${ASM_SOURCES} PROPERTY LANGUAGE C)

	add_executable(${ELF_TARGET} ${ASM_SOURCES} ${SOURCES})
	set_target_properties(${ELF_TARGET} PROPERTIES COMPILE_FLAGS "-O3 ${CMAKE_C_FLAGS}")
	set_target_properties(${ELF_TARGET} PROPERTIES LINK_FLAGS "-T ${LINKER_SCRIPT}")
	set_property(TARGET ${ELF_TARGET} PROPERTY C_STANDARD 99)
	set_target_properties(${ELF_TARGET} PROPERTIES PREFIX "")
	#set_target_properties(${ELF_TARGET} PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})

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
		COMMAND ${CROSS_OBJDUMP} -Mforce-thumb -marm  -D -S --prefix-addresses $<TARGET_FILE:${ELF_TARGET}> >
		${DISASM_FULL_FILE_NAME}
		DEPENDS ${BIN_TARGET}
		VERBATIM
	)
	set_target_properties(${DISASM_NAME} PROPERTIES OUTFILE "${DISASM_FILE_NAME}")
endfunction()


make_program(
	TARGET simple_c
	CFLAGS "-O0"
	LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/link.ld
	C_SOURCES simple.c
	ASM_SOURCES boot.s
)

make_program(
	TARGET performance_test
	CFLAGS "-O3"
	LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/link.ld
	C_SOURCES performance.c system/tinyprintf.h system/tinyprintf.c
	ASM_SOURCES boot.s div.S lmul.S uldivmod.S
)

#[[
make_program(
	TARGET cpptest
	CFLAGS "-O3"
	LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/link.ld
	C_SOURCES cpptest.cpp
	ASM_SOURCES boot.s
)
]]
