# engine

set(ENGINE_LBR "engine")
file(GLOB ENGINE_SRC "sources/*.cpp")
set(LIST_TARGETS "${LIST_TARGETS};${ENGINE_LBR}" CACHE STRING "add target" FORCE)

add_library(${ENGINE_LBR} ${ENGINE_SRC})
target_link_libraries(${ENGINE_LBR} -ltinyxml)

target_include_directories(${ENGINE_LBR} PUBLIC "include")
target_include_directories(${ENGINE_LBR} PRIVATE "../math_2d/include")
