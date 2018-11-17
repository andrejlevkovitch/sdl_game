// mat33.hpp

#pragma once

#include <array>
#include <ostream>

namespace levi {
class vertex;

// TODO change order like in opengl
/**\brief row-order matrix in one-dimensional array*/
class mat33 : public std::array<float, 9> {
public:
  mat33();
  vertex operator*(const vertex &rhs);
  mat33 operator*(const mat33 &rhs);
};

std::ostream &operator<<(std::ostream &out, const mat33 &matrix);
} // namespace levi
