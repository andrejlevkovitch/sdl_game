# engine

cmake_minimum_required(VERSION "3.10")

project(engine)

if(CMAKE_CXX_COMPILER MATCHES "mingw" AND UNIX)
  set(SDL2_LIBRARY "/usr/i686-w64-mingw32/bin/SDL2.dll")
  set(OPENGL_gl_LIBRARY "/usr/i686-w64-mingw32/lib/libopengl32.a")
elseif(UNIX)
  find_package(SDL2 REQUIRED)
  find_package(OpenGL REQUIRED)
endif()

configure_file("shaders_config.hpp.cmake" "${CMAKE_BINARY_DIR}/shaders_config.hpp")

set(ENGINE_SRC
  "sources/abstract_object.cpp"
  "sources/button.cpp"
  "sources/engine.cpp"
  "sources/event.cpp"
  "sources/gl_loader.cpp"
  "sources/input_handler.cpp"
  "sources/light.cpp"
  "sources/menu_imgui.cpp"
  "sources/player.cpp"
  "sources/scene.cpp"
  "sources/state_machine.cpp"
  "sources/texture.cpp"
  "sources/texture_manager.cpp"
  "sources/time.cpp"
  )

add_library(${PROJECT_NAME} ${ENGINE_SRC})

# because in lyambda I use auto
target_compile_options(${PROJECT_NAME} PUBLIC "-std=c++14")

target_include_directories(${PROJECT_NAME} PUBLIC "include")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_BINARY_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "../third_party")

target_link_libraries(${PROJECT_NAME} PUBLIC ${SDL2_LIBRARY} ${OPENGL_gl_LIBRARY})
target_link_libraries(${PROJECT_NAME} PUBLIC imgui math2d tinyxml2)
