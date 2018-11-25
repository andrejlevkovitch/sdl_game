# cmake for bombino

cmake_minimum_required(VERSION "3.10")

project(bombino)

configure_file("objects_config.hpp.cmake" "${CMAKE_BINARY_DIR}/objects_config.hpp")

set(GAME_LBR "my_game")
set(GAME_SRC
  "sources/bomb.cpp"
  "sources/callback_map.cpp"
  "sources/flame.cpp"
  "sources/gamer.cpp"
  "sources/menu_state.cpp"
  "sources/object_factory.cpp"
  "sources/object_manager.cpp"
  "sources/over_state.cpp"
  "sources/pause_state.cpp"
  "sources/playing_state.cpp"
  "sources/power.cpp"
  "sources/tile.cpp"
  "sources/tile_loader.cpp"
 )

add_library(${PROJECT_NAME} ${GAME_SRC})

add_dependencies(${PROJECT_NAME} engine)

target_compile_options(${PROJECT_NAME} PUBLIC "-std=c++11")

target_include_directories(${PROJECT_NAME} PUBLIC "include")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_BINARY_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "$<TARGET_PROPERTY:engine,INCLUDE_DIRECTORIES>")

target_link_libraries(${PROJECT_NAME} PUBLIC imgui base64 miniz tinyxml2 math2d engine)
