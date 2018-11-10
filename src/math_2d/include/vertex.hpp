// vertex.hpp

#pragma once

#include "vector2d.hpp"
#include <ostream>

namespace levi {
class vertex {
public:
  vertex();
  vertex(float x, float y, float z);
  vertex(vector2d v, float z);
  vertex operator+(const vertex &rhs) const;
  operator levi::vector2d() const;

public:
  float x;
  float y;
  float z;
};

std::ostream &operator<<(std::ostream &out, const vertex &vertex);
}; // namespace levi
