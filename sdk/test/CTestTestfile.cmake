# CMake generated Testfile for 
# Source directory: /home/roth/code/perso/micromachine/sdk/test
# Build directory: /home/roth/code/perso/micromachine/sdk/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(md5_test "/home/roth/code/perso/micromachine/sdk/test/test_caller.sh" "/home/roth/code/perso/micromachine/sdk/examples/md5sum/expected_stdout.txt" "/home/roth/code/perso/micromachine/cmake-build-debug/run/run_micromachine" "/home/roth/code/perso/micromachine/sdk/examples/md5sum/md5sum")
add_test(timer_test "/home/roth/code/perso/micromachine/sdk/test/test_caller.sh" "/home/roth/code/perso/micromachine/sdk/examples/timer_test/expected_stdout.txt" "/home/roth/code/perso/micromachine/cmake-build-debug/run/run_micromachine" "/home/roth/code/perso/micromachine/sdk/examples/timer_test/timer_test")
