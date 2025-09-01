cmake_minimum_required(VERSION 3.19)
project(QWlanAppRoot LANGUAGES CXX)

# Set C++ standard to use
set(CMAKE_CXX_STANDARD 20)

# Export generated application
if(NOT PROJECT_BUILD_OUTPUT)
    set(PROJECT_BUILD_OUTPUT ${CMAKE_SOURCE_DIR}/build/output/${PROJECT_ARCH_TARGET}/${CMAKE_BUILD_TYPE})

    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BUILD_OUTPUT}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BUILD_OUTPUT}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BUILD_OUTPUT}/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BUILD_OUTPUT}/bin)
endif()

## Manage subdirectories
## Set options
set(QWLANMANAGER_WINDOWS_COMPAT_PREWIN10 OFF)

## Add subdirectories
add_subdirectory(src)
#add_subdirectory(deps/qlogger)
#add_subdirectory(deps/qwlanmanager)
#add_subdirectory(deps/toolboxqt)
