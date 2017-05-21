
enable_testing()
find_package(GTest REQUIRED)
find_package(Threads)
include_directories(${GTEST_INCLUDE_DIRS})

macro(declare_test test_name test_files)
    message(STATUS "declare_test ${test_name} ${test_files}")
    add_executable(${test_name} ${test_files})
    target_link_libraries(${test_name} ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
    add_test(all_tests ${test_name})
endmacro()

declare_test(integer_tests integer_tests.cpp)
declare_test(alu_tests alu_tests.cpp)
declare_test(cpu_tests cpu_tests.cpp)

add_subdirectory(pinksim)
