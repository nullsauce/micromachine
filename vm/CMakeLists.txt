include(cxxopts)

set(SOURCE_FILES
	main.cpp
)

add_executable(micromachine_vm ${SOURCE_FILES})
set_target_properties(micromachine_vm PROPERTIES OUTPUT_NAME "micromachine")
target_include_directories(micromachine_vm PUBLIC ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(micromachine_vm cxxopts micromachine)
