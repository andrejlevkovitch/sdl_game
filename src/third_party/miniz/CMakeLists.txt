# cmake for miniz

cmake_minimum_required(VERSION 3.10)

project(miniz)

set(miniz_SOURCE miniz.c miniz_zip.c miniz_tinfl.c miniz_tdef.c)

add_library(miniz ${miniz_SOURCE})
target_include_directories(miniz PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
