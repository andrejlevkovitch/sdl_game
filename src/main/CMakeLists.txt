# main

cmake_minimum_required(VERSION "3.10")

project(sdl_game)

add_executable(${PROJECT_NAME} main.cpp)

target_compile_options(${PROJECT_NAME} PRIVATE "-std=c++11")

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE bombino engine)
