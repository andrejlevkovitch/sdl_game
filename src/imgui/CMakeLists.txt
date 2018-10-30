# imgui

set(IMGUI_LBR "imgui")
file(GLOB IMGUI_SRC "sources/*.cpp" "sources/*.c")
set(LIST_TARGETS "${LIST_TARGETS};${IMGUI_LBR}" CACHE STRING "add target" FORCE)

add_library(${IMGUI_LBR} ${IMGUI_SRC})

target_include_directories(${IMGUI_LBR} PUBLIC "include")
