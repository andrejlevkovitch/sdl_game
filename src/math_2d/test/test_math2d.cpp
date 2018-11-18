// test_math2d.cpp

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "rect.hpp"
#include "vector2d.hpp"
#include "vertex.hpp"

TEST_CASE("check lenght", "vector2d") {
  levi::vector2d vec{4, 3};
  CHECK(vec.get_length() == Approx(5).margin(0.01));
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

TEST_CASE("test dot", "vector2d") {
  levi::vector2d vec1{-1, 1};
  levi::vector2d vec2{1, 1};

  CHECK(vec1 * vec2 == Approx(0).epsilon(0.01));

  vec1.y = 0;
  vec2.y = 0;
  CHECK(vec1 * vec2 < 0);

  vec1.x = 3;
  CHECK(vec1 * vec2 > 0);

  vec1 = levi::vector2d{3, 2};
  vec2 = levi::vector2d{1, 4};
  CHECK(vec1 * vec2 == Approx(11).epsilon(0.01));
}

TEST_CASE("test get angle", "vector2d") {
  levi::vector2d vec1{2, 2};
  levi::vector2d vec2{-3, 3};

  CHECK(get_angle_bitwin(vec1, vec2) == Approx(90).epsilon(0.01));

  vec2.x = 0;
  CHECK(get_angle_bitwin(vec2, vec1) == Approx(45).epsilon(0.001));

  vec1.x = 52;
  vec1.y = 98;
  vec2.x = 83;
  vec2.y = 280;
  CHECK(get_angle_bitwin(vec1, vec2) ==
        Approx(get_angle_bitwin(vec2, vec1)).epsilon(0.001));

  vec1 = levi::vector2d{3, 6};
  vec2 = levi::vector2d{7, 6};
  levi::vector2d beg_koord{5, 4};
  CHECK(get_angle_bitwin(vec1, vec2, beg_koord) == Approx(90).epsilon(0.001));
}

TEST_CASE("test distance", "vector2d") {
  levi::vector2d vec1{0, 50};
  levi::vector2d vec2{0, 0};

  CHECK(levi::get_distance_bitwin(vec1, vec2) == Approx(50).epsilon(0.01));

  vec2 = levi::vector2d{25, 25};
  CHECK(levi::get_distance_bitwin(vec1, vec2) == Approx(35.355).epsilon(0.01));
}

TEST_CASE("test multiplication on scalar", "vector2d") {
  levi::vector2d vec{5, 3};

  auto rez = vec * 3;
  CHECK(rez.x == Approx(15).epsilon(0.001));
  CHECK(rez.y == Approx(9).epsilon(0.001));
}

TEST_CASE("test get normal", "vector2d") {
  levi::vector2d vec{3, 4};
  auto norm = vec.get_norm();
  CHECK(norm.get_length() == Approx(1).epsilon(0.01));
}

TEST_CASE("test get vector2d from", "vector2d") {
  levi::vector2d vec{0, 0};
  auto vec1 = vec.get_vec_from(4, 60);
  CHECK(vec1.x == Approx(2.0f).epsilon(0.001));
  CHECK(vec1.y == Approx(3.464f).epsilon(0.001));
}

TEST_CASE("test crossing", "rect") {
  levi::rect rect1{0, 0, 64, 64};
  levi::rect rect2{32, 32, 64, 64};
  CHECK(levi::is_crossing(rect1, rect2));
  rect2 = levi::rect{65, 65, 64, 64};
  CHECK(!levi::is_crossing(rect1, rect2));

  rect1 = levi::rect{60, 5, 64, 64};
  CHECK(levi::is_crossing(rect1, rect2));

  rect1 = levi::rect{32, 90, 64, 64};
  CHECK(levi::is_crossing(rect1, rect2));

  rect1 = levi::rect{0, 0, 64, 64};
  rect2 = levi::rect{64, 64, 64, 64};
  CHECK(!levi::is_crossing(rect1, rect2));
}

TEST_CASE("test intake pos", "rect") {
  levi::rect rect{0, 0, 64, 64};
  levi::vector2d pos{0, 0};

  CHECK(rect.is_intake_pos(pos));

  pos = levi::vector2d{64, 64};
  CHECK(!rect.is_intake_pos(pos));

  pos = levi::vector2d{15, 63};
  CHECK(rect.is_intake_pos(pos));

  rect = levi::rect{960, 64, 64, 64};
  pos = levi::vector2d{988, 96};
  CHECK(rect.is_intake_pos(pos));
}
