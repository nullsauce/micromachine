include(FetchContent)


# this requires a recent cmake
FetchContent_Declare(
	cppurses
	GIT_REPOSITORY https://github.com/flavioroth/CPPurses
	GIT_TAG "master"
)

FetchContent_GetProperties(cppurses)
if(NOT cppurses_POPULATED)
	FetchContent_Populate(cppurses)
	add_subdirectory(${cppurses_SOURCE_DIR}  ${cppurses_BINARY_DIR})
endif()
