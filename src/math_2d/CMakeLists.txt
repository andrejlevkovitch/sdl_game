# math

set(MATH2D_LBR "math_2d" CACHE STRING "add in cache" FORCE)
file(GLOB MATH2D_SRC "sources/*.cpp")
set(LIST_TARGETS "${LIST_TARGETS};${MATH2D_LBR}" CACHE STRING "add target" FORCE)

add_library(${MATH2D_LBR} ${MATH2D_SRC})
target_include_directories(${MATH2D_LBR} PUBLIC "include")

if(BUILD_TESTING)
  find_package(Boost COMPONENTS unit_test_framework)
  if (Boost_FOUND)
    set(TEST_MATH2D "test_math2d")
    set(TEST_MATH2D_SRC "test/boost_test_math2d.cpp")
    add_executable(${TEST_MATH2D} ${TEST_MATH2D_SRC})
    target_link_libraries(${TEST_MATH2D} ${MATH2D_LBR})
    add_test(NAME ${TEST_MATH2D} COMMAND ${TEST_MATH2D})
  else()
    message(WARNING "for testing Boost required")
  endif()
endif()
