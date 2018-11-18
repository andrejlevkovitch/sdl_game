# math

cmake_minimum_required(VERSION "3.10")

project(math2d)

set(MATH2D_SRC
  "sources/circle.cpp"
  "sources/rect.cpp"
  "sources/size.cpp"
  "sources/vector2d.cpp"
  "sources/vertex.cpp"
  )

add_library(${PROJECT_NAME} ${MATH2D_SRC})
target_compile_options(${PROJECT_NAME} PRIVATE "-std=c++11")
target_include_directories(${PROJECT_NAME} PUBLIC "include")

if(BUILD_TESTING)
  add_executable(test_math2d "test/test_math2d.cpp")
  target_compile_options(test_math2d PRIVATE "-std=c++11")
  target_link_libraries(test_math2d PUBLIC ${PROJECT_NAME} Catch2::Catch2)
  catch_discover_tests(test_math2d)
endif()
