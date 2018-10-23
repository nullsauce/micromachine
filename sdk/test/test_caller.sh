#!/usr/bin/env bash
# usage ./test_caller.sh <stdout-file> <command>
# will return zero if the stdout generated by executing <command> is strictly equal
# to the content of <stdout-file>

# Example: ./test.sh expected_stdout.txt ../../../cmake-build-release/run/run_micromachine md5sum

actual_stdout_file="$(mktemp)"
expected_stdout_file="$1"
command_to_run="${@:2}"
${command_to_run} > ${actual_stdout_file}

if cmp --silent ${expected_stdout_file} ${actual_stdout_file}; then
    exit 0
else
    echo "Test failed:"
    echo "Expected stdout file: ${expected_stdout_file}"
    echo "Actual stdout file: ${actual_stdout_file}"
    diff ${expected_stdout_file} ${actual_stdout_file}
fi

exit -1