

set(SOURCE_FILES
	main.cpp
)
# CPPurses is cloned into a directory named external/
add_subdirectory(${CMAKE_SOURCE_DIR}/external/cppurses ${CMAKE_BINARY_DIR}/external/cppurses)
add_executable(termdbg ${SOURCE_FILES})
target_include_directories(termdbg PUBLIC ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(termdbg micromachine cppurses)
