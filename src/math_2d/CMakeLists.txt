# math

cmake_minimum_required(VERSION "3.10")

project(math2d)

set(MATH2D_SRC
  "sources/circle.cpp"
  "sources/mat33.cpp"
  "sources/rect.cpp"
  "sources/size.cpp"
  "sources/vector2d.cpp"
  "sources/vertex.cpp"
  )

add_library(${PROJECT_NAME} ${MATH2D_SRC})

target_compile_options(${PROJECT_NAME} PRIVATE "-std=c++11")

target_include_directories(${PROJECT_NAME} PUBLIC "include")

if(BUILD_TESTING)
  find_package(Boost COMPONENTS unit_test_framework)
  if (Boost_FOUND)
    set(TEST_MATH2D "boost_test_math2d")
    set(TEST_MATH2D_SRC "test/boost_test_math2d.cpp")
    add_executable(${TEST_MATH2D} ${TEST_MATH2D_SRC})
    target_compile_options(${TEST_MATH2D} PRIVATE "-std=c++11")
    target_link_libraries(${TEST_MATH2D} ${PROJECT_NAME})
    add_test(NAME ${TEST_MATH2D} COMMAND ${TEST_MATH2D})
  else()
    message(WARNING "for testing Boost required")
  endif()
endif()

if(BUILD_TESTING)
  add_executable(test_math2d "test/test_math2d.cpp")
  target_compile_options(test_math2d PRIVATE "-std=c++11")
  target_link_libraries(test_math2d PUBLIC ${PROJECT_NAME} Catch2::Catch2)
  catch_discover_tests(test_math2d)
endif()
