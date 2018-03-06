
set(QT_RUNTIME_ROOT "$ENV{HOME}/Qt5/5.9.1/gcc_64")

if(DEFINED QT_QMAKE_EXECUTABLE)
	message(STATUS "Building from Qt Creator. Using automatic Qt version.")
else()
	message(STATUS "Building outside Qt Creator. Using manual Qt install path.")
	set(Qt5_DIR "${QT_RUNTIME_ROOT}/lib/cmake/Qt5")
	if(NOT IS_DIRECTORY "${Qt5_DIR}")
		message(FATAL_ERROR "Qt5_DIR not found. (Qt5_DIR=${Qt5_DIR})")
	endif()
endif()

message(STATUS "Using Qt5 located at ${Qt5_DIR}")

