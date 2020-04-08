

set(BENCHMARK_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external/benchmark)

message(STATUS "Will install benchmark in ${BENCHMARK_INSTALL_LOCATION}")

set(BENCHMARK_INCLUDE_PATH "${BENCHMARK_INSTALL_LOCATION}/include")
set(BENCHMARK_LIB_PATH "${BENCHMARK_INSTALL_LOCATION}/lib")

find_library(TRY_FIND_BENCHMARK
	NAMES libbenchmark.a
	PATHS ${BENCHMARK_LIB_PATH}
	DOC "Find benchmark static library"
	NO_DEFAULT_PATH
)

if(NOT TRY_FIND_BENCHMARK)
	message(STATUS "Benchmark library not found. Will download and install in ${BENCHMARK_INSTALL_LOCATION}")
	ExternalProject_Add(compile_benchmark
		GIT_REPOSITORY https://github.com/google/benchmark
		CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${BENCHMARK_INSTALL_LOCATION} -DBENCHMARK_ENABLE_TESTING=FALSE
	)
endif()




# this directory has to exist for INTERFACE_INCLUDE_DIRECTORIES to work
file(MAKE_DIRECTORY ${BENCHMARK_INCLUDE_PATH})

# defines the libbenchmark library
add_library(libbenchmark IMPORTED STATIC GLOBAL)
add_dependencies(libbenchmark compile_benchmark)
set_target_properties(libbenchmark PROPERTIES
    "IMPORTED_LOCATION" ${BENCHMARK_LIB_PATH}/libbenchmark.a
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
    "INTERFACE_INCLUDE_DIRECTORIES" ${BENCHMARK_INCLUDE_PATH}
)


# defines the libbenchmark_main library
add_library(libbenchmark_main IMPORTED STATIC GLOBAL)
add_dependencies(libbenchmark_main compile_benchmark)
set_target_properties(libbenchmark_main PROPERTIES
    "IMPORTED_LOCATION" ${BENCHMARK_LIB_PATH}/libbenchmark_main.a
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
    "INTERFACE_INCLUDE_DIRECTORIES" ${BENCHMARK_INCLUDE_PATH}
)
