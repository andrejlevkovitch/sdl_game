# cmake for gl3w

cmake_minimum_required(VERSION "3.10")

project(gl3w)

add_library(gl3w gl3w.c)
target_include_directories(gl3w PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
