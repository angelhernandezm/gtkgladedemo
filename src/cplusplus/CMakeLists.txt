cmake_minimum_required(VERSION 3.5)
project(cpluscalc)

set(CMAKE_CXX_STANDARD 17)

# Use pkg-config to find gtkmm-3.0
include(FindPkgConfig)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

# Include GTKMM headers
include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})
add_definitions(${GTKMM_CFLAGS_OTHER})

# Define source files
set(SOURCE_FILES
        src/MainWindow.cpp
        src/MainWindowController.cpp
        src/MainWindowController.h
)

# Compile GResource file
find_program(GLIB_COMPILE_RESOURCES glib-compile-resources REQUIRED)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/resources.c
        COMMAND ${GLIB_COMPILE_RESOURCES}
        --target=${CMAKE_CURRENT_BINARY_DIR}/resources.c
        --generate-source
        --sourcedir=${CMAKE_CURRENT_SOURCE_DIR}/ui
        ${CMAKE_CURRENT_SOURCE_DIR}/ui/resources.gresource.xml
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/ui/resources.gresource.xml
)

add_custom_target(resources DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/resources.c)

# Add executable and link GTKMM
add_executable(cpluscalc ${SOURCE_FILES} ${CMAKE_CURRENT_BINARY_DIR}/resources.c)
add_dependencies(cpluscalc resources)

target_include_directories(cpluscalc PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(cpluscalc ${GTKMM_LIBRARIES})