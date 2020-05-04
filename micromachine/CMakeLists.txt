

set(SRCS
	elfio/elf_types.hpp
	elfio/elfio.hpp
	elfio/elfio_dump.hpp
	elfio/elfio_dynamic.hpp
	elfio/elfio_header.hpp
	elfio/elfio_note.hpp
	elfio/elfio_relocation.hpp
	elfio/elfio_section.hpp
	elfio/elfio_segment.hpp
	elfio/elfio_strings.hpp
	elfio/elfio_symbols.hpp
	elfio/elfio_utils.hpp
	utils/interruptible_signal.hpp
	utils/blocking_queue.hpp
	utils/signal.hpp
	control_signals.hpp
	exception/exception.hpp
	exception/exception_controller.hpp
	exception/exception_return_handler.hpp
	exception/exception_state.hpp
	exception/exception_vector.hpp
	execution/alu.hpp
	execution/context_switcher.hpp
	execution/exec.hpp
	execution/exec_dispatcher.hpp
	execution/execution_mode.hpp
	execution/interworking_brancher.hpp
	helpers/binops.hpp
	helpers/check.hpp
	helpers/disassembler.hpp
	helpers/string_format.hpp
	instruction/instruction_decoder.hpp
	instruction/instruction_pair.hpp
	instruction/instructions.hpp
	memory/mapping.hpp
	memory/memory.hpp
	memory/register_map.hpp
	registers/custom
	registers/custom/generic_io_reg.hpp
	registers/system_control
	registers/system_control/cpuid_reg.hpp
	registers/system_control/shpr2_reg.hpp
	registers/system_control/shpr3_reg.hpp
	registers/system_control/systick_control_reg.hpp
	registers/system_control/config_and_control_reg.hpp
	registers/special_registers/apsr_reg.hpp
	registers/special_registers/control_reg.hpp
	registers/core_registers/core_registers.hpp
	registers/special_registers/epsr_reg.hpp
	registers/event_register.hpp
	registers/special_registers/ipsr_reg.hpp
	registers/iregister.hpp
	registers/core_registers/pc_reg.hpp
	registers/special_registers/primask_reg.hpp
	registers/registers.hpp
	registers/core_registers/sp_reg.hpp
	registers/special_registers/special_registers.hpp
	registers/standard_reg.hpp
	registers/memory_mapped_reg.hpp
	registers/special_registers/xpsr_reg.hpp
	peripherals/usart/usart_reg.hpp
	peripherals/usart/usart_controller.hpp
	types/bits.hpp
	types/types.hpp
	cpu.hpp
	mcu.cpp
	mcu.hpp
	nvic.hpp
	programmer.hpp
	systick.hpp
)

add_library(micromachine STATIC ${SRCS})
set_target_properties(micromachine PROPERTIES COMPILE_FLAGS "${BUILD_FLAGS} -Wall -Werror -Wno-unused-variable -Wno-unused-function")

if (CMAKE_BUILD_TYPE MATCHES "Debug")
	message(STATUS "Enabling precondition checks")
	add_definitions(-DMICROMACHINE_ENABLE_CHECKS)
endif ()

target_include_directories(micromachine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(micromachine PROPERTIES LINKER_LANGUAGE CXX)
cotire(micromachine)
