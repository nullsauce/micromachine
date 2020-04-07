

set(GTEST_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external/googletest)

message(STATUS "Will install googletest in ${GTEST_INSTALL_LOCATION}")

set(GTEST_INCLUDE_PATH "${GTEST_INSTALL_LOCATION}/include")
set(GTEST_LIB_PATH "${GTEST_INSTALL_LOCATION}/lib")

find_library(TRY_FIND_GTEST
	NAMES libgtest.a
	PATHS ${GTEST_LIB_PATH}
	DOC "Find gtest static library"
	NO_DEFAULT_PATH
)

if(NOT TRY_FIND_GTEST)
	message(WARNING "GTest library not found. Will download and install in ${GTEST_INSTALL_LOCATION}")
	ExternalProject_Add(compile_googletest
		GIT_REPOSITORY https://github.com/google/googletest
		GIT_TAG release-1.8.1
		CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${GTEST_INSTALL_LOCATION}
	)
endif()




# this directory has to exist for INTERFACE_INCLUDE_DIRECTORIES to work
file(MAKE_DIRECTORY ${GTEST_INCLUDE_PATH})

# defines the libgtest library
add_library(libgtest IMPORTED STATIC GLOBAL)
add_dependencies(libgtest compile_googletest)
set_target_properties(libgtest PROPERTIES
    "IMPORTED_LOCATION" ${GTEST_LIB_PATH}/libgtest.a
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
    "INTERFACE_INCLUDE_DIRECTORIES" ${GTEST_INCLUDE_PATH}
)

# defines the libgtest_main library
add_library(libgtest_main IMPORTED STATIC GLOBAL)
add_dependencies(libgtest_main compile_googletest)
set_target_properties(libgtest_main PROPERTIES
    "IMPORTED_LOCATION" ${GTEST_LIB_PATH}/libgtest_main.a
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
    "INTERFACE_INCLUDE_DIRECTORIES" ${GTEST_INCLUDE_PATH}
)
