# game

find_library(TinyXML_LIBRARY tinyxml)
if (TinyXML_LIBRARY STREQUAL "TinyXML_LIBRARY-NOTFOUND")
  message(FATAL_ERROR "tinyxml library not founded")
endif()

set(GAME_LBR "my_game")
file(GLOB GAME_SRC "sources/*.cpp")
set(LIST_TARGETS "${LIST_TARGETS};${GAME_LBR}" CACHE STRING "add_target" FORCE)

add_library(${GAME_LBR} ${GAME_SRC})
target_link_libraries(${GAME_LBR} ${TinyXML_LIBRARY})

target_include_directories(${GAME_LBR} PUBLIC "include")
target_include_directories(${GAME_LBR} PRIVATE "../engine/include")
target_include_directories(${GAME_LBR} PRIVATE "../math_2d/include")
target_include_directories(${GAME_LBR} PRIVATE "${CMAKE_BINARY_DIR}")
