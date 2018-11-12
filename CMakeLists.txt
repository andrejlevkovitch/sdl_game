#cmake

cmake_minimum_required(VERSION 3.5)

project(sdl_game VERSION "0.0.1")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")

set(LIST_TARGETS CACHE STRING "list of targets" FORCE)
set(LIST_SUBDIRS bombino base64 engine math_2d imgui main)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR};${CMAKE_MODULE_PATH}")

configure_file("menu_config.hpp.cmake" "${CMAKE_CURRENT_BINARY_DIR}/menu_config.hpp")

include(build.cmake)
include(doxygen.cmake)

message(STATUS "SUBDIRECTORIES: ${LIST_SUBDIRS}")
message(STATUS "TARGETS: ${LIST_TARGETS}")
