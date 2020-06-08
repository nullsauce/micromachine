set(ELFIO_DIR "${CMAKE_BINARY_DIR}/external/elfio")
set(ELFIO_SOURCE_DIR "${ELFIO_DIR}/src")
set(ELFIO_INSTALL_LOCATION ${ELFIO_DIR}/install)
set(ELFIO_INCLUDE_PATH "${ELFIO_INSTALL_LOCATION}/include")

find_path(ELFIO_INCLUDE_DIR "elfio.hpp" "${ELFIO_INCLUDE_PATH}/elfio")
if (ELFIO_INCLUDE_DIR STREQUAL "ELFIO_INCLUDE_DIR-NOTFOUND")
	message(STATUS "ELFIO library not found. Will download and install in ${ELFIO_INSTALL_LOCATION}")
	ExternalProject_Add(compile_elfio
		GIT_REPOSITORY https://github.com/serge1/ELFIO
		GIT_TAG master
		SOURCE_DIR ${ELFIO_SOURCE_DIR}
		BUILD_IN_SOURCE 1
		CONFIGURE_COMMAND ${ELFIO_SOURCE_DIR}/autogen.sh
				  COMMAND ${ELFIO_SOURCE_DIR}/configure --prefix=${ELFIO_INSTALL_LOCATION}
		BUILD_COMMAND make -j $(nproc)
		INSTALL_COMMAND make install
	)
else()
	message(STATUS "ELFIO library found at ${ELFIO_INCLUDE_DIR}")
endif()

if(NOT TARGET elfio)
	add_library(elfio IMPORTED INTERFACE GLOBAL)
	add_dependencies(elfio compile_elfio)
	set_target_properties(elfio PROPERTIES
		INTERFACE_INCLUDE_DIRECTORIES ${ELFIO_INCLUDE_PATH}
	)
endif()


