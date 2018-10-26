# engine

find_package(SDL2 REQUIRED)

find_library(TinyXML_LIBRARY tinyxml)
if (TinyXML_LIBRARY STREQUAL "TinyXML_LIBRARY-NOTFOUND")
  message(FATAL_ERROR "tinyxml library not founded")
endif()

find_library(PNG_LIBRARY png)
if (PNG_LIBRARY STREQUAL "PNG_LIBRARY-NOTFOUND")
  message(FATAL_ERROR "png library not founded")
endif()

find_package(OpenGL REQUIRED)

configure_file("shaders_config.hpp.cmake" "${CMAKE_BINARY_DIR}/shaders_config.hpp")

set(ENGINE_LBR "engine")
file(GLOB ENGINE_SRC "sources/*.cpp")
set(LIST_TARGETS "${LIST_TARGETS};${ENGINE_LBR}" CACHE STRING "add target" FORCE)

add_library(${ENGINE_LBR} ${ENGINE_SRC})
target_link_libraries(${ENGINE_LBR} ${SDL2_LIBRARY} ${TinyXML_LIBRARY} ${PNG_LIBRARY} ${OPENGL_gl_LIBRARY})

target_include_directories(${ENGINE_LBR} PUBLIC "include")
target_include_directories(${ENGINE_LBR} PRIVATE "../math_2d/include")
target_include_directories(${ENGINE_LBR} PRIVATE "${CMAKE_BINARY_DIR}")
