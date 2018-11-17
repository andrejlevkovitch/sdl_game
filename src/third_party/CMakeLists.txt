# cmake for third_party

cmake_minimum_required(VERSION "3.10")

project(third_party)

add_subdirectory(imgui)

add_library(base64 "base64/base64.cpp")
target_compile_options(base64 PRIVATE "-std=c++11")
target_include_directories(base64 PUBLIC "base64")

add_library(tinyxml2 "tinyxml2/tinyxml2.cpp")
target_compile_options(tinyxml2 PRIVATE "-std=c++11")
target_include_directories(tinyxml2 PUBLIC "tinyxml2")
