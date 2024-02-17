
if(DEFINED ENV{VCPKG_ROOT})
    if(WIN32)
        set(VCPKG_TARGET_TRIPLET "x64-windows-static-md")
    elseif(APPLE)
        set(VCPKG_INSTALL_OPTIONS "--allow-unsupported")
    endif()
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
else()
    message(FATAL_ERROR "NOT FIND ENVIRONMENT VARIABLE VCPKG_ROOT")
endif()

cmake_minimum_required(VERSION 3.22)

project(DesktopApp VERSION 0.1 LANGUAGES CXX)
set(APPLICATION_NAME "DesktopApp" CACHE STRING "APPLICATION_NAME" FORCE)

if(WIN32)
    add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)
endif(WIN32)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "ON" FORCE)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMakeTargets")

file(GLOB_RECURSE ${APPLICATION_NAME}_HEADERS src/*.h src/*.hpp)
file(GLOB_RECURSE ${APPLICATION_NAME}_SOURCES src/*.c src/*.cpp)

find_package(Qt6Core CONFIG REQUIRED)
qt_standard_project_setup()
qt_add_executable(${APPLICATION_NAME} ${${APPLICATION_NAME}_HEADERS} ${${APPLICATION_NAME}_SOURCES})
target_link_libraries(${APPLICATION_NAME} PUBLIC Qt::Core Qt6::Core Qt::CorePrivate Qt6::CorePrivate)

find_package(Qt6Gui CONFIG REQUIRED)
target_link_libraries(${APPLICATION_NAME} PUBLIC Qt::Gui Qt6::Gui Qt::GuiPrivate Qt6::GuiPrivate)
 
find_package(Qt6Widgets CONFIG REQUIRED)
target_link_libraries(${APPLICATION_NAME} PUBLIC Qt::Widgets Qt6::Widgets Qt::WidgetsPrivate Qt6::WidgetsPrivate)

find_package(Qt6Qml CONFIG REQUIRED)
target_link_libraries(${APPLICATION_NAME} PRIVATE Qt::Qml Qt6::Qml Qt::QmlPrivate Qt6::QmlPrivate)

find_package(Qt6Quick CONFIG REQUIRED)
target_link_libraries(${APPLICATION_NAME} PRIVATE Qt::Quick Qt6::Quick Qt::QuickPrivate Qt6::QuickPrivate)

find_package(Qt6QuickControls2 CONFIG REQUIRED)
target_link_libraries(${APPLICATION_NAME} PRIVATE Qt::QuickControls2 Qt6::QuickControls2 Qt::QuickControls2Private Qt6::QuickControls2Private)

set_target_properties(${APPLICATION_NAME} PROPERTIES
    WIN32_EXECUTABLE $<IF:$<CONFIG:Debug>,OFF,ON>
    MACOSX_BUNDLE ON
)

file(GLOB_RECURSE ${APPLICATION_NAME}_QML_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" qml/*)
file(GLOB_RECURSE ${APPLICATION_NAME}_IMG_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" img/*)

qt_add_resources(${APPLICATION_NAME} QRC
    PREFIX "/"
    FILES ${${APPLICATION_NAME}_QML_FILES} ${${APPLICATION_NAME}_IMG_FILES}
)

# git version
cmake_policy(SET CMP0135 NEW)
include(FetchContent)
FetchContent_Declare(GitInfo
    URL https://github.com/huiseliming/GitInfo/archive/refs/tags/v1.0.0.zip
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext/GitInfo
)
FetchContent_MakeAvailable(GitInfo)
target_link_libraries(${APPLICATION_NAME} PRIVATE GitInfo)
