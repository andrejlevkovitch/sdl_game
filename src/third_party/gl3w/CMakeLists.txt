# cmake for gl3w

cmake_minimum_required(VERSION "3.10")

project(gl3w)

add_library(gl3w gl3w.c)
target_include_directories(gl3w PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

# here we have problem with static linking on linux
if(NOT BUILD_SHARED_LIBS AND NOT CMAKE_CXX_COMPILER MATCHES "mingw" AND UNIX)
  target_link_libraries(gl3w PUBLIC -ldl)
endif()
