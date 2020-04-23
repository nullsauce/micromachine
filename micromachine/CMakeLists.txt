
set(SRCS
	binops.hpp
	types.hpp
	instruction_pair.hpp
	instructions.hpp
	precond.hpp
	exec.hpp
	alu.hpp
	registers/core_registers.hpp
	registers/special_registers.hpp
	registers/xpsr_reg.hpp
	registers/apsr_reg.hpp
	registers/ipsr_reg.hpp
	registers/epsr_reg.hpp
	registers/ireg.hpp
	registers/pc_reg.hpp
	registers/sp_reg.hpp
	registers/standard_reg.hpp
	registers/control_reg.hpp
	registers/system_control/cpuid_reg.hpp
	registers/system_control/shpr2_reg.hpp
	registers/system_control/shpr3_reg.hpp
	registers/system_control/systick_control_reg.hpp
	registers/custom/generic_io_reg.hpp
	registers/event_register.hpp
	memory/memory.hpp
	nvic.hpp
	string_format.hpp
	exec_dispatcher.hpp
	interworking_brancher.hpp
	exception_defs.hpp
	interrupter.hpp
	dispatcher.hpp
	disasm.hpp
	cpu.hpp
	mcu.hpp
	mcu.cpp
)

add_library(micromachine STATIC ${SRCS})
set_target_properties(micromachine PROPERTIES COMPILE_FLAGS "${BUILD_FLAGS} -Wall -Werror -Wno-unused-variable -Wno-unused-function")

if (CMAKE_BUILD_TYPE MATCHES "Debug")
	message(STATUS "Enabling precondition checks")
	add_definitions(-DMICROMACHINE_ENABLE_PRECOND_CHECKS)
endif ()

target_include_directories(micromachine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(micromachine PROPERTIES LINKER_LANGUAGE CXX)
cotire(micromachine)
