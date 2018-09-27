// boost_test_math2d.cpp

#define BOOST_TEST_MODULE test_math2d
#include <boost/test/included/unit_test.hpp>

#include "vector2d.hpp"

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
  BOOST_CHECK(vec3.get_x() == x1 + x2);
  BOOST_CHECK(vec3.get_y() == y1 + y2);

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

  vec1.set_y(0);
  vec2.set_y(0);
  BOOST_CHECK(vec1 * vec2 < 0);

  vec1.set_x(3);
  BOOST_CHECK(vec1 * vec2 > 0);

  vec1 = levi::vector2d{3, 2};
  vec2 = levi::vector2d{1, 4};
  BOOST_CHECK_CLOSE_FRACTION(vec1 * vec2, 11, 0.001);
}

BOOST_AUTO_TEST_CASE(test_get_angle) {
  levi::vector2d vec1{2, 2};
  levi::vector2d vec2{-3, 3};

  BOOST_CHECK_CLOSE_FRACTION(get_angle_bitwin(vec1, vec2), 90, 0.001);

  vec2.set_x(0);
  BOOST_CHECK_CLOSE_FRACTION(get_angle_bitwin(vec2, vec1), 45, 0.001);

  vec1.set_x(52);
  vec1.set_y(98);
  vec2.set_x(83);
  vec2.set_y(280);
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
  BOOST_CHECK_CLOSE_FRACTION(rez.get_x(), 15, 0.001);
  BOOST_CHECK_CLOSE_FRACTION(rez.get_y(), 9, 0.001);
}

BOOST_AUTO_TEST_CASE(test_get_norm) {
  levi::vector2d vec{3, 4};
  auto norm = vec.get_norm();
  BOOST_CHECK_CLOSE_FRACTION(norm.get_length(), 1, 0.001);
}

BOOST_AUTO_TEST_CASE(test_get_vec_from) {
  levi::vector2d vec{0, 0};
  auto vec1 = vec.get_vec_from(4, 60);
  BOOST_CHECK_CLOSE_FRACTION(vec1.get_x(), 2.0f, 0.001);
  BOOST_CHECK_CLOSE_FRACTION(vec1.get_y(), 3.464f, 0.001);
}

BOOST_AUTO_TEST_SUITE_END()
