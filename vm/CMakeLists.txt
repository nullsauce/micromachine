include(cxxopts)

set(SOURCE_FILES
	main.cpp
)


find_package(Threads)

add_executable(micromachine_vm ${SOURCE_FILES})
set_target_properties(micromachine_vm PROPERTIES OUTPUT_NAME "micromachine")
target_link_libraries(micromachine_vm cxxopts micromachine ${CMAKE_THREAD_LIBS_INIT})
