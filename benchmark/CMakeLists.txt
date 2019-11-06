

include(benchmark)

find_package(Threads)

add_executable(benchmark_test test.cpp)
target_include_directories(benchmark_test PUBLIC ${EXTERNAL_INSTALL_LOCATION}/include)
target_link_libraries(benchmark_test libbenchmark_main libbenchmark ${CMAKE_THREAD_LIBS_INIT} micromachine)

