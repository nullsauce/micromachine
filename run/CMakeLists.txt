set(SOURCE_FILES
	main.cpp
)
add_executable(run_micromachine ${SOURCE_FILES})
target_include_directories(run_micromachine PUBLIC ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(run_micromachine micromachine)


