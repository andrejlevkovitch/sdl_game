// mat33.hpp

#pragma once

#include <array>

namespace levi {
class vertex;

/**\brief row-order matrix in one-dimensional array*/
class mat33 : public std::array<float, 9> {
public:
  mat33();
  vertex operator*(const vertex &rhs);
  mat33 operator*(const mat33 &rhs);
};
} // namespace levi
