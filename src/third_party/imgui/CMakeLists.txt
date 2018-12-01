# imgui

cmake_minimum_required(VERSION "3.10")

project(imgui)

set(IMGUI_SRC
  "sources/imgui.cpp"
  "sources/imgui_demo.cpp"
  "sources/imgui_draw.cpp"
  "sources/imgui_impl_opengl3.cpp"
  "sources/imgui_impl_sdl.cpp"
  "sources/imgui_widgets.cpp"
  )

if(CMAKE_CXX_COMPILER MATCHES "mingw" AND UNIX)
  set(SDL2_LIBRARY "/usr/i686-w64-mingw32/bin/SDL2.dll")
  set(OPENGL_gl_LIBRARY "/usr/i686-w64-mingw32/lib/libopengl32.a")
else()
  find_package(OpenGL REQUIRED)
  find_package(SDL2 REQUIRED)
endif()

add_library(imgui ${IMGUI_SRC})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_11)
target_include_directories(imgui PUBLIC "include")
target_link_libraries(imgui PRIVATE gl3w)
target_link_libraries(imgui PRIVATE ${SDL2_LIBRARY} ${OPENGL_gl_LIBRARY})
