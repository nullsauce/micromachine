add_subdirectory(md5sum_test)
add_subdirectory(timer_test)
add_subdirectory(arithmetic_test)
add_subdirectory(static_initialization)
add_subdirectory(cpuid_test)
add_subdirectory(usart_test)

# pass the --testing flag to the vm to enable various testing behaviors
# such as filling uninitialized memory with non-zero values
set(MICROMACHINE_VM_OPTS "--testing")

enable_testing()

function(add_test_command)
	set(oneValueArgs NAME EXECUTABLE EXPECTED_OUTPUT_FILE)
	set(multiValueArgs VM_ARGS)
	cmake_parse_arguments(this_test "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	add_test(NAME ${this_test_NAME}
		COMMAND ${CMAKE_SOURCE_DIR}/test/test_caller.sh ${this_test_EXPECTED_OUTPUT_FILE} ${MICROMACHINE_VM}
		${MICROMACHINE_VM_OPTS} ${this_test_VM_ARGS} ${this_test_EXECUTABLE}
	)
	set_tests_properties(${test_name} PROPERTIES LABELS sdkTests)
endfunction()

add_test_command(
	NAME md5_test
	EXECUTABLE ${CMAKE_CURRENT_BINARY_DIR}/md5sum_test/md5sum
	EXPECTED_OUTPUT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/md5sum_test/expected_stdout.txt
)

add_test_command(
	NAME timer_test
	EXECUTABLE ${CMAKE_CURRENT_BINARY_DIR}/timer_test/timer_test
	EXPECTED_OUTPUT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/timer_test/expected_stdout.txt
)

add_test_command(
	NAME arithmetic_test
	EXECUTABLE ${CMAKE_CURRENT_BINARY_DIR}/arithmetic_test/arithmetic_test
	EXPECTED_OUTPUT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/arithmetic_test/expected_stdout.txt
)

add_test_command(
	NAME data_nonzero_initialization
	EXECUTABLE ${CMAKE_CURRENT_BINARY_DIR}/static_initialization/initialized_data_value
	EXPECTED_OUTPUT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/static_initialization/initialized_data_value_expected_stdout.txt
)

add_test_command(
	NAME bss_zero_initialization
	EXECUTABLE ${CMAKE_CURRENT_BINARY_DIR}/static_initialization/bss_is_zeroed
	EXPECTED_OUTPUT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/static_initialization/bss_is_zeroed_expected_stdout.txt
)

add_test_command(
	NAME data_re_initialization
	EXECUTABLE ${CMAKE_CURRENT_BINARY_DIR}/static_initialization/re_initialized_data_value
	EXPECTED_OUTPUT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/static_initialization/re_initialized_data_value_expected_stdout.txt
)

add_test_command(
	NAME cpuid_test
	EXECUTABLE ${CMAKE_CURRENT_BINARY_DIR}/cpuid_test/cpuid_test
	EXPECTED_OUTPUT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cpuid_test/expected_stdout.txt
)

add_test_command(
	NAME usart_simple_test
	EXECUTABLE ${CMAKE_CURRENT_BINARY_DIR}/usart_test/usart_simple_test
	EXPECTED_OUTPUT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/usart_test/usart_simple_expected_stdout.txt
	VM_ARGS --usart-input-string '<vm> Hello app!'
)


add_test_command(
	NAME usart_interrupt_test
	EXECUTABLE ${CMAKE_CURRENT_BINARY_DIR}/usart_test/usart_interrupt_test
	EXPECTED_OUTPUT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/usart_test/usart_interrupt_expected_stdout.txt
	VM_ARGS --usart-input-string '<vm> Hello app!'
)
