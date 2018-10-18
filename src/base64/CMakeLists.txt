# base64

set(BASE64_LIB "base64")
file(GLOB BASE64_SRC "sources/*")
set(LIST_TARGETS "${LIST_TARGETS};${BASE64_LIB}" CACHE STRING "add target" FORCE)

add_library(${BASE64_LIB} ${BASE64_SRC})
target_include_directories(${BASE64_LIB} PUBLIC "include")
