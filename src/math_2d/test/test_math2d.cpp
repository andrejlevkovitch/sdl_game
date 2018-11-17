// test_math2d.cpp

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "mat33.hpp"
#include "rect.hpp"
#include "vector2d.hpp"
#include "vertex.hpp"

TEST_CASE("check lenght", "vector2d") {
  levi::vector2d vec{4, 3};
  REQUIRE(vec.get_length() == Approx(5).margin(0.01));
}

TEST_CASE("addition", "vector2d") {
  int x1 = 5;
  int y1 = 15;
  int x2 = 98;
  int y2 = 87;
  levi::vector2d vec1(x1, y1);
  levi::vector2d vec2(x2, y2);

  auto vec3 = vec1 + vec2;
  CHECK(vec3.x == Approx(x1 + x2).epsilon(0.01));
  CHECK(vec3.y == Approx(y1 + y2).epsilon(0.01));

  vec1 += vec2;
  CHECK(vec3 == vec1);
  vec1 -= vec2;
  CHECK(vec3 == (vec1 + vec2));
  CHECK((vec3 - vec1) == vec2);
}
