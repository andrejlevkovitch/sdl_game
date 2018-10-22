// boost_test_math2d.cpp

#define BOOST_TEST_MODULE test_math2d
#include <boost/test/included/unit_test.hpp>

#include "mat33.hpp"
#include "rect.hpp"
#include "vector2d.hpp"
#include "vertex.hpp"

BOOST_AUTO_TEST_SUITE(test_vector2d)
BOOST_AUTO_TEST_CASE(test_get_lenght) {
  levi::vector2d vec{4, 3};
  BOOST_CHECK_CLOSE_FRACTION(vec.get_length(), 5, 0.001);
}

BOOST_AUTO_TEST_CASE(test_addition) {
  int x1 = 5;
  int y1 = 15;
  int x2 = 98;
  int y2 = 87;
  levi::vector2d vec1(x1, y1);
  levi::vector2d vec2(x2, y2);

  auto vec3 = vec1 + vec2;
  BOOST_CHECK(vec3.x == x1 + x2);
  BOOST_CHECK(vec3.y == y1 + y2);

  vec1 += vec2;
  BOOST_CHECK(vec3 == vec1);
  vec1 -= vec2;
  BOOST_CHECK(vec3 == (vec1 + vec2));
  BOOST_CHECK((vec3 - vec1) == vec2);
}

BOOST_AUTO_TEST_CASE(test_dot) {
  levi::vector2d vec1{-1, 1};
  levi::vector2d vec2{1, 1};

  BOOST_CHECK_CLOSE_FRACTION(vec1 * vec2, 0, 0.001);

  vec1.y = 0;
  vec2.y = 0;
  BOOST_CHECK(vec1 * vec2 < 0);

  vec1.x = 3;
  BOOST_CHECK(vec1 * vec2 > 0);

  vec1 = levi::vector2d{3, 2};
  vec2 = levi::vector2d{1, 4};
  BOOST_CHECK_CLOSE_FRACTION(vec1 * vec2, 11, 0.001);
}

BOOST_AUTO_TEST_CASE(test_get_angle) {
  levi::vector2d vec1{2, 2};
  levi::vector2d vec2{-3, 3};

  BOOST_CHECK_CLOSE_FRACTION(get_angle_bitwin(vec1, vec2), 90, 0.001);

  vec2.x = 0;
  BOOST_CHECK_CLOSE_FRACTION(get_angle_bitwin(vec2, vec1), 45, 0.001);

  vec1.x = 52;
  vec1.y = 98;
  vec2.x = 83;
  vec2.y = 280;
  BOOST_CHECK_CLOSE_FRACTION(get_angle_bitwin(vec1, vec2),
                             get_angle_bitwin(vec2, vec1), 0.001);

  vec1 = levi::vector2d{3, 6};
  vec2 = levi::vector2d{7, 6};
  levi::vector2d beg_koord{5, 4};
  BOOST_CHECK_CLOSE_FRACTION(get_angle_bitwin(vec1, vec2, beg_koord), 90,
                             0.001);
}

BOOST_AUTO_TEST_CASE(test_multiplication_on_scalar) {
  levi::vector2d vec{5, 3};

  auto rez = vec * 3;
  BOOST_CHECK_CLOSE_FRACTION(rez.x, 15, 0.001);
  BOOST_CHECK_CLOSE_FRACTION(rez.y, 9, 0.001);
}

BOOST_AUTO_TEST_CASE(test_get_norm) {
  levi::vector2d vec{3, 4};
  auto norm = vec.get_norm();
  BOOST_CHECK_CLOSE_FRACTION(norm.get_length(), 1, 0.001);
}

BOOST_AUTO_TEST_CASE(test_get_vec_from) {
  levi::vector2d vec{0, 0};
  auto vec1 = vec.get_vec_from(4, 60);
  BOOST_CHECK_CLOSE_FRACTION(vec1.x, 2.0f, 0.001);
  BOOST_CHECK_CLOSE_FRACTION(vec1.y, 3.464f, 0.001);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_mat33)
BOOST_AUTO_TEST_CASE(test_multiplication_matrix_on_vertex) {
  levi::mat33 matrix;
  matrix[0] = 0.5;
  matrix[2] = 0.3;
  matrix[4] = 2;
  matrix[5] = -1;
  matrix[8] = 1;

  levi::vertex vertex{5, 8};
  auto rezult = matrix * vertex;
  BOOST_CHECK_CLOSE_FRACTION(rezult.x, 2.8, 0.001);
  BOOST_CHECK_CLOSE_FRACTION(rezult.y, 15, 0.001);
  BOOST_CHECK_CLOSE_FRACTION(rezult.z, 1, 0.001);
}

BOOST_AUTO_TEST_CASE(test_multiplication_matrixs) {
  levi::mat33 matrix1;
  matrix1[0] = 1;
  matrix1[1] = 2;
  matrix1[2] = 3;
  matrix1[3] = 4;
  matrix1[4] = 5;
  matrix1[5] = 6;
  matrix1[6] = 7;
  matrix1[7] = 8;
  matrix1[8] = 9;

  levi::mat33 matrix2;
  matrix2[0] = 9;
  matrix2[1] = 8;
  matrix2[2] = 7;
  matrix2[3] = 6;
  matrix2[4] = 5;
  matrix2[5] = 4;
  matrix2[6] = 3;
  matrix2[7] = 2;
  matrix2[8] = 1;

  auto rezult = matrix1 * matrix2;
  BOOST_CHECK_CLOSE_FRACTION(rezult[0], 30, 0.001);
  BOOST_CHECK_CLOSE_FRACTION(rezult[1], 24, 0.001);
  BOOST_CHECK_CLOSE_FRACTION(rezult[2], 18, 0.001);
  BOOST_CHECK_CLOSE_FRACTION(rezult[3], 84, 0.001);
  BOOST_CHECK_CLOSE_FRACTION(rezult[4], 69, 0.001);
  BOOST_CHECK_CLOSE_FRACTION(rezult[5], 54, 0.001);
  BOOST_CHECK_CLOSE_FRACTION(rezult[6], 138, 0.001);
  BOOST_CHECK_CLOSE_FRACTION(rezult[7], 114, 0.001);
  BOOST_CHECK_CLOSE_FRACTION(rezult[8], 90, 0.001);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_rect);
BOOST_AUTO_TEST_CASE(test_crossing) {
  levi::rect rect1{0, 0, 64, 64};
  levi::rect rect2{32, 32, 64, 64};
  BOOST_CHECK(levi::is_crossing(rect1, rect2));
  rect2 = levi::rect{65, 65, 64, 64};
  BOOST_CHECK(!levi::is_crossing(rect1, rect2));

  rect1 = levi::rect{60, 5, 64, 64};
  BOOST_CHECK(levi::is_crossing(rect1, rect2));

  rect1 = levi::rect{32, 90, 64, 64};
  BOOST_CHECK(levi::is_crossing(rect1, rect2));

  rect1 = levi::rect{0, 0, 64, 64};
  rect2 = levi::rect{64, 64, 64, 64};
  BOOST_CHECK(!levi::is_crossing(rect1, rect2));
}

BOOST_AUTO_TEST_CASE(test_intake_pos) {
  levi::rect rect{0, 0, 64, 64};
  levi::vector2d pos{0, 0};

  BOOST_CHECK(rect.is_intake_pos(pos));

  pos = levi::vector2d{64, 64};
  BOOST_CHECK(!rect.is_intake_pos(pos));

  pos = levi::vector2d{15, 63};
  BOOST_CHECK(rect.is_intake_pos(pos));

  rect = levi::rect{960, 64, 64, 64};
  pos = levi::vector2d{988, 96};
  BOOST_CHECK(rect.is_intake_pos(pos));
}
BOOST_AUTO_TEST_SUITE_END();
