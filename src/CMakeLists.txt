# CMakeLists file based on project template repository: https://github.com/legerch/projects-template
## Template file version: 2025-08-20

cmake_minimum_required(VERSION 3.20)

# Set project properties
set(PRJ_COMPANY_ID          legerch)
set(PRJ_COMPANY_NAME        "Leger Charlie")
set(PRJ_ID                  qwlanmanagerapp)
set(PRJ_READABLE_NAME       "QWlan App")
set(PRJ_VERSION_SEMANTIC    0.0.1)
set(PRJ_DESCRIPTION         "Allow to perform wlan operations.")
set(PRJ_LICENSE             "MIT")
set(PRJ_COPYRIGHT           "Copyright (c) 2025 ${PRJ_COMPANY_NAME} (${PRJ_LICENSE} License)")
set(PRJ_HOMEPAGE_URL        "https://github.com/legerch/QWlanManagerApp")
set(PRJ_ICON_NAME           "wifi-router")

set(PROJECT_NAME ${PRJ_ID})
set(PROJECT_VERSION_CPP_MIN 17)

# Set project configuration
## Generic properties
set(CMAKE_INCLUDE_CURRENT_DIR ON)
project(${PROJECT_NAME}
    LANGUAGES       CXX
    VERSION         "${PRJ_VERSION_SEMANTIC}"
    DESCRIPTION     ${PRJ_DESCRIPTION}
    HOMEPAGE_URL    ${PRJ_HOMEPAGE_URL}
)

## Qt specific properties
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Define project options
# Example: option(APPNAME_BUILD_TESTS "Use to enable/disable build of unit-tests." ON)
option(APPNAME_USE_QML "Enable to use QML instead of Widgets." OFF)

# Manage global compiler options
## For MSVC: force to read source code as UTF-8 file (already default behaviour on GCC/Clang)
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# Set C++ standard to use
if(DEFINED CMAKE_CXX_STANDARD)
    if(${CMAKE_CXX_STANDARD} LESS ${PROJECT_VERSION_CPP_MIN})
        message(FATAL_ERROR "Project ${PROJECT_NAME} require at least C++ standard ${PROJECT_VERSION_CPP_MIN}")
    endif()
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message(STATUS "Project \"${PROJECT_NAME}\" compiled with C++ standard ${CMAKE_CXX_STANDARD}")

# Set needed packages
## Qt packages
set(PROJECT_QT_PACKAGES
    Core
    Widgets
)

find_package(QT NAMES Qt6 Qt5 COMPONENTS ${PROJECT_QT_PACKAGES} REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS ${PROJECT_QT_PACKAGES} REQUIRED)

list(TRANSFORM PROJECT_QT_PACKAGES PREPEND "Qt${QT_VERSION_MAJOR}::")

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_standard_project_setup(REQUIRES 6.8)
endif()

## Other library packages
### Example: find_package(nlohmann_json 3.11.3 REQUIRED)

# Defines useful path variables for easier CMake configuration
set(PROJECT_DIR_ROOT                    "${PROJECT_SOURCE_DIR}")
set(PROJECT_DIR_BASE                    "${PROJECT_DIR_ROOT}/base")
set(PROJECT_DIR_META                    "${PROJECT_DIR_ROOT}/metadata")
set(PROJECT_DIR_LOGO                    "${PROJECT_DIR_ROOT}/assets/logo")

set(PROJECT_FILE_GLOBAL                 "${PROJECT_DIR_BASE}/qwlanapp_global.h")

set(PROJECT_FILE_OS_METADATA_WINDOWS    "${PROJECT_DIR_META}/windows/infos.rc")
set(PROJECT_FILE_OS_METADATA_MACOS      "${PROJECT_DIR_META}/macos/Info.plist")

set(PRJ_ICON                            "${PROJECT_DIR_LOGO}/${PRJ_ICON_NAME}")

# Manage application files
## Configure file project - File containing macros that can be used in project
configure_file("${PROJECT_FILE_OS_METADATA_WINDOWS}.in" "${PROJECT_FILE_OS_METADATA_WINDOWS}")
configure_file("${PROJECT_FILE_OS_METADATA_MACOS}.in" "${PROJECT_FILE_OS_METADATA_MACOS}")

configure_file("${PROJECT_FILE_GLOBAL}.in" "${PROJECT_FILE_GLOBAL}")

## Set application compilation definitions
set(PROJECT_DEFS_PRIV
    "QT_MESSAGELOGCONTEXT"
)

set(PROJECT_DEFS_PUB

)

## Set library linking
### Linking dirs
set(PROJECT_LINKING_DIR

)

### Linking libs
set(PROJECT_LINKING_LIB
    ${PROJECT_QT_PACKAGES}

    qlogger
    qwlanmanager
    toolboxqt
)

## Set application files
set(PROJECT_HEADERS
    ui/widgets/mainwindow.h
    ui/widgets/menu/menubar.h
)

set(PROJECT_SOURCES
    main.cpp
    
    ui/widgets/mainwindow.cpp
    ui/widgets/menu/menubar.cpp
)

set(PROJECT_UI_WIDGETS
    ui/widgets/mainwindow.ui
)

set(PROJECT_RESSOURCES
    ressources.qrc
)

set(PROJECT_UI_QML

)

### Manage QML files according to Qt version
if(APPNAME_USE_QML)
    set(PROJECT_DEFS_PRIV ${PROJECT_DEFS_PRIV}
        APPNAME_USE_QML
    )

    if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
        list(TRANSFORM PROJECT_UI_QML PREPEND "ui/qml/qt6/")
    else()
        set(PROJECT_RESSOURCES ${PROJECT_RESSOURCES} ui/qml/qt5/qml.qrc)
    endif()
endif()

# Manage platform differences
## Refer to: https://cmake.org/cmake/help/latest/variable/CMAKE_SYSTEM_NAME.html
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(PROJECT_DEFS_PUB ${PROJECT_DEFS_PUB}
        "NOMINMAX"  # Needed because Windows define macros "min/max", better to not defined those and use "std::min/std::max" instead
        "WIN32_LEAN_AND_MEAN=1"
    )

    set(PROJECT_RESSOURCES ${PROJECT_RESSOURCES}
        ${PROJECT_FILE_OS_METADATA_WINDOWS}
    )
endif()

# Manage options
## Example: if(APPNAME_USE_CUSTOM_BEHAVIOUR)

# Add files to the application
set(PROJECT_FILES ${PROJECT_FILE_GLOBAL} ${PROJECT_HEADERS} ${PROJECT_SOURCES} ${PROJECT_UI_WIDGETS} ${PROJECT_RESSOURCES})

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME} ${PROJECT_FILES})

    if(APPNAME_USE_QML)
        qt_add_qml_module(${PROJECT_NAME}
            URI         ${PROJECT_NAME}
            VERSION     1.0
            QML_FILES   ${PROJECT_UI_QML}
        )
    endif()
    
else()
    add_executable(${PROJECT_NAME} ${PROJECT_FILES})
endif()

# Link needed libraries
target_link_directories(${PROJECT_NAME} PUBLIC ${PROJECT_LINKING_DIR})
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${PROJECT_LINKING_LIB})

# Specify compiler options
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Werror=return-type -Wshadow -Wlogical-op -Wduplicated-cond -Wduplicated-branches>
    $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Werror=return-type -Wshadow -Wlogical-op -Wduplicated-cond -Wduplicated-branches>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /wd4251>
)

# Compile needed definitions
## Application related
target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_DEFS_PUB})
target_compile_definitions(${PROJECT_NAME} PRIVATE ${PROJECT_DEFS_PRIV})

# Set properties of application
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION                     ${PROJECT_VERSION}
    SOVERSION                   ${PROJECT_VERSION_MAJOR}
    MACOSX_BUNDLE               TRUE
    MACOSX_BUNDLE_INFO_PLIST    ${PROJECT_FILE_OS_METADATA_MACOS}
    WIN32_EXECUTABLE            TRUE
)
