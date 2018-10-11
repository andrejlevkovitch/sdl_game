# main

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

set(MAIN_SRC "main.cpp")

add_executable(${PROJECT_NAME} ${MAIN_SRC})

foreach(SOME_TARGET ${LIST_TARGETS})
  target_link_libraries(${PROJECT_NAME} ${SOME_TARGET})
endforeach()

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARY} ${SDL2_LIBRARY})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR})
