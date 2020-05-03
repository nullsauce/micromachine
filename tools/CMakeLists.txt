include(cxxopts)

set(SOURCE_FILES
	instruction-dispatch-dump.cpp
)

add_executable(instruction-dispatch-dump ${SOURCE_FILES})
target_link_libraries(instruction-dispatch-dump micromachine)
