# imgui

cmake_minimum_required(VERSION "3.10")

set(IMGUI_SRC
  "sources/gl3w.c"
  "sources/imgui.cpp"
  "sources/imgui_demo.cpp"
  "sources/imgui_draw.cpp"
  "sources/imgui_impl_opengl3.cpp"
  "sources/imgui_impl_sdl.cpp"
  "sources/imgui_widgets.cpp"
  )

add_library(imgui SHARED ${IMGUI_SRC})

target_include_directories(imgui PUBLIC "include")
