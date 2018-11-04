# game

find_library(Z_LIBRARY z)
if (Z_LIBRARY STREQUAL "Z_LIBRARY_LIBRARY-NOTFOUND")
  message(FATAL_ERROR "zlib library not founded")
endif()

configure_file("objects_config.hpp.cmake" "${CMAKE_BINARY_DIR}/objects_config.hpp")

set(GAME_LBR "my_game")
file(GLOB GAME_SRC "sources/*.cpp")
set(LIST_TARGETS "${LIST_TARGETS};${GAME_LBR}" CACHE STRING "add_target" FORCE)

add_library(${GAME_LBR} ${GAME_SRC})
target_link_libraries(${GAME_LBR} ${Z_LIBRARY})

target_include_directories(${GAME_LBR} PUBLIC "include")
target_include_directories(${GAME_LBR} PRIVATE "../engine/include")
target_include_directories(${GAME_LBR} PRIVATE "../math_2d/include")
target_include_directories(${GAME_LBR} PRIVATE "${CMAKE_BINARY_DIR}")
target_include_directories(${GAME_LBR} PRIVATE "../base64/include")
target_include_directories(${GAME_LBR} PRIVATE "../imgui/include")
