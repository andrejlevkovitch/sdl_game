#cmake

cmake_minimum_required(VERSION 3.5)

project(sdl_game VERSION "0.0.1")

include(build.cmake)
include(doxygen.cmake)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR};${CMAKE_MODULE_PATH}")

set(LIST_SUBDIRS third_party bombino engine math_2d main)
message(STATUS "SUBDIRECTORIES: ${LIST_SUBDIRS}")
foreach(DIR ${LIST_SUBDIRS})
  add_subdirectory("src/${DIR}")
endforeach()

configure_file("menu_config.hpp.cmake" "${CMAKE_CURRENT_BINARY_DIR}/menu_config.hpp")
