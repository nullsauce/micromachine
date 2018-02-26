cmake_minimum_required(VERSION 3.0.2)
project(micromachine_emu)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

find_package(Threads)


add_subdirectory(lib)

set(SOURCE_FILES
    main.cpp
)
add_executable(run_micromachine ${HEADER_FILES} ${SOURCE_FILES})
target_include_directories(run_micromachine PUBLIC ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(run_micromachine micromachine)


add_subdirectory(debugger)
add_subdirectory(test)
