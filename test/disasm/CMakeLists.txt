
include_directories(framework)
add_definitions(-DSOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
declare_test(disassembler disassembler.cpp)
add_test(disassembler disassembler)