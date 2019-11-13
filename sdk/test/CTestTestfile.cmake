# CMake generated Testfile for 
# Source directory: /home/fla/projects/micromachine/sdk/test
# Build directory: /home/fla/projects/micromachine/sdk/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(md5_test "/home/fla/projects/micromachine/sdk/test/test_caller.sh" "/home/fla/projects/micromachine/sdk/examples/md5sum/expected_stdout.txt" "/home/fla/projects/micromachine/cmake-build-debug/run/run_micromachine" "/home/fla/projects/micromachine/sdk/examples/md5sum/md5sum")
add_test(timer_test "/home/fla/projects/micromachine/sdk/test/test_caller.sh" "/home/fla/projects/micromachine/sdk/examples/timer_test/expected_stdout.txt" "/home/fla/projects/micromachine/cmake-build-debug/run/run_micromachine" "/home/fla/projects/micromachine/sdk/examples/timer_test/timer_test")
