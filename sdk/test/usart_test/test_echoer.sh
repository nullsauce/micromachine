#!/usr/bin/env bash

# usage ./test_caller.sh <stdout-file> <command>
# will return zero if the stdout generated by executing <command> is strictly equal
# to the content of <stdout-file>

# Example: ./test_echoer.sh expected_stdout.txt ../../../cmake-build-release/vm/micromachine md5sum

set -e

actual_stdout_file="$(mktemp)"
expected_stdout_file="$1"
command_to_run="${@:2}"

echo "running test: $command_to_run"
${command_to_run} &
vm_pid=$!

# The application will terminate when sizeof("Welcome micromachinist!") is received
sleep 0.25
socat FILE:${expected_stdout_file},ignoreeof\!\!OPEN:${actual_stdout_file},creat,trunc "UNIX-CLIENT:/tmp/micromachine/${vm_pid}/usart0"
sleep 0.25

if cmp --silent ${expected_stdout_file} ${actual_stdout_file}; then
    exit 0
else
    echo "Test failed:"
    echo "Expected stdout file: ${expected_stdout_file}"
    echo "--BEGIN-FILE-CONTENT--"
    cat ${expected_stdout_file}
    echo "--END-FILE-CONTENT--"
    echo "Actual stdout file: ${actual_stdout_file}"
    echo "--BEGIN-FILE-CONTENT--"
    cat ${actual_stdout_file}
    echo "--END-FILE-CONTENT--"
    echo "diff:"
    diff ${expected_stdout_file} ${actual_stdout_file}
fi

exit -1
