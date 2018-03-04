
set(QT_RUNTIME_ROOT "$ENV{HOME}/Qt/5.9.2/gcc_64")

if(NOT IS_DIRECTORY "${Qt5_DIR}")
    message(STATUS "Qt5_DIR not defined. Falling back to manual value.")
    if(NOT ENV{Qt5_DIR})
            set(Qt5_DIR "${QT_RUNTIME_ROOT}/lib/cmake/Qt5")
    endif()
else()
    message(STATUS "Using preset value for Qt5_DIR")
endif()

if(NOT IS_DIRECTORY ${Qt5_DIR})
    message(FATAL_ERROR "Qt5_DIR is not a directory. (Qt5_DIR=${Qt5_DIR})")
endif()

message(STATUS "Using Qt5 located at ${Qt5_DIR}")

