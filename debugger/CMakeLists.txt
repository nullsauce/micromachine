cmake_minimum_required(VERSION 2.8.12)



set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)

add_executable(visual_debugger "main.cpp" "qml.qrc")
target_include_directories(visual_debugger PUBLIC "../lib")
target_link_libraries(visual_debugger
	Qt5::Quick
	Qt5::Core
	Qt5::Qml
	Qt5::Gui
	libmicro
)
