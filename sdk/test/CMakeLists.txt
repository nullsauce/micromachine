
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
	${CMAKE_BINARY_DIR}/examples/md5sum/md5sum
	${CMAKE_SOURCE_DIR}/examples/md5sum/expected_stdout.txt
)

add_test_command(
	timer_test
	${CMAKE_BINARY_DIR}/examples/timer_test/timer_test
	${CMAKE_SOURCE_DIR}/examples/timer_test/expected_stdout.txt
)



