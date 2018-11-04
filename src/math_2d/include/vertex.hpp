// vertex.hpp

#pragma once

#include <ostream>

namespace levi {
class vertex {
public:
  vertex();
  vertex(float x, float y, float z);

public:
  float x;
  float y;
  float z;
};

std::ostream &operator<<(std::ostream &out, const vertex &vertex);
}; // namespace levi
