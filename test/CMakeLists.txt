
include(gtest)

enable_testing()
find_package(Threads)


macro(declare_test test_name test_files)
    add_executable(${test_name} ${test_files})
    target_include_directories(${test_name} PUBLIC "${CMAKE_SOURCE_DIR}/lib")
    target_link_libraries(${test_name} libgtest libgtest_main ${CMAKE_THREAD_LIBS_INIT} micromachine)
    add_test(all_tests ${test_name})
endmacro()

declare_test(bits_tests bits_tests.cpp)
declare_test(integer_tests integer_tests.cpp)
declare_test(alu_tests alu_tests.cpp)
declare_test(bitslice_tests bitslice_tests.cpp)
add_subdirectory(pinksim)
