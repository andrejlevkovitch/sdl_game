// mat33.cpp

#include "mat33.hpp"
#include "vertex.hpp"

const int rows{3};
const int cols{3};

levi::mat33::mat33() : std::array<float, rows * cols>{} {}

levi::vertex levi::mat33::operator*(const vertex &rhs) {
  vertex retval;
  auto arr = reinterpret_cast<float *>(&retval);
  auto rhs_arr = reinterpret_cast<const float *>(&rhs);
  for (int i{}; i < rows; ++i) {
    // clang-format off
    arr[i] = (*this)[i * cols + 0] * rhs_arr[0] +
             (*this)[i * cols + 1] * rhs_arr[1] +
             (*this)[i * cols + 2] * rhs_arr[2];
    // clang-format on
  }
  return retval;
}

levi::mat33 levi::mat33::operator*(const mat33 &rhs) {
  mat33 retval;
  for (int i{}; i < rows; ++i) {
    for (int j{}; j < cols; ++j) {
      // clang-format off
      retval[i * cols + j] = (*this)[i * cols + 0] * rhs[0 * cols + j] +
                             (*this)[i * cols + 1] * rhs[1 * cols + j] +
                             (*this)[i * cols + 2] * rhs[2 * cols + j];
      // clang-fomat on
    }
  }
  return retval;
}

std::ostream &levi::operator<<(std::ostream &out, const mat33 &matrix) {
  for (int i{}; i < 3; ++i) {
    for (int j{}; j < 3; ++j) {
      out << matrix[i * 3 + j] << ' ';
    }
    out << std::endl;
  }
  return out;
}
