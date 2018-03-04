

include(ExternalProject)

set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

ExternalProject_Add(googletest
    GIT_REPOSITORY https://github.com/google/googletest
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
)

link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)


enable_testing()
find_package(Threads)


macro(declare_test test_name test_files)
    add_executable(${test_name} ${test_files})
    add_dependencies(${test_name} googletest)
    target_include_directories(${test_name} PUBLIC ${EXTERNAL_INSTALL_LOCATION}/include "${CMAKE_SOURCE_DIR}/lib")
    target_link_libraries(${test_name} gtest_main gtest ${CMAKE_THREAD_LIBS_INIT} capstone micromachine)
    add_test(all_tests ${test_name})
endmacro()

declare_test(bits_tests bits_tests.cpp)
declare_test(integer_tests integer_tests.cpp)
declare_test(alu_tests alu_tests.cpp)

add_subdirectory(pinksim)
