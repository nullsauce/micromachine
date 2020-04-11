add_subdirectory(md5sum_test)
add_subdirectory(timer_test)
add_subdirectory(arithmetic_test)

function(add_test_command)
	set(test_name "${ARGV0}")
	set(test_executable "${ARGV1}")
	set(expected_stdout_file "${ARGV2}")
	add_test(NAME ${test_name}
		COMMAND ${CMAKE_SOURCE_DIR}/test/test_caller.sh ${expected_stdout_file} ${MICROMACHINE_VM} ${test_executable}
	)
	set_tests_properties(${test_name} PROPERTIES LABELS sdkTests)
endfunction()

add_test_command(
	md5_test
	${CMAKE_CURRENT_BINARY_DIR}/md5sum_test/md5sum
		${CMAKE_CURRENT_SOURCE_DIR}/md5sum_test/expected_stdout.txt
)

add_test_command(
	timer_test
	${CMAKE_CURRENT_BINARY_DIR}/timer_test/timer_test
	${CMAKE_CURRENT_SOURCE_DIR}/timer_test/expected_stdout.txt
)

add_test_command(
	arithmetic_test
	${CMAKE_CURRENT_BINARY_DIR}/arithmetic_test/arithmetic_test
	${CMAKE_CURRENT_SOURCE_DIR}/arithmetic_test/expected_stdout.txt
)



