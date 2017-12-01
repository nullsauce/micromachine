cmake_minimum_required(VERSION 3.0.2)
project(micromachine)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

find_package(Threads)


add_subdirectory(lib)

set(SOURCE_FILES
    main.cpp
)
add_executable(micromachine ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(micromachine micro)


add_subdirectory(debugger)
add_subdirectory(test)
