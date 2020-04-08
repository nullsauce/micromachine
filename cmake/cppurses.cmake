include(FetchContent)


# this requires a recent cmake
FetchContent_Declare(
	cppurses
	GIT_REPOSITORY https://github.com/a-n-t-h-o-n-y/CPPurses
	GIT_TAG "791dfe5eb696d2729c80f7c45d18620bcaa78b5f"
)

FetchContent_GetProperties(cppurses)
if(NOT cppurses_POPULATED)
	FetchContent_Populate(cppurses)
	add_subdirectory(${cppurses_SOURCE_DIR}  ${cppurses_BINARY_DIR})
endif()
