# imgui

cmake_minimum_required(VERSION "3.10")

set(IMGUI_SRC
  "sources/imgui.cpp"
  "sources/imgui_demo.cpp"
  "sources/imgui_draw.cpp"
  "sources/imgui_impl_opengl3.cpp"
  "sources/imgui_impl_sdl.cpp"
  "sources/imgui_widgets.cpp"
  )

add_library(imgui ${IMGUI_SRC})
target_compile_options(imgui PRIVATE "-std=c++11")
target_include_directories(imgui PUBLIC "include")
target_link_libraries(imgui PRIVATE gl3w)
